// CINFCityCashShop.cpp: implementation of the CINFCityCashShop class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFCityCashShop.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "D3DHanFont.h"
#include "AtumDatabase.h"
#include "INFGameMain.h"
#include "INFIcon.h"
#include "ItemInfo.h"
#include "INFItemInfo.h"
#include "INFScrollBar.h"
#include "FieldWinSocket.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "Interface.h"
#include "INFCityBase.h"
#include "INFWindow.h"
#include "INFGameMain.h"
#include "Chat.h"
#include "dxutil.h"
#include "StoreData.h"
#include "INFInven.h"
#include "AtumSound.h"	// 2009-02-24 by bhsohn ĳ���� ���� �������� �߰�
#include "INFImageEx.h"	// 2011. 11. 17 by jskim CINFImage -> CINFImageEx ����
#include "INFGroupImage.h"
#include "INFGroupManager.h"
#include "INFToolTip.h"
// 2012-11-23 by jhjang �������� �� ���� ���� �۾�
#include "md5_lib_src.h"
// end 2012-11-23 by jhjang �������� �� ���� ���� �۾�

// 2011. 11. 17 by jskim EP4 UI ����
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
#define		CASHSHOP_RENDER_START_X				((int)( g_pD3dApp->GetBackBufferDesc().Width / 2 - ( m_pImgBack->GetMaxPos().x - m_pImgBack->GetMinPos().x ) / 2 ) )		// ���� ������ ������ġ	X
#define		CASHSHOP_RENDER_START_Y				((int)(g_pD3dApp->GetBackBufferDesc().Height/2) - 250)			// ���� ������ ������ġ	Y
#define		CASHSHOP_RENDER_TAB_X				(5+CASHSHOP_RENDER_START_X)			// ������ ������ ���� ��ġ
#define		CASHSHOP_RENDER_TAB_Y				(28+CASHSHOP_RENDER_START_Y)			// ������ ������ ���� ��ġ
#define		CASHSHOP_RENDER_BUY_BUTTON_X		(CASHSHOP_RENDER_START_X + 450)		// �����ϱ� ���� ��ġ
#define		CASHSHOP_RENDER_BUY_BUTTON_Y		(CASHSHOP_RENDER_START_Y + 482)		// �����ϱ� ���� ��ġ
#define		CASHSHOP_RENDER_BUY_BUTTON_W		25									// ��ư ������
#define		CASHSHOP_RENDER_BUY_BUTTON_H		25									// ��ư ������
// 2012-11-08 by mspark, ĳ���� ���� ��� ����
//#ifdef C_CASHSHOP_IMAGE_REMOVE
#if defined(WIKIGAMES_ENG) || defined(DEVX) // 2013-06-27 by bhsohn ĳ���� "Filling up"��ư �����
#define		CASHSHOP_RENDER_CHARGE_BUTTON_X		(CASHSHOP_RENDER_START_X + 540)
#define		CASHSHOP_RENDER_CHARGE_BUTTON_Y		(CASHSHOP_RENDER_START_Y + 482)
#else
#define		CASHSHOP_RENDER_CHARGE_BUTTON_X		(CASHSHOP_RENDER_START_X + 510)		// �����ϱ� ���� ��ġ
#define		CASHSHOP_RENDER_CHARGE_BUTTON_Y		(CASHSHOP_RENDER_START_Y + 482)		// �����ϱ� ���� ��ġ
#endif
// end 2012-11-08 by mspark, ĳ���� ���� ��� ����
#define		CASHSHOP_RENDER_CHARGE_BUTTON_W		25									// ��ư ������
#define		CASHSHOP_RENDER_CHARGE_BUTTON_H		25									// ��ư ������
#if defined(WIKIGAMES_ENG) || defined(DEVX) // 2013-06-27 by bhsohn ĳ���� "Filling up"��ư �����
#define		CASHSHOP_RENDER_GIFT_BUTTON_X		(CASHSHOP_RENDER_START_X + 510)		// �����ϱ� ���� ��ġ
#define		CASHSHOP_RENDER_GIFT_BUTTON_Y		(CASHSHOP_RENDER_START_Y + 482)		// �����ϱ� ���� ��ġ
#else// 2007-11-14 by dgwoo �����ϱ� ��ư.
#define		CASHSHOP_RENDER_GIFT_BUTTON_X		(CASHSHOP_RENDER_START_X + 480)		// �����ϱ� ���� ��ġ
#define		CASHSHOP_RENDER_GIFT_BUTTON_Y		(CASHSHOP_RENDER_START_Y + 482)		// �����ϱ� ���� ��ġ
#endif

// 2009-02-24 by bhsohn ĳ���� ���� �������� �߰�
#define		CASHSHOP_RENDER_CHARGE_BTN_X		(CASHSHOP_RENDER_START_X + 540)		// �����ϱ� ���� ��ġ
#define		CASHSHOP_RENDER_CHARGE_BTN_Y		(CASHSHOP_RENDER_START_Y + 482)		// �����ϱ� ���� ��ġ
// end 2009-02-24 by bhsohn ĳ���� ���� �������� �߰�

#define		CASHSHOP_RENDER_FONT_ICONINFO_X		(CASHSHOP_RENDER_START_X+124)		// ������ ���� �������� ���� ���� ��ġ
#define		CASHSHOP_RENDER_FONT_ICONINFO_Y		(CASHSHOP_RENDER_START_Y+62)		// ������ ���� �������� ���� ���� ��ġ
#define		CASHSHOP_RENDER_FONT_ICONINFO_GAB	64									// ������ ���� ������ ��
#define		CASHSHOP_RENDER_SELECT_TAB_GAB		24									// ����Ʈ �� ������ ��

#define		CASHSHOP_RENDER_BIGICON_START_X		(CASHSHOP_RENDER_START_X+77)		// ĳ�������� ���� ��ġ
#define		CASHSHOP_RENDER_BIGICON_START_Y		(CASHSHOP_RENDER_START_Y+77)		// ĳ�������� ���� ��ġ
#define		CASHSHOP_RENDER_BIGICON_W			84									// ĳ�� ������� W
#define		CASHSHOP_RENDER_BIGICON_H			64									// ĳ�� ������� H

#define		CASHSHOP_RENDER_CITY_GAB			88		// ���ÿ� �ʵ� ���������� ��
#define		CASHSHOP_RENDER_GAME_GAB			41		// ���ÿ� �ʵ� ���������� ��
#define		CASHSHOP_FONT_LINE_HEIGHT			15		// �� ������ ����
#define		CASHSHOP_SIDE_WIDTH_ENGLISH			5.5		// ���� ���� WIDTH
#define		CASHSHOP_SHOW_ICON_MAX				5		// �����ټ� �ִ� �ִ� ����

#define		CASHSHOP_BUTTON_STATE_NOM			0		// ��ư ����
#define		CASHSHOP_BUTTON_STATE_OVE			1		// ��ư ����
#define		CASHSHOP_BUTTON_STATE_PUH			2		// ��ư ����
#define		CASHSHOP_BUTTON_STATE_DIS			3		// ��ư ����

#define		CASHSHOP_SCROLL_START_X				(CASHSHOP_RENDER_START_X+560)
#define		CASHSHOP_SCROLL_START_Y				(CASHSHOP_RENDER_START_Y+68)
#define		CASHSHOP_SCROLL_LINE_LENGTH			390//370
#define		CASHSHOP_SCROLL_RECT_START_X		(CASHSHOP_RENDER_START_X+14)
#define		CASHSHOP_SCROLL_RECT_START_Y		(CASHSHOP_RENDER_START_Y+62)
// 	#define		CASHSHOP_SCROLL_RECT_W				(CASHSHOP_RENDER_START_X+540)//314)	// 2008-08-13 by dgwoo ������ ����.
// 	#define		CASHSHOP_SCROLL_RECT_H				(CASHSHOP_RENDER_START_Y+364)//364
#define		CASHSHOP_SCROLL_RECT_W				(CASHSHOP_RENDER_START_X+575)//+540)//314) // 2013-02-15 by jhseol, ĳ���� ��ũ�� ���� ����
#define		CASHSHOP_SCROLL_RECT_H				(CASHSHOP_RENDER_START_Y+480)//+364)//364) // 2013-02-15 by jhseol, ĳ���� ��ũ�� ���� ����


#define		CASHSHOP_ITEM_INFO_ICON_X			(CASHSHOP_RENDER_START_X+540)//416)		// ������ ����â�� ������ ������ ������
#define		CASHSHOP_ITEM_INFO_ICON_Y			(CASHSHOP_RENDER_START_Y+49)		// ������ ����â�� ������ ������ ������
#define		CASHSHOP_ITEM_INFO_NAME_X			(CASHSHOP_ITEM_INFO_ICON_X+42)		// ������ ����â�� ������ �̸� ������ 
#define		CASHSHOP_ITEM_INFO_NAME_Y			(CASHSHOP_ITEM_INFO_ICON_Y+62)		// ������ ����â�� ������ �̸� ������(�̸��� ��� ����)
#define		CASHSHOP_ITEM_INFO_X				(CASHSHOP_RENDER_START_X+361)		// ������ ����â�� ������ �̸� ������ 

#define		CASHSHOP_ITEM_SELECT_RECT_X			(CASHSHOP_RENDER_START_X+23)			// ������ ���� ����â 
#define		CASHSHOP_ITEM_SELECT_RECT_Y			(CASHSHOP_RENDER_START_Y+54)			// ������ ���� ����â 
#define		CASHSHOP_ITEM_SELECT_RECT_W			405
#define		CASHSHOP_ITEM_SELECT_RECT_H			64

#define		CASHSHOP_ITEM_TAB_X					(CASHSHOP_RENDER_TAB_X)
#define		CASHSHOP_ITEM_TAB_Y					(CASHSHOP_RENDER_TAB_Y)
#define		CASHSHOP_ITEM_TAB_W					80
#define		CASHSHOP_ITEM_TAB_H					25

#define		CASHSHOP_ITEM_CLOSE_X				(19)
#define		CASHSHOP_ITEM_CLOSE_Y				(8)
#define		CASHSHOP_ITEM_CLOSE_BUTTON_W		16
#define		CASHSHOP_ITEM_CLOSE_BUTTON_H		16
#define		CASHSHOP_ITEM_SHOP_MOVE_H			19
#define		CASHSHOP_ITEM_SHOP_MOVE_W			591

#define		CASHSHOP_TIME_MORE_ITEM				0.5f
#define		CASH_SHOP_ITEM_INFO_DES				8

#define		CASH_SHOP_GEAR_ROTATION_KEY_X		475//342			// 2008-10-28 by dgwoo ĳ������ �������̽� ����.
#define		CASH_SHOP_GEAR_ROTATION_KEY_Y		254
#define		CASH_SHOP_NUM						110

// 2008-10-28 by bhsohn ĳ�� ���� ���� ����
// Mirror ��ġ
#define		CASH_SHOP_GEAR_UNIT_X			495
#define		CASH_SHOP_GEAR_UNIT_Y			94
// end 2008-10-28 by bhsohn ĳ�� ���� ���� ����

#define ROTATION_NONE			-1
#define ROTATION_LEFT			0
#define ROTATION_RIGHT			1
#define ROTATION_UP				2
#define ROTATION_DOWN			3
#define ROTATION_ORIGINAL		4

#define ROTATION_STATE_N		-1		// ����
#define ROTATION_STATE_O		0		// ����
#define ROTATION_STATE_P		1		// ����

#define SOURCEINDEXTOITEMNUM(i,j)	(((i/100)*100)+j)

#define		STRING_CULL ::StringCullingUserData_ToBlank
#define		CASHSHOP_ITEM_DESC_MAX				210

#define		CASHSHOP_TEX						1.0f
// 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
//#ifdef C_CASHSHOP_IMAGE_REMOVE
#if defined(WIKIGAMES_ENG) || defined(DEVX) // 2013-06-27 by bhsohn ĳ���� "Filling up"��ư �����
#define		CASHSHOP_RENDER_MYCASH_X			(CASHSHOP_RENDER_START_X + 500)
#else
#define		CASHSHOP_RENDER_MYCASH_X			(CASHSHOP_RENDER_START_X + 463)		// ��ĳ�� ������ ������
#endif
// end 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
#define		CASHSHOP_RENDER_MYCASH_Y			(CASHSHOP_RENDER_START_Y + 490)		// ��ĳ�� ������ ������(�⺻ 12DIGIT)

#define		CASHSHOP_TAB_W_SIZE					3
#define		CASHSHOP_TAB_H_SIZE					3
#define		CASHSHOP_TAB_W						169
#define		CASHSHOP_TAB_H						135

#define		CASHSHOP_NEW_ICON_W					33
#define		CASHSHOP_NEW_ICON_H					11

#define		CASHSHOP_FONT_MAX_SIZE				160

#define		CASHSHOP_GAB						12
#define		CASHSHOP_ITME_POS_GAB				38
#define		CASHSHOP_SELECT_IMAGE_X				40
#define		CASHSHOP_SELECT_IMAGE_Y				5

#define		SCROLL_LINE_X_SIZE					9
#define		SCROLL_LINE_Y_SIZE					23
#define		SOLDOUTCOLOR						RGB( 129, 127, 128 );

#define		RENDER_TOOLTIP_TOP_GAB				28
#define		RENDER_TOOLTIP_CEN_GAB				20
#define		RENDER_TOOLTIP_BOT_GAB				24
#define		RENDER_TOOLTIP_LEFT_GAB				17
#define		RENDER_TOOLTIP_RIGHT_GAB			17
#else // C_EPSODE4_UI_CHANGE_JSKIM
#define		CASHSHOP_RENDER_START_X				((int)(g_pD3dApp->GetBackBufferDesc().Width - 591 ) /2 )		// ���� ������ ������ġ	X
#define		CASHSHOP_RENDER_START_Y				((int)(g_pD3dApp->GetBackBufferDesc().Height/2) - 199)			// ���� ������ ������ġ	Y
#define		CASHSHOP_RENDER_TAB_X				(14+CASHSHOP_RENDER_START_X)			// ������ ������ ���� ��ġ
#define		CASHSHOP_RENDER_TAB_Y				(28+CASHSHOP_RENDER_START_Y)			// ������ ������ ���� ��ġ
#define		CASHSHOP_RENDER_BUY_BUTTON_X		(512+CASHSHOP_RENDER_START_X)		// �����ϱ� ���� ��ġ
#define		CASHSHOP_RENDER_BUY_BUTTON_Y		(357+CASHSHOP_RENDER_START_Y)		// �����ϱ� ���� ��ġ
#define		CASHSHOP_RENDER_BUY_BUTTON_W		68									// ��ư ������
#define		CASHSHOP_RENDER_BUY_BUTTON_H		24									// ��ư ������
#define		CASHSHOP_RENDER_CHARGE_BUTTON_X		(545+CASHSHOP_RENDER_START_X)		// �����ϱ� ���� ��ġ
#define		CASHSHOP_RENDER_CHARGE_BUTTON_Y		(299+CASHSHOP_RENDER_START_Y)		// �����ϱ� ���� ��ġ
#define		CASHSHOP_RENDER_CHARGE_BUTTON_W		90									// ��ư ������
#define		CASHSHOP_RENDER_CHARGE_BUTTON_H		29									// ��ư ������
// 2007-11-14 by dgwoo �����ϱ� ��ư.
#define		CASHSHOP_RENDER_GIFT_BUTTON_X		(545+CASHSHOP_RENDER_START_X)		// �����ϱ� ���� ��ġ
#define		CASHSHOP_RENDER_GIFT_BUTTON_Y		(329+CASHSHOP_RENDER_START_Y)		// �����ϱ� ���� ��ġ

// 2009-02-24 by bhsohn ĳ���� ���� �������� �߰�
#define		CASHSHOP_RENDER_CHARGE_BTN_X		(643+CASHSHOP_RENDER_START_X)		// �����ϱ� ���� ��ġ
#define		CASHSHOP_RENDER_CHARGE_BTN_Y		(357+CASHSHOP_RENDER_START_Y)		// �����ϱ� ���� ��ġ
// end 2009-02-24 by bhsohn ĳ���� ���� �������� �߰�

#define		CASHSHOP_RENDER_FONT_ICONINFO_X		(CASHSHOP_RENDER_START_X+124)		// ������ ���� �������� ���� ���� ��ġ
#define		CASHSHOP_RENDER_FONT_ICONINFO_Y		(CASHSHOP_RENDER_START_Y+62)		// ������ ���� �������� ���� ���� ��ġ
#define		CASHSHOP_RENDER_FONT_ICONINFO_GAB	64									// ������ ���� ������ ��
#define		CASHSHOP_RENDER_SELECT_TAB_GAB		24									// ����Ʈ �� ������ ��

#define		CASHSHOP_RENDER_BIGICON_START_X		(CASHSHOP_RENDER_START_X+25)		// ĳ�������� ���� ��ġ
#define		CASHSHOP_RENDER_BIGICON_START_Y		(CASHSHOP_RENDER_START_Y+55)		// ĳ�������� ���� ��ġ
#define		CASHSHOP_RENDER_BIGICON_W			84									// ĳ�� ������� W
#define		CASHSHOP_RENDER_BIGICON_H			64									// ĳ�� ������� H

#define		CASHSHOP_RENDER_CITY_GAB			88		// ���ÿ� �ʵ� ���������� ��
#define		CASHSHOP_RENDER_GAME_GAB			41		// ���ÿ� �ʵ� ���������� ��
#define		CASHSHOP_FONT_LINE_HEIGHT			15		// �� ������ ����
#define		CASHSHOP_SIDE_WIDTH_ENGLISH			5.5		// ���� ���� WIDTH
#define		CASHSHOP_SHOW_ICON_MAX				5		// �����ټ� �ִ� �ִ� ����

#define		CASHSHOP_BUTTON_STATE_NOM			0		// ��ư ����
#define		CASHSHOP_BUTTON_STATE_OVE			1		// ��ư ����
#define		CASHSHOP_BUTTON_STATE_PUH			2		// ��ư ����
#define		CASHSHOP_BUTTON_STATE_DIS			3		// ��ư ����

#define		CASHSHOP_SCROLL_START_X				(CASHSHOP_RENDER_START_X+441)
#define		CASHSHOP_SCROLL_START_Y				(CASHSHOP_RENDER_START_Y+62)
#define		CASHSHOP_SCROLL_LINE_LENGTH			297
#define		CASHSHOP_SCROLL_RECT_START_X		(CASHSHOP_RENDER_START_X+14)
#define		CASHSHOP_SCROLL_RECT_START_Y		(CASHSHOP_RENDER_START_X+41)
#define		CASHSHOP_SCROLL_RECT_W				(CASHSHOP_RENDER_START_X+440)//314)	// 2008-08-13 by dgwoo ������ ����.
#define		CASHSHOP_SCROLL_RECT_H				(CASHSHOP_RENDER_START_X+339)

#define		CASHSHOP_ITEM_INFO_ICON_X			(CASHSHOP_RENDER_START_X+540)//416)		// ������ ����â�� ������ ������ ������
#define		CASHSHOP_ITEM_INFO_ICON_Y			(CASHSHOP_RENDER_START_Y+49)		// ������ ����â�� ������ ������ ������
#define		CASHSHOP_ITEM_INFO_NAME_X			(CASHSHOP_ITEM_INFO_ICON_X+42)		// ������ ����â�� ������ �̸� ������ 
#define		CASHSHOP_ITEM_INFO_NAME_Y			(CASHSHOP_ITEM_INFO_ICON_Y+62)		// ������ ����â�� ������ �̸� ������(�̸��� ��� ����)
#define		CASHSHOP_ITEM_INFO_X				(CASHSHOP_RENDER_START_X+361)		// ������ ����â�� ������ �̸� ������ 

#define		CASHSHOP_ITEM_SELECT_RECT_X			(CASHSHOP_RENDER_START_X+23)			// ������ ���� ����â 
#define		CASHSHOP_ITEM_SELECT_RECT_Y			(CASHSHOP_RENDER_START_Y+54)			// ������ ���� ����â 
#define		CASHSHOP_ITEM_SELECT_RECT_W			405
#define		CASHSHOP_ITEM_SELECT_RECT_H			64

#define		CASHSHOP_ITEM_TAB_X					(CASHSHOP_RENDER_TAB_X)
#define		CASHSHOP_ITEM_TAB_Y					(CASHSHOP_RENDER_TAB_Y)
#define		CASHSHOP_ITEM_TAB_W					80
#define		CASHSHOP_ITEM_TAB_H					14

#define		CASHSHOP_ITEM_CLOSE_X				(CASHSHOP_RENDER_START_X+700)
#define		CASHSHOP_ITEM_CLOSE_Y				(CASHSHOP_RENDER_START_Y+3)
#define		CASHSHOP_ITEM_CLOSE_BUTTON_W		16
#define		CASHSHOP_ITEM_CLOSE_BUTTON_H		16
#define		CASHSHOP_ITEM_SHOP_MOVE_H			19
#define		CASHSHOP_ITEM_SHOP_MOVE_W			591

#define		CASHSHOP_TIME_MORE_ITEM				0.5f
#define		CASH_SHOP_ITEM_INFO_DES				8

#define		CASH_SHOP_GEAR_ROTATION_KEY_X		475//342			// 2008-10-28 by dgwoo ĳ������ �������̽� ����.
#define		CASH_SHOP_GEAR_ROTATION_KEY_Y		254

// 2008-10-28 by bhsohn ĳ�� ���� ���� ����
// Mirror ��ġ
#define		CASH_SHOP_GEAR_UNIT_X			495
#define		CASH_SHOP_GEAR_UNIT_Y			94
// end 2008-10-28 by bhsohn ĳ�� ���� ���� ����

#define ROTATION_NONE			-1
#define ROTATION_LEFT			0
#define ROTATION_RIGHT			1
#define ROTATION_UP				2
#define ROTATION_DOWN			3
#define ROTATION_ORIGINAL		4

#define ROTATION_STATE_N		-1		// ����
#define ROTATION_STATE_O		0		// ����
#define ROTATION_STATE_P		1		// ����

#define SOURCEINDEXTOITEMNUM(i,j)	(((i/100)*100)+j)

//#define		CASHSHOP_ITEM_PREMIUM_CARD			0
//#define		CASHSHOP_ITEM_OPTION_ITEM			1
//#define		CASHSHOP_ITEM_CARD_ITEM				2
//#define		CASHSHOP_ITEM_ACCESSORY_ITEM		3
//#define		CASHSHOP_ITEM_SKIN_ITEM				4							// 2007-08-07 by dgwoo ��Ų �� �߰�.
//#define		CASHSHOP_ITEM_PACKAGE				5							// 2008-10-20 by dgwoo ��Ű�� �� �߰�


// 2006-06-07 by ispark
#ifdef LANGUAGE_ENGLISH
#define		STRING_CULL ::StringCullingUserData_ToBlank
#define		CASHSHOP_ITEM_DESC_MAX				175

#define		CASHSHOP_TEX						1.0f
#define		CASHSHOP_RENDER_MYCASH_X			(CASHSHOP_RENDER_START_X + 525)		// ��ĳ�� ������ ������
#define		CASHSHOP_RENDER_MYCASH_Y			(CASHSHOP_RENDER_BUY_BUTTON_Y+4)		// ��ĳ�� ������ ������(�⺻ 12DIGIT)
#endif


#ifdef LANGUAGE_VIETNAM
#define		STRING_CULL ::StringCullingUserData_ToBlank
#define		CASHSHOP_ITEM_DESC_MAX				175

#define		CASHSHOP_TEX						1.0f
#define		CASHSHOP_RENDER_MYCASH_X			(CASHSHOP_RENDER_START_X + 545)//525)		// ��ĳ�� ������ ������
#define		CASHSHOP_RENDER_MYCASH_Y			(CASHSHOP_RENDER_BUY_BUTTON_Y+4)		// ��ĳ�� ������ ������(�⺻ 12DIGIT)
#endif
// end 2008-12-04 by bhsohn Japan Working

#endif // C_EPSODE4_UI_CHANGE_JSKIM
// eend 2011. 11. 17 by jskim EP4 UI ����
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCityCashShop::CINFCityCashShop(CAtumNode* pParent, BUILDINGNPC* pBuilding)
{
	FLOG("CINFCityCashShop(CAtumNode* pParent, BUILDINGNPC* pBuilding)");
	m_pParent = pParent;	// CGameMain*
	m_pBuildingInfo = pBuilding;

	m_pImgBack = NULL;
	m_pImgSelectItem = NULL;
	int i;
	for (i = 0; i < CASH_SHOP_VIEW_TAB; i++)
		m_pImgTab[i] = NULL;
	for (i = 0; i < CASH_SHOP_BUTTON_STATE_ALL; i++)
	{
		m_pImgChargeButton[i] = NULL;
		m_pImgBuyButton[i] = NULL;
	}

	for (i = 0; i < CASH_SHOP_ITEM_FONT; i++)
	{
		m_pFontItem[i] = NULL;
	}
	for (i = 0; i < CASH_SHOP_ITEM_INFO; i++)
	{
		m_pFontItemInfo[i] = NULL;
	}
	m_bInputItemDone = FALSE;
	m_nCurrentSelectTab = 0;
	m_nCurrentMouseStateCharge = CASHSHOP_BUTTON_STATE_NOM;
	m_bCurrentMouseStateCharge = FALSE;
	m_nCurrentMouseStateBuy = CASHSHOP_BUTTON_STATE_NOM;
	m_nGiftButtonState = BUTTON_STATE_NORMAL;
	m_bCurrentMouseStateBuy = FALSE;
	m_nItemSelect = 0;
	for (i = 0; i < MAX_CASH_SHOP_TAB; i++)
		m_vecImgCashIcon[i].clear();
	for (i = 0; i < INVEN_ROTATION_STATE; i++)
	{
		for (int j = 0; j < INVEN_ROTATION_NUM; j++)
		{
			m_pDirection[i][j] = NULL;
		}
	}
	m_pRotationBase = NULL;

	m_pScroll = NULL;
	m_nMyCashPoint = 0;
	m_nGiftCardPoint = 0;

	m_bItemBuySend = FALSE;
	m_bMoveShop = FALSE;
	m_nShopOldPosX = 0;
	m_nShopOldPosY = 0;
	m_nShopMovePosX = 0;
	m_nShopMovePosY = 0;
	for (i = 0; i < 4; i++)
		m_pImgMoreItem[i] = NULL;
	m_fTimeMoreIcon = CASHSHOP_TIME_MORE_ITEM;
	m_bMoreItemRenderFlag = TRUE;
	m_pImgGiftButton = NULL;
	m_nGiveTargetCharacterUID = 0;
	m_nButtonState = ROTATION_STATE_N;
	m_nRotationState = ROTATION_NONE;
	// 2008-10-28 by bhsohn ĳ�� ���� ���� ����
	for (i = 0; i < CASH_SHOP_VIEW_TAB; i++)
	{
		m_nShopTapEndPosX[i] = -1;
	}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	for (i = 0; i < CASH_SHOP_VIEW_TAB; i++)
	{
		m_nShopTapEndPosX[i] = NULL;
	}

	m_pCloseBtn = NULL;
#else
	int nPosX = 91;
	for (i = 0; i < CASH_SHOP_VIEW_TAB; i++)
	{
		m_nShopTapEndPosX[i] = nPosX;
		nPosX += 75;
	}
#endif
	for (i = 0; i < CASH_SHOP_VIEW_TAB; i++)
	{
		m_nTabToItemIdx[i] = -1;
	}
	m_nSkinItemIdx = -2;

	for (i = 0; i < MAX_NEW_CASHSHOP_IMAGE; i++)
	{
		m_pImgNewTab[i] = NULL;
	}
	m_nNewPlayIndex = 0;
	m_fNewPlayTime = 1.0f;
	// end 2009-01-28 by bhsohn ĳ���� ó��

	// 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
	m_pSoldOut_Item = NULL;
	m_pSoldOut_ToolTip = NULL;
	for (i = 0; i < 2; ++i)
		m_pLimitedEdtion[i] = NULL;

	m_bBling = TRUE;
	m_nBlingStartTime = 0;
	// end 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
}

CINFCityCashShop::~CINFCityCashShop()
{
	FLOG("~CINFCityCashShop()");

	util::del(m_pImgBack);
	util::del(m_pImgSelectItem);
	for (auto i = 0; i < CASH_SHOP_VIEW_TAB; i++)
		util::del(m_pImgTab[i]);
	for (auto i = 0; i < CASH_SHOP_BUTTON_STATE_ALL; i++)
	{
		util::del(m_pImgChargeButton[i]);
		util::del(m_pImgBuyButton[i]);
	}

	for (auto i = 0; i < CASH_SHOP_ITEM_FONT; i++)
	{
		util::del(m_pFontItem[i]);
	}
	for (auto i = 0; i < CASH_SHOP_ITEM_INFO; i++)
	{
		util::del(m_pFontItemInfo[i]);
	}

	// 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
#ifndef C_CASHSHOP_IMAGE_REMOVE
	util::del(m_pImgGiftButton);
#endif
	// end 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
	// end 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
		// end 2009-02-24 by bhsohn ĳ���� ���� �������� �߰�

	for (auto i = 0; i < MAX_CASH_SHOP_TAB; i++)
	{
		vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[i].begin();
		while (it != m_vecItemInfo[i].end())
		{
			SHOP_ITEM* pItem = *it;
			delete pItem;
			pItem = NULL;
			it++;
		}
		m_vecItemInfo[i].clear();


		for (auto pImg : m_vecImgCashIcon[i])
		{
			// 2007-08-21 by bhsohn ĳ�� ������ alt+tab���� ����
			util::del(pImg.pDataHeader);
			util::del(pImg.vecImgCashIcon);
		}

		m_vecImgCashIcon[i].clear();
		util::del(m_pScroll);
	}
	for (auto i = 0; i < INVEN_ROTATION_STATE; i++)
	{
		for (auto j = 0; j < INVEN_ROTATION_NUM; j++) util::del(m_pDirection[i][j]);
	}
	util::del(m_pRotationBase);

	for (auto i = 0; i < 4; i++) util::del(m_pImgMoreItem[i]);

	// 2009-01-28 by bhsohn ĳ���� ó��
	for (auto i = 0; i < MAX_NEW_CASHSHOP_IMAGE; i++)
	{
		util::del(m_pImgNewTab[i]);
	}
	// end 2009-01-28 by bhsohn ĳ���� ó��

	// 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
	util::del(m_pSoldOut_Item);
	util::del(m_pSoldOut_ToolTip);
	for (auto i = 0; i < 2; ++i) util::del(m_pLimitedEdtion[i]);
	// end 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
}

void CINFCityCashShop::InitData()
{
	m_bInputItemDone = TRUE;
	// 2007-11-15 by dgwoo ĳ�����?ó�� ������ ��� ����� ���̱⶧���� ��Ȱ��ȭ �����ش�.
// 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
#ifdef C_CASHSHOP_IMAGE_REMOVE
	//
#else
	if (m_pImgGiftButton)
	{
		m_pImgGiftButton->EnableBtn(FALSE);
	}
#endif
	// end 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����

	for (int i = 0; i < MAX_CASH_SHOP_TAB; i++)
	{
		vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[i].begin();
		while (it != m_vecItemInfo[i].end())
		{
			SHOP_ITEM* pItem = *it;
			delete pItem;
			pItem = NULL;
			it++;
		}
		m_vecItemInfo[i].clear();

		for (auto img : m_vecImgCashIcon[i])
		{
			// 2007-08-21 by bhsohn ĳ�� ������ alt+tab���� ����
			util::del(img.pDataHeader);
			util::del(img.vecImgCashIcon);
		}
		m_vecImgCashIcon[i].clear();
	}

	// 2009-03-02 by bhsohn ĳ���� ���� ����
	g_pGameMain->m_bBuyMessage = FALSE;
	// end 2009-03-02 by bhsohn ĳ���� ���� ����
}

void CINFCityCashShop::InputItemDone()
{
	//	CashItemSort();								// 2006-10-20 by ispark, ���� ���
	//	m_bInputItemDone = TRUE;					// 2007-10-05 by dgwoo �ƸӰ� �ٲ𶧸��� ����Ʈ�� ���û�Ѵ�.
	g_pD3dApp->m_bRequestEnable = TRUE;			// 2006-07-07 by ispark, �޼��� ���� �Ϸ�
}
void CINFCityCashShop::ClearItemList()
{

	for (int i = 0; i < MAX_CASH_SHOP_TAB; i++)
	{
		m_vecItemInfo[i].clear();
		m_vecImgCashIcon[i].clear();
	}


}

int CINFCityCashShop::InputItem(char* pMsg)
{
	FLOG("CINFCityCashShop::InputItem(char* pMsg)");
	if (pMsg)
	{
		ITEM *item = g_pDatabase->GetServerItemInfo(((SHOP_ITEM*)pMsg)->ItemNum);

		// 2009-01-28 by bhsohn ĳ���� ó��
		SHOP_ITEM* pShopItem = ((SHOP_ITEM*)pMsg);
		BYTE byCashIndex = GET_TABINDEX_BY_CASHSHOPINDEX(pShopItem->CashShopIndex);
		BYTE byReNewIndex = GET_CASHSHOP_BIT_BY_CASHSHOPINDEX(pShopItem->CashShopIndex);

		// �Ϲ� ��
		if (item
			&& (byCashIndex >= 0)
			&& (byCashIndex < (MAX_CASH_SHOP_TAB - 1)))
		{
			SHOP_ITEM* pItem = new SHOP_ITEM;

			memcpy(pItem, pMsg, sizeof(SHOP_ITEM));
			if (FindCashShopData(byCashIndex, pItem->ItemNum) == NULL)
			{
				CashItemIcon pTemp;
				m_vecItemInfo[byCashIndex].push_back(pItem);

				pTemp.nItemNum = pItem->ItemNum;
				pTemp.pDataHeader = NULL;
				pTemp.vecImgCashIcon = FindCashIcon(item->SourceIndex, pTemp.pDataHeader);

				m_vecImgCashIcon[byCashIndex].push_back(pTemp);
			}
			else
			{
				// ��� �ſ��� �߰����� ���� �������̴�.
				util::del(pItem);
			}
		}

		// ��õ ��
		if (item && (IS_RECOMMEND_CASHSHOPITEM_BY_CASHSHOPINDEX(byReNewIndex)))
		{
			// 0 ���� ��õ��CASH_ITEMKIND_RECOMMEND_ITEM
			SHOP_ITEM* pRecommandItem = new SHOP_ITEM;

			memcpy(pRecommandItem, pMsg, sizeof(SHOP_ITEM));
			if (FindCashShopData(CASH_ITEMKIND_RECOMMEND_ITEM, pRecommandItem->ItemNum) == NULL)
			{
				CashItemIcon pTemp;
				m_vecItemInfo[CASH_ITEMKIND_RECOMMEND_ITEM].push_back(pRecommandItem);

				pTemp.nItemNum = pRecommandItem->ItemNum;
				pTemp.pDataHeader = NULL;
				pTemp.vecImgCashIcon = FindCashIcon(item->SourceIndex, pTemp.pDataHeader);

				m_vecImgCashIcon[CASH_ITEMKIND_RECOMMEND_ITEM].push_back(pTemp);
			}
			else
			{
				// ��� �ſ��� �߰����� ���� �������̴�.
				util::del(pRecommandItem);
			}
		}
		//		if(item)
		// 		{
		// 			// 2006-01-11 by ispark, ���� ������ �ִ��� Ȯ��
		// 			// ���� ���ؼ� �ߺ� �Է��� �Ǵ� ������ �ִ�. �׷��� ���� ������ ������ �Ѿ
		// 
		// 			SHOP_ITEM* pItem = new SHOP_ITEM;
		// 			CashItemIcon pTemp;
		// 
		// // 2006-06-08 by ispark, Sharging���� SummonMonster�� ����
		// //			if(item->Charging == CASH_ITEMKIND_PREMIUM_CARD)
		// 			if(item->SummonMonster == CASH_ITEMKIND_PREMIUM_CARD)
		// 			{
		// 				memcpy(pItem, pMsg, sizeof(SHOP_ITEM));
		// 				if(FindCashShopData(CASHSHOP_ITEM_PREMIUM_CARD, pItem) == NULL)
		// 				{
		// 					m_vecItemInfo[CASHSHOP_ITEM_PREMIUM_CARD].push_back(pItem);
		// 				
		// 					pTemp.nItemNum = pItem->ItemNum;
		// //					pTemp.vecImgCashIcon = g_pGameMain->m_pItemInfo->FindBigIcon(pItem->ItemNum);	// 2005-08-23 by ispark
		// 
		// 					// 2007-08-21 by bhsohn ĳ�� ������ alt+tab���� ����
		// 					//pTemp.vecImgCashIcon = g_pGameMain->m_pItemInfo->FindBigIcon(item->SourceIndex);
		// 					pTemp.pDataHeader = NULL;
		// 					pTemp.vecImgCashIcon = FindCashIcon(item->SourceIndex, pTemp.pDataHeader);					
		// 					
		// 					m_vecImgCashIcon[CASHSHOP_ITEM_PREMIUM_CARD].push_back(pTemp);
		// 				}
		// 			}
		// 			else if(item->SummonMonster == CASH_ITEMKIND_OPTION_ITEM)
		// 			{
		// 				memcpy(pItem, pMsg, sizeof(SHOP_ITEM));
		// 				if(FindCashShopData(CASHSHOP_ITEM_OPTION_ITEM, pItem) == NULL)
		// 				{
		// 					m_vecItemInfo[CASHSHOP_ITEM_OPTION_ITEM].push_back(pItem);
		// 
		// 					pTemp.nItemNum = pItem->ItemNum;
		// //					pTemp.vecImgCashIcon = g_pGameMain->m_pItemInfo->FindBigIcon(pItem->ItemNum);	// 2005-08-23 by ispark
		// 
		// 					// 2007-08-21 by bhsohn ĳ�� ������ alt+tab���� ����
		// 					//pTemp.vecImgCashIcon = g_pGameMain->m_pItemInfo->FindBigIcon(item->SourceIndex);
		// 					pTemp.pDataHeader = NULL;
		// 					pTemp.vecImgCashIcon = FindCashIcon(item->SourceIndex, pTemp.pDataHeader);
		// 
		// 					m_vecImgCashIcon[CASHSHOP_ITEM_OPTION_ITEM].push_back(pTemp);
		// 				}
		// 			}
		// 			else if(item->SummonMonster == CASH_ITEMKIND_CARD_ITEM)
		// 			{
		// 				//CASH_ITEMKIND_WASTEFUL_ITEM
		// 				memcpy(pItem, pMsg, sizeof(SHOP_ITEM));
		// 				if(FindCashShopData(CASHSHOP_ITEM_CARD_ITEM, pItem) == NULL)
		// 				{
		// 					m_vecItemInfo[CASHSHOP_ITEM_CARD_ITEM].push_back(pItem);
		// 
		// 					pTemp.nItemNum = pItem->ItemNum;
		// //						pTemp.vecImgCashIcon = g_pGameMain->m_pItemInfo->FindBigIcon(pItem->ItemNum);	// 2005-08-23 by ispark
		// 
		// 					// 2007-08-21 by bhsohn ĳ�� ������ alt+tab���� ����
		// 					//pTemp.vecImgCashIcon = g_pGameMain->m_pItemInfo->FindBigIcon(item->SourceIndex);
		// 					pTemp.pDataHeader = NULL;
		// 					pTemp.vecImgCashIcon = FindCashIcon(item->SourceIndex, pTemp.pDataHeader);
		// 
		// 					m_vecImgCashIcon[CASHSHOP_ITEM_CARD_ITEM].push_back(pTemp);
		// 				}
		// 			}
		// 			else if(item->SummonMonster == CASH_ITEMKIND_ACCESSORY_ITEM)
		// 			{
		// 				memcpy(pItem, pMsg, sizeof(SHOP_ITEM));
		// 				if(FindCashShopData(CASHSHOP_ITEM_ACCESSORY_ITEM, pItem) == NULL)
		// 				{
		// 					m_vecItemInfo[CASHSHOP_ITEM_ACCESSORY_ITEM].push_back(pItem);
		// 
		// 					pTemp.nItemNum = pItem->ItemNum;
		// //					pTemp.vecImgCashIcon = g_pGameMain->m_pItemInfo->FindBigIcon(pItem->ItemNum);	// 2005-08-23 by ispark
		// 
		// 					// 2007-08-21 by bhsohn ĳ�� ������ alt+tab���� ����
		// 					//pTemp.vecImgCashIcon = g_pGameMain->m_pItemInfo->FindBigIcon(item->SourceIndex);
		// 					pTemp.pDataHeader = NULL;
		// 					pTemp.vecImgCashIcon = FindCashIcon(item->SourceIndex, pTemp.pDataHeader);
		// 
		// 					m_vecImgCashIcon[CASHSHOP_ITEM_ACCESSORY_ITEM].push_back(pTemp);
		// 				}
		// 			}
		// 			// 2008-10-20 by dgwoo ��Ű�� �� �߰�
		// 			else if(item->SummonMonster == CASH_ITEMKIND_PACKAGE_ITEM)
		// 			{
		// 				memcpy(pItem,pMsg,sizeof(SHOP_ITEM));
		// 				if(FindCashShopData(CASHSHOP_ITEM_PACKAGE, pItem) == NULL)
		// 				{
		// 					m_vecItemInfo[CASHSHOP_ITEM_PACKAGE].push_back(pItem);
		// 					pTemp.nItemNum = pItem->ItemNum;
		// 					// 2008-10-28 by bhsohn ĳ�� ���� ���� ����
		// 					// �ʱ�ȭ ���ϸ� ������ ����.
		// 					pTemp.pDataHeader = NULL;	
		// 					pTemp.vecImgCashIcon = g_pGameMain->m_pItemInfo->FindBigIcon(item->SourceIndex);
		// 					m_vecImgCashIcon[CASHSHOP_ITEM_PACKAGE].push_back(pTemp);
		// 				}
		// 			}
		// 			// 2008-10-20 by dgwoo ��Ű�� �� �߰�
		// 		}
				// end 2009-01-28 by bhsohn ĳ���� ó��
	}

	// 2009-01-28 by bhsohn ĳ���� ó��
	//m_pScroll->SetNumberOfData( m_vecImgCashIcon[CASH_ITEMKIND_PREMIUM_CARD].size() );	
	// end 2009-01-28 by bhsohn ĳ���� ó��

	return sizeof(SHOP_ITEM);
}

HRESULT CINFCityCashShop::InitDeviceObjects()
{
	FLOG("CINFCityCashShop::InitDeviceObjects(char* pData, int nSize)");
	int i;
	DataHeader	* pDataHeader;

	// 2011. 11. 17 by jskim EP4 UI ����
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("cash_sh");
	m_pImgBack = g_pGameMain->m_GruopImagemanager->GetGroupImage(pDataHeader);
	m_pImgBack->InitDeviceObjects(g_pD3dApp->m_pImageList);
	m_pImgBack->RestoreDeviceObjects();

	m_pCloseBtn = new CINFImageBtn;
	m_pCloseBtn->InitDeviceObjects("close", "close", "close", "close");
#else

	wsprintf(buf, "ca_bas");
	m_pImgBack = new CINFImageEx;
	pDataHeader = FindResource(buf);
	m_pImgBack->InitDeviceObjects(pDataHeader);
#endif
	// end 2011. 11. 17 by jskim EP4 UI ����

	m_pImgSelectItem = new CINFImageEx;
	pDataHeader = FindResource("casel");
	m_pImgSelectItem->InitDeviceObjects(pDataHeader);

	for (i = 0; i < CASH_SHOP_VIEW_TAB; i++)
	{
		// 2009-01-12 by bhsohn �ű� ĳ���� �۾�
		if (m_nShopTapEndPosX[i] < 0)
		{
			continue;
		}
		// end 2009-01-12 by bhsohn �ű� ĳ���� �۾�
		
		char buf[32] { };

		sprintf_s(buf, "ca1_ad%d", i);
		// end 2009. 06. 08 by ckPark ĳ���� ���� UI(��Ű�ӽ�)�� ����
		pDataHeader = FindResource(buf);
		if (pDataHeader)
		{
			m_pImgTab[i] = new CINFImageEx;
			m_pImgTab[i]->InitDeviceObjects(pDataHeader);
		}
	}
	// 2009-01-28 by bhsohn ĳ���� ó��
	// end 2009-01-28 by bhsohn ĳ���� ó��
	for (i = 0; i < INVEN_ROTATION_STATE; i++)
	{
		for (int j = 0; j < INVEN_ROTATION_NUM; j++)
		{
			char buf[32] { };

			sprintf_s(buf, "inven%d%d", i, j);

			m_pDirection[i][j] = new CINFImageEx;
			pDataHeader = FindResource(buf);
			m_pDirection[i][j]->InitDeviceObjects(pDataHeader);
		}
	}

	m_pRotationBase = new CINFImageEx;
	pDataHeader = FindResource("innom");
	m_pRotationBase->InitDeviceObjects(pDataHeader);


	for (i = 0; i < CASH_SHOP_BUTTON_STATE_ALL; i++)
	{
		char buf[32] { };

		sprintf_s(buf, "canob%d", i);
		m_pImgChargeButton[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pImgChargeButton[i]->InitDeviceObjects(pDataHeader);

		sprintf_s(buf, "cachb%d", i);
		m_pImgBuyButton[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pImgBuyButton[i]->InitDeviceObjects(pDataHeader);
	}

#if defined(NEW_CASHSHOP_INTERFACE_HSSON) && !defined(C_CASHSHOP_IMAGE_REMOVE)
	
	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();

	if (!COMPARE_RACE(myShuttleInfo.Race, RACE_GAMEMASTER | RACE_MONITOR | RACE_GUEST | RACE_DEMO))
	{
		if (!m_pImgGiftButton) m_pImgGiftButton = new CINFImageBtn;
		

		if (m_pImgGiftButton->InitDeviceObjects("cagift", "STRTOOLTIP73") == S_FALSE)
		{
			m_pImgGiftButton->DeleteDeviceObjects();
			delete m_pImgGiftButton;
			m_pImgGiftButton = nullptr;
		}
	}

#endif

	for (i = 0; i < 4; i++)
	{
		char buf[32] { };
		sprintf_s(buf, "more_s%d", i);
		m_pImgMoreItem[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pImgMoreItem[i]->InitDeviceObjects(pDataHeader);
	}

	for (i = 0; i < CASH_SHOP_ITEM_FONT; i++)
	{
		// 2007-10-16 by bhsohn �ɽ��� ������ �̸��� ����� ��.
		//m_pFontItem[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,256,32);		
		if (0 == i % 3)
		{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM 
			m_pFontItem[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE | D3DFONT_BOLD, TRUE, 512, 32);		// 2008-10-22 by dgwoo 256 ->512 �� ����
#else
			m_pFontItem[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 10, D3DFONT_ZENABLE | D3DFONT_BOLD, TRUE, 512, 32);		// 2008-10-22 by dgwoo 256 ->512 �� ����
#endif
		}
		else
		{
			m_pFontItem[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, TRUE, 512, 32);		// 2008-10-22 by dgwoo 256 ->512 �� ����
		}
		m_pFontItem[i]->InitDeviceObjects(g_pD3dDev);
	}
	for (i = 0; i < CASH_SHOP_ITEM_INFO; i++)
	{
		// 2007-10-16 by bhsohn �ɽ��� ������ �̸��� ����� ��.
		//m_pFontItemInfo[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,256,32);
		if (0 == i)
		{
			m_pFontItemInfo[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE | D3DFONT_BOLD, TRUE, 512, 32);		// 2008-10-22 by dgwoo 256 ->512 �� ����
		}
		else
		{
			m_pFontItemInfo[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, TRUE, 512, 32);		// 2008-10-22 by dgwoo 256 ->512 �� ����
		}

		m_pFontItemInfo[i]->InitDeviceObjects(g_pD3dDev);
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	m_pScroll = new CINFScrollBar(this,
		CASHSHOP_SCROLL_START_X,
		CASHSHOP_SCROLL_START_Y,
		CASHSHOP_SCROLL_LINE_LENGTH,
		CASHSHOP_SHOW_ICON_MAX,
		SCROLL_LINE_X_SIZE,
		SCROLL_LINE_Y_SIZE);
#else
	m_pScroll = new CINFScrollBar(this,
		CASHSHOP_SCROLL_START_X,
		CASHSHOP_SCROLL_START_Y,
		CASHSHOP_SCROLL_LINE_LENGTH,
		CASHSHOP_SHOW_ICON_MAX);
#endif	
	m_pScroll->SetGameData(m_pGameData);
	m_pScroll->InitDeviceObjects();
	m_pScroll->SetWheelRect(CASHSHOP_SCROLL_RECT_START_X,
		CASHSHOP_SCROLL_RECT_START_Y,
		CASHSHOP_SCROLL_RECT_W,
		CASHSHOP_SCROLL_RECT_H);

	// 2009-01-28 by bhsohn ĳ���� ó��
	for (i = 0; i < MAX_NEW_CASHSHOP_IMAGE; i++)
	{
		char buf[32] { };
		sprintf_s(buf, "ca_n%d", i);
		pDataHeader = FindResource(buf);

		if (pDataHeader)
		{
			m_pImgNewTab[i] = new CINFImageEx;
			m_pImgNewTab[i]->InitDeviceObjects(pDataHeader);
		}
	}
	// end 2009-01-28 by bhsohn ĳ���� ó��

	// 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
	m_pSoldOut_Item = new CINFImageEx;
	pDataHeader = FindResource("soldouts");
	m_pSoldOut_Item->InitDeviceObjects(pDataHeader);

	m_pSoldOut_ToolTip = new CINFImageEx;
	pDataHeader = FindResource("soldoutl");
	m_pSoldOut_ToolTip->InitDeviceObjects(pDataHeader);

	for (i = 0; i < 2; ++i)
	{
		m_pLimitedEdtion[i] = new CINFImageEx;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		pDataHeader = FindResource("limited0");
#else
		char buf[32] { };
		sprintf_s(buf, "limited%d", i)
		pDataHeader = FindResource(buf);
#endif		

		m_pLimitedEdtion[i]->InitDeviceObjects(pDataHeader);
	}
	// end 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���

	return S_OK;
}

HRESULT CINFCityCashShop::RestoreDeviceObjects()
{
	FLOG("CINFCityCashShop::RestoreDeviceObjects()");
	int i;
	m_pImgBack->RestoreDeviceObjects();
	m_pImgSelectItem->RestoreDeviceObjects();
	for (i = 0; i < CASH_SHOP_VIEW_TAB; i++)
	{
		if (m_pImgTab[i])
		{
			m_pImgTab[i]->RestoreDeviceObjects();
		}
	}
	for (i = 0; i < INVEN_ROTATION_STATE; i++)
	{
		for (int j = 0; j < INVEN_ROTATION_NUM; j++)
		{
			m_pDirection[i][j]->RestoreDeviceObjects();
		}
	}
	m_pRotationBase->RestoreDeviceObjects();

	for (i = 0; i < CASH_SHOP_BUTTON_STATE_ALL; i++)
	{
		m_pImgChargeButton[i]->RestoreDeviceObjects();
		m_pImgBuyButton[i]->RestoreDeviceObjects();
	}
	// 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
#ifdef C_CASHSHOP_IMAGE_REMOVE
	//
#else
	if (m_pImgGiftButton)
	{
		m_pImgGiftButton->RestoreDeviceObjects();
		m_pImgGiftButton->SetBtnPosition(CASHSHOP_RENDER_GIFT_BUTTON_X + m_nShopMovePosX, CASHSHOP_RENDER_GIFT_BUTTON_Y + m_nShopMovePosY);
	}
#endif
	// end 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
	for (i = 0; i < 4; i++)
	{
		m_pImgMoreItem[i]->RestoreDeviceObjects();
	}

	for (i = 0; i < CASH_SHOP_ITEM_FONT; i++)
	{
		m_pFontItem[i]->RestoreDeviceObjects();
	}
	for (i = 0; i < CASH_SHOP_ITEM_INFO; i++)
	{
		m_pFontItemInfo[i]->RestoreDeviceObjects();
	}
	for (i = 0; i < MAX_CASH_SHOP_TAB; i++)
	{
		vector<CashItemIcon>::iterator itImg = m_vecImgCashIcon[i].begin();
		while (itImg != m_vecImgCashIcon[i].end())
		{
			if (itImg->nItemNum > 0)
			{
				// 2007-08-21 by bhsohn ĳ�� ������ alt+tab���� ����
				//itImg->vecImgCashIcon = g_pGameMain->m_pItemInfo->FindBigIcon(itImg->nItemNum);				
				if (itImg->vecImgCashIcon)
				{
					itImg->vecImgCashIcon->RestoreDeviceObjects();
				}
				// end 2007-08-21 by bhsohn ĳ�� ������ alt+tab���� ����

				//itImg->vecImgCashIcon->RestoreDeviceObjects();
			}
			itImg++;
		}
	}
	m_pScroll->RestoreDeviceObjects();
	// 2009-01-28 by bhsohn ĳ���� ó��
	for (i = 0; i < MAX_NEW_CASHSHOP_IMAGE; i++)
	{
		if (m_pImgNewTab[i])
		{
			m_pImgNewTab[i]->RestoreDeviceObjects();
		}
	}
	// end 2009-01-28 by bhsohn ĳ���� ó��

	// 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
	m_pSoldOut_Item->RestoreDeviceObjects();
	m_pSoldOut_ToolTip->RestoreDeviceObjects();

	for (i = 0; i < 2; ++i)
		m_pLimitedEdtion[i]->RestoreDeviceObjects();
	// end 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	int size = (m_pImgBack->GetMaxPos().x - m_pImgBack->GetMinPos().x) / (CASH_SHOP_VIEW_TAB - 1);
	int nTemp = 0;
	for (i = 0; i < CASH_SHOP_VIEW_TAB; i++)
	{
		nTemp += size;
		m_nShopTapEndPosX[i] += nTemp;
	}
	m_pCloseBtn->RestoreDeviceObjects();
#endif
	return S_OK;
}

HRESULT CINFCityCashShop::InvalidateDeviceObjects()
{
	FLOG("CINFCityCashShop::InvalidateDeviceObjects()");

#ifdef C_EPSODE4_UI_CHANGE_JSKIM 
	m_pCloseBtn->InvalidateDeviceObjects();
#endif
	m_pImgBack->InvalidateDeviceObjects();
	m_pImgSelectItem->InvalidateDeviceObjects();
	int i;
	for (i = 0; i < CASH_SHOP_VIEW_TAB; i++)
	{
		if (m_pImgTab[i])
		{
			m_pImgTab[i]->InvalidateDeviceObjects();
		}
	}
	for (i = 0; i < INVEN_ROTATION_STATE; i++)
	{
		for (int j = 0; j < INVEN_ROTATION_NUM; j++)
		{
			m_pDirection[i][j]->InvalidateDeviceObjects();
		}
	}
	m_pRotationBase->InvalidateDeviceObjects();

	for (i = 0; i < CASH_SHOP_BUTTON_STATE_ALL; i++)
	{
		m_pImgChargeButton[i]->InvalidateDeviceObjects();
		m_pImgBuyButton[i]->InvalidateDeviceObjects();
	}
	// 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
#ifdef C_CASHSHOP_IMAGE_REMOVE
	//
#else
	if (m_pImgGiftButton)
	{
		m_pImgGiftButton->InvalidateDeviceObjects();
	}
#endif

	// end 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
		// end 2009-02-24 by bhsohn ĳ���� ���� �������� �߰�		
	for (i = 0; i < 4; i++)
	{
		m_pImgMoreItem[i]->InvalidateDeviceObjects();
	}

	for (i = 0; i < CASH_SHOP_ITEM_FONT; i++)
	{
		m_pFontItem[i]->InvalidateDeviceObjects();
	}
	for (i = 0; i < CASH_SHOP_ITEM_INFO; i++)
	{
		m_pFontItemInfo[i]->InvalidateDeviceObjects();
	}
	for (i = 0; i < MAX_CASH_SHOP_TAB; i++)
	{
		vector<CashItemIcon>::iterator itImg = m_vecImgCashIcon[i].begin();
		while (itImg != m_vecImgCashIcon[i].end())
		{
			if (itImg->nItemNum > 0)
			{
				if (itImg->vecImgCashIcon != NULL)
					itImg->vecImgCashIcon->InvalidateDeviceObjects();
			}
			itImg++;
		}
	}
	m_pScroll->InvalidateDeviceObjects();
	// 2009-01-28 by bhsohn ĳ���� ó��
	for (i = 0; i < MAX_NEW_CASHSHOP_IMAGE; i++)
	{
		if (m_pImgNewTab[i])
		{
			m_pImgNewTab[i]->InvalidateDeviceObjects();
		}
	}
	// end 2009-01-28 by bhsohn ĳ���� ó��

	// 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
	m_pSoldOut_Item->InvalidateDeviceObjects();
	m_pSoldOut_ToolTip->InvalidateDeviceObjects();

	for (i = 0; i < 2; ++i)
		m_pLimitedEdtion[i]->InvalidateDeviceObjects();
	// end 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���

	return S_OK;
}

HRESULT CINFCityCashShop::DeleteDeviceObjects()
{
	FLOG("CINFCityCashShop::DeleteDeviceObjects()");


	///////////////////////////////////////////////////////////////////
#ifdef C_EPSODE4_UI_CHANGE_JSKIM 
	m_pCloseBtn->DeleteDeviceObjects();
	util::del(m_pCloseBtn);
#endif
	m_pImgBack->DeleteDeviceObjects();
	m_pImgSelectItem->DeleteDeviceObjects();
	int i;
	for (i = 0; i < CASH_SHOP_VIEW_TAB; i++)
	{
		if (m_pImgTab[i])
		{
			m_pImgTab[i]->DeleteDeviceObjects();
		}
	}
	for (i = 0; i < INVEN_ROTATION_STATE; i++)
	{
		for (int j = 0; j < INVEN_ROTATION_NUM; j++)
		{
			m_pDirection[i][j]->DeleteDeviceObjects();
			util::del(m_pDirection[i][j]);
		}
	}
	m_pRotationBase->DeleteDeviceObjects();
	util::del(m_pRotationBase);

	for (i = 0; i < CASH_SHOP_BUTTON_STATE_ALL; i++)
	{
		m_pImgChargeButton[i]->DeleteDeviceObjects();
		m_pImgBuyButton[i]->DeleteDeviceObjects();
	}
	// 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
#ifdef C_CASHSHOP_IMAGE_REMOVE
	//
#else
	if (m_pImgGiftButton)
	{
		m_pImgGiftButton->DeleteDeviceObjects();
	}
#endif
	// end 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
		// end 2009-02-24 by bhsohn ĳ���� ���� �������� �߰�		
	for (i = 0; i < CASH_SHOP_ITEM_FONT; i++)
	{
		m_pFontItem[i]->DeleteDeviceObjects();
	}
	for (i = 0; i < CASH_SHOP_ITEM_INFO; i++)
	{
		m_pFontItemInfo[i]->DeleteDeviceObjects();
	}
	for (i = 0; i < MAX_CASH_SHOP_TAB; i++)
	{

		for (auto img : m_vecImgCashIcon[i])
		{
			// 2007-08-21 by bhsohn ĳ�� ������ alt+tab���� ����
			util::del(img.pDataHeader);

			if (img.vecImgCashIcon != nullptr)
			{
				img.vecImgCashIcon->DeleteDeviceObjects();
				util::del(img.vecImgCashIcon);
			}
		}
		m_vecImgCashIcon[i].clear();
	}

	m_pScroll->DeleteDeviceObjects();

	////////////////////////////////////////////////////////////////////
	util::del(m_pImgBack);
	util::del(m_pImgSelectItem);
	for (i = 0; i < CASH_SHOP_VIEW_TAB; i++)
		util::del(m_pImgTab[i]);
	for (i = 0; i < CASH_SHOP_BUTTON_STATE_ALL; i++)
	{
		util::del(m_pImgChargeButton[i]);
		util::del(m_pImgBuyButton[i]);
	}

	for (i = 0; i < CASH_SHOP_ITEM_FONT; i++)
	{
		util::del(m_pFontItem[i]);
	}
	for (i = 0; i < CASH_SHOP_ITEM_INFO; i++)
	{
		util::del(m_pFontItemInfo[i]);
	}
	for (i = 0; i < 3; i++)
	{
		vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[i].begin();
		while (it != m_vecItemInfo[i].end())
		{
			SHOP_ITEM* pItem = *it;
			delete pItem;
			pItem = NULL;
			it++;
		}
		m_vecItemInfo[i].clear();
	}
	util::del(m_pScroll);
	for (i = 0; i < 4; i++)
	{
		m_pImgMoreItem[i]->DeleteDeviceObjects();
		util::del(m_pImgMoreItem[i]);
	}
	// 2009-01-28 by bhsohn ĳ���� ó��
	for (i = 0; i < MAX_NEW_CASHSHOP_IMAGE; i++)
	{
		if (m_pImgNewTab[i])
		{
			m_pImgNewTab[i]->DeleteDeviceObjects();
			util::del(m_pImgNewTab[i]);
		}
	}
	// end 2009-01-28 by bhsohn ĳ���� ó��

	// 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
	m_pSoldOut_Item->DeleteDeviceObjects();
	util::del(m_pSoldOut_Item);
	m_pSoldOut_ToolTip->DeleteDeviceObjects();
	util::del(m_pSoldOut_ToolTip);

	for (i = 0; i < 2; ++i)
	{
		m_pLimitedEdtion[i]->DeleteDeviceObjects();
		util::del(m_pLimitedEdtion[i]);
	}
	// end 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���




	return S_OK;
}

void CINFCityCashShop::Render()
{
	FLOG("CINFCityCashShop::Render()");
	// ���ÿ� �ʵ忡�� ������ ��ġ ����
	if (m_fTimeMoreIcon > 0)
	{
		m_fTimeMoreIcon -= g_pD3dApp->GetElapsedTime();
	}
	if (m_fTimeMoreIcon < 0)
	{
		m_fTimeMoreIcon = CASHSHOP_TIME_MORE_ITEM;
		m_bMoreItemRenderFlag = !m_bMoreItemRenderFlag;
	}

	m_pItem = NULL;
	char buf[512];
	memset(buf, 0x00, 512);
	char buf2[512];
	memset(buf2, 0x00, 512);
	if (g_pD3dApp->m_dwGameState == _CITY)
	{
		// ���û󿡼� ������ ��ġ
		//nShopGap = CASHSHOP_RENDER_CITY_GAB;
	}
	else
	{
		// �ʵ�󿡼� ������ ��ġ
		//nShopGap = CASHSHOP_RENDER_GAME_GAB;	
	}
	//m_nCityRenderGab = nShopGap;
	m_pScroll->SetScrollLinePos(CASHSHOP_SCROLL_START_X + m_nShopMovePosX,
		CASHSHOP_SCROLL_START_Y + m_nShopMovePosY);
	m_pScroll->SetWheelRect(CASHSHOP_SCROLL_RECT_START_X + m_nShopMovePosX,
		CASHSHOP_SCROLL_RECT_START_Y + m_nShopMovePosY,
		CASHSHOP_SCROLL_RECT_W,
		CASHSHOP_SCROLL_RECT_H);
	// 2009-01-28 by bhsohn ĳ���� ó��
	int nSelTab = GetTabToItemIdx(m_nCurrentSelectTab);
	if (nSelTab == -1)
	{
		m_nItemSelect = 0;
	}
	else if (nSelTab != -1 && m_vecImgCashIcon[nSelTab].size() <= (m_nItemSelect + m_pScroll->GetCurrentScrollIndex()))
	{
		m_nItemSelect = 0;
	}

	// ��� ȭ�� ������
	m_pImgBack->Move(CASHSHOP_RENDER_START_X + m_nShopMovePosX, CASHSHOP_RENDER_START_Y + m_nShopMovePosY);
	m_pImgBack->Render();
	// �Ƿ�����

#ifdef C_EPSODE4_UI_CHANGE_JSKIM 
	m_pCloseBtn->SetBtnPosition(CASHSHOP_RENDER_START_X + m_nShopMovePosX + m_pImgBack->GetMaxPos().x - m_pImgBack->GetMinPos().x - CASHSHOP_ITEM_CLOSE_X,
		CASHSHOP_RENDER_START_Y + m_nShopMovePosY + CASHSHOP_ITEM_CLOSE_Y);
	m_pCloseBtn->Render();
#endif
	// 2008-10-28 by bhsohn ĳ�� ���� ���� ����
	if (m_pImgTab[m_nCurrentSelectTab])
	{
		m_pImgTab[m_nCurrentSelectTab]->Move(CASHSHOP_RENDER_TAB_X + m_nShopMovePosX, CASHSHOP_RENDER_TAB_Y + m_nShopMovePosY);
		m_pImgTab[m_nCurrentSelectTab]->Render();
	}

	// 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
// 	// ����Ʈ �� ������
// 	m_pImgSelectItem->Move(CASHSHOP_RENDER_TAB_X+m_nShopMovePosX+9,
// 		CASHSHOP_RENDER_TAB_Y+m_nShopMovePosY+CASHSHOP_RENDER_SELECT_TAB_GAB+(64*m_nItemSelect));
// 	m_pImgSelectItem->Render();
	// end 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���

	// More ��ũ�� ������
	// 2009-01-28 by bhsohn ĳ���� ó��
	// 2011. 11. 17 by jskim EP4 UI ����
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	if (nSelTab != -1 && m_vecImgCashIcon[nSelTab].size() / CASHSHOP_TAB_W_SIZE > m_pScroll->GetCurrentScrollIndex())
	{
		int nCount = 0;
		int nItemCountNum = 1;
		int nRenderWTab = 0;
		DWORD dwColor;
		vector<CashItemIcon>::iterator itCash = m_vecImgCashIcon[nSelTab].begin();
		while (itCash != m_vecImgCashIcon[nSelTab].end())
		{
			int nTemp = nItemCountNum / CASHSHOP_TAB_W_SIZE;
			if (nItemCountNum % CASHSHOP_TAB_W_SIZE != 0)
			{
				nTemp += 1;
			}

			if (m_pScroll->GetCurrentScrollIndex() >= nTemp)
			{
				itCash++;
				nItemCountNum++;
				continue;
			}

			if (nRenderWTab >= CASHSHOP_TAB_W_SIZE)
			{
				nRenderWTab = 0;
				nCount += 1;
			}

			if (nCount >= CASHSHOP_TAB_H_SIZE)
			{
				break;
			}

			if (itCash->vecImgCashIcon != NULL)
			{
				itCash->vecImgCashIcon->Move(m_nShopMovePosX + CASHSHOP_RENDER_BIGICON_START_X + CASHSHOP_TAB_W * nRenderWTab,
					m_nShopMovePosY + CASHSHOP_RENDER_BIGICON_START_Y + CASHSHOP_TAB_H * nCount);
				itCash->vecImgCashIcon->Render();
				SHOP_ITEM* pShopInfo = FindCashShopData(nSelTab, itCash->nItemNum);
				// New �׵θ�
				if (pShopInfo && !pShopInfo->IsSoldOutShopItem())
				{
					RenderNewShopInfo(m_nShopMovePosX + CASHSHOP_RENDER_BIGICON_START_X + CASHSHOP_TAB_W * nRenderWTab - CASHSHOP_NEW_ICON_W,
						m_nShopMovePosY + CASHSHOP_RENDER_BIGICON_START_Y + CASHSHOP_TAB_H * nCount - CASHSHOP_NEW_ICON_H,
						pShopInfo);
				}
				// ������ ����
				ITEM *item = g_pDatabase->GetServerItemInfo(itCash->nItemNum);
				vector<string> vecMessage;

				StringCullingUserDataEx(item->ItemName, CASHSHOP_FONT_MAX_SIZE, &vecMessage, m_pFontItem[0], FALSE, TRUE); // 2013-04-05 by ssjung �?�϶� ������ �̸� ���� �� �Ѿ� ���� ó�� ����
				int i = 0;
				int nHeight;
				SIZE nSize;

				if (!pShopInfo->IsSoldOutShopItem())
				{
					dwColor = RGB(255, 255, 255);
				}
				else
				{
					dwColor = SOLDOUTCOLOR;
				}

				for (i = 0; i < vecMessage.size(); i++)
				{
					nSize = m_pFontItem[1]->GetStringSize((char*)vecMessage[i].c_str());

					nHeight = m_nShopMovePosY + CASHSHOP_RENDER_BIGICON_START_Y + CASHSHOP_TAB_H * nCount + itCash->vecImgCashIcon->GetImgSize().y + nSize.cy * i;
					m_pFontItem[1]->DrawText(m_nShopMovePosX + CASHSHOP_RENDER_BIGICON_START_X + CASHSHOP_TAB_W * nRenderWTab + (itCash->vecImgCashIcon->GetImgSize().x / 2) - (nSize.cx / 2),
						nHeight,
						dwColor, (char*)vecMessage[i].c_str(), 0L);
				}
				nHeight = m_nShopMovePosY + CASHSHOP_RENDER_BIGICON_START_Y + CASHSHOP_TAB_H * nCount + itCash->vecImgCashIcon->GetImgSize().y + 36;

				wsprintf(buf, STRMSG_C_SHOP_0001, item->MinTradeQuantity);//"%d��/1��"
				SIZE Cashsize = m_pFontItem[1]->GetStringSize(buf);
#ifdef SHOP_PRICES_PER_BUILDING_NPC
				wsprintf(buf, "%d", (int)(pShopInfo->Price * CASHSHOP_TEX));
#else
				wsprintf(buf, "%d", (int)(item->CashPrice * CASHSHOP_TEX));
#endif // SHOP_PRICES_PER_BUILDING_NPC


				MakeCurrencySeparator(buf2, buf, 3, ',');
				wsprintf(buf2, "%s %s", buf, STRMSG_C_SHOP_0002);
				SIZE Cashsize1 = m_pFontItem[0]->GetStringSize(buf2);

				int nCash = Cashsize.cx + Cashsize1.cx + CASHSHOP_GAB;
				int nWidth = m_nShopMovePosX + CASHSHOP_RENDER_BIGICON_START_X + CASHSHOP_TAB_W * nRenderWTab + (itCash->vecImgCashIcon->GetImgSize().x / 2) - (nCash / 2);
				wsprintf(buf, STRMSG_C_SHOP_0001, item->MinTradeQuantity);//"%d��/1��"

				if (!pShopInfo->IsSoldOutShopItem())
					dwColor = RGB(255, 255, 255);
				else
					dwColor = SOLDOUTCOLOR;

				m_pFontItem[1]->DrawText(nWidth,
					nHeight,
					dwColor, buf, 0L);

#ifdef SHOP_PRICES_PER_BUILDING_NPC
				wsprintf(buf, "%d", (int)(pShopInfo->Price * CASHSHOP_TEX));
#else
				wsprintf(buf, "%d", (int)(item->CashPrice * CASHSHOP_TEX));
#endif // SHOP_PRICES_PER_BUILDING_NPC
				MakeCurrencySeparator(buf2, buf, 3, ',');
				wsprintf(buf2, "%s %s", buf2, STRMSG_C_SHOP_0002);

				if (!pShopInfo->IsSoldOutShopItem())
					dwColor = RGB(254, 186, 1);
				else
					dwColor = SOLDOUTCOLOR;

				m_pFontItem[0]->DrawText(nWidth + Cashsize.cx + CASHSHOP_GAB, nHeight,
					dwColor, buf2, 0L);

				if (pShopInfo && pShopInfo->IsLimitedEditionShopItem())
				{
					CINFImageEx* pImg;
					if (m_bBling)
						pImg = m_pLimitedEdtion[0];


					if (pShopInfo->IsSoldOutShopItem())
						pImg = m_pLimitedEdtion[0];

					if (m_bBling && !pShopInfo->IsSoldOutShopItem())
					{
						pImg->Move(m_nShopMovePosX + CASHSHOP_RENDER_BIGICON_START_X - CASHSHOP_ITME_POS_GAB + CASHSHOP_TAB_W * nRenderWTab - 3,
							m_nShopMovePosY + CASHSHOP_RENDER_BIGICON_START_Y + CASHSHOP_TAB_H * nCount - 5);
						pImg->Render();
					}

					if (!pShopInfo->IsSoldOutShopItem())
					{
						sprintf(buf, STRMSG_C_100127_0301, pShopInfo->RemainCountForLimitedEdition);
						Cashsize = m_pFontItem[1]->GetStringSize(buf);
						m_pFontItem[1]->DrawText(CASHSHOP_RENDER_BIGICON_START_X + m_nShopMovePosX + CASHSHOP_TAB_W * nRenderWTab + (itCash->vecImgCashIcon->GetImgSize().x / 2) - (Cashsize.cx / 2),
							CASHSHOP_RENDER_BIGICON_START_Y + m_nShopMovePosY + CASHSHOP_TAB_H * nCount + CASH_SHOP_NUM,
							GUI_FONT_COLOR, buf, 0L);
					}

					if (pShopInfo->IsSoldOutShopItem())
					{
						m_pSoldOut_ToolTip->Move(CASHSHOP_RENDER_BIGICON_START_X + m_nShopMovePosX + CASHSHOP_TAB_W * nRenderWTab - CASHSHOP_SELECT_IMAGE_X - 3,
							CASHSHOP_RENDER_BIGICON_START_Y + m_nShopMovePosY + CASHSHOP_TAB_H * nCount - 5);
						m_pSoldOut_ToolTip->Render();
					}
					//if()
				}
			}
			nRenderWTab++;
			itCash++;
		}
		int nW = m_nItemSelect % CASHSHOP_TAB_W_SIZE;
		int nH = m_nItemSelect / CASHSHOP_TAB_H_SIZE;
		// ���� ������ ĳ�� ������
		m_pImgSelectItem->Move(m_nShopMovePosX + CASHSHOP_RENDER_BIGICON_START_X - CASHSHOP_SELECT_IMAGE_X + nW * CASHSHOP_TAB_W,
			m_nShopMovePosY + CASHSHOP_RENDER_BIGICON_START_Y - CASHSHOP_SELECT_IMAGE_Y + nH * CASHSHOP_TAB_H);
		m_pImgSelectItem->Render();

		m_pItem = g_pDatabase->GetServerItemInfo(m_vecImgCashIcon[nSelTab][m_nItemSelect + CASHSHOP_TAB_W_SIZE * m_pScroll->GetCurrentScrollIndex()].nItemNum);
	}
#else
	if (nSelTab != -1 && (m_vecImgCashIcon[nSelTab].size() - m_pScroll->GetCurrentScrollIndex()) > CASHSHOP_SHOW_ICON_MAX - 1)
	{
		m_pImgMoreItem[0]->Move(CASHSHOP_SCROLL_START_X + m_nShopMovePosX - 4, CASHSHOP_SCROLL_START_Y + m_nShopMovePosY + 298);
		m_pImgMoreItem[0]->Render();

		if (m_bMoreItemRenderFlag)
		{
			m_pImgMoreItem[3]->Move(CASHSHOP_SCROLL_START_X + m_nShopMovePosX - 4, CASHSHOP_SCROLL_START_Y + m_nShopMovePosY + 298);
			m_pImgMoreItem[3]->Render();
		}
	}
	if (m_pScroll->GetCurrentScrollIndex() > 0)
	{
		m_pImgMoreItem[2]->Move(CASHSHOP_SCROLL_START_X + m_nShopMovePosX - 4, CASHSHOP_SCROLL_START_Y + m_nShopMovePosY - 12);
		m_pImgMoreItem[2]->Render();

		if (m_bMoreItemRenderFlag)
		{
			m_pImgMoreItem[1]->Move(CASHSHOP_SCROLL_START_X + m_nShopMovePosX - 4, CASHSHOP_SCROLL_START_Y + m_nShopMovePosY - 12);
			m_pImgMoreItem[1]->Render();
		}
	}

	// ������ ������
	// 2009-01-28 by bhsohn ĳ���� ó��
	//if(m_vecImgCashIcon[m_nCurrentSelectTab].size()>m_pScroll->GetCurrentScrollIndex())
	if (nSelTab != -1 && m_vecImgCashIcon[nSelTab].size() > m_pScroll->GetCurrentScrollIndex())
	{
		int nCount = 0;
		int nItemCountNum = 1;
		vector<CashItemIcon>::iterator itCash = m_vecImgCashIcon[nSelTab].begin();
		while (itCash != m_vecImgCashIcon[nSelTab].end())
		{
			if (m_pScroll->GetCurrentScrollIndex() >= nItemCountNum)
			{
				itCash++;
				nItemCountNum++;
				continue;
			}

			if (nCount > CASHSHOP_SHOW_ICON_MAX - 1)
				break;
			if (itCash->vecImgCashIcon != NULL)
			{
				itCash->vecImgCashIcon->Move(CASHSHOP_RENDER_BIGICON_START_X + m_nShopMovePosX,
					CASHSHOP_RENDER_BIGICON_START_Y + m_nShopMovePosY + (CASHSHOP_RENDER_BIGICON_H*nCount));
				itCash->vecImgCashIcon->Render();
				// 2009-01-28 by bhsohn ĳ���� ó��
				SHOP_ITEM* pShopInfo = FindCashShopData(nSelTab, itCash->nItemNum);
				if (pShopInfo)
				{
					RenderNewShopInfo(CASHSHOP_RENDER_BIGICON_START_X + m_nShopMovePosX,
						CASHSHOP_RENDER_BIGICON_START_Y + m_nShopMovePosY + (CASHSHOP_RENDER_BIGICON_H*nCount),
						pShopInfo);
				}
				// end 2009-01-28 by bhsohn ĳ���� ó��
			}

			// ������ ����
			ITEM *item = g_pDatabase->GetServerItemInfo(itCash->nItemNum);

			m_pFontItem[nCount * 3]->DrawText(CASHSHOP_RENDER_FONT_ICONINFO_X + m_nShopMovePosX,
				CASHSHOP_RENDER_FONT_ICONINFO_Y + m_nShopMovePosY + (CASHSHOP_RENDER_FONT_ICONINFO_GAB*nCount),
				RGB(255, 255, 255), item->ItemName, 0L);

			wsprintf(buf, STRMSG_C_SHOP_0001, item->MinTradeQuantity);//"%d��/1��"
			m_pFontItem[(nCount * 3) + 1]->DrawText(CASHSHOP_RENDER_FONT_ICONINFO_X + m_nShopMovePosX,
				CASHSHOP_RENDER_FONT_ICONINFO_Y + m_nShopMovePosY + (CASHSHOP_RENDER_FONT_ICONINFO_GAB*nCount) + CASHSHOP_FONT_LINE_HEIGHT,
				RGB(178, 190, 255), buf, 0L);

			// 2006-06-07 by ispark, ���� üũ
//			wsprintf( buf, "%d", (int)(item->CashPrice*1.1f));
			wsprintf(buf, "%d", (int)(item->CashPrice * CASHSHOP_TEX));
			MakeCurrencySeparator(buf2, buf, 3, ',');
			m_pFontItem[(nCount * 3) + 2]->DrawText(CASHSHOP_RENDER_FONT_ICONINFO_X + m_nShopMovePosX,
				CASHSHOP_RENDER_FONT_ICONINFO_Y + m_nShopMovePosY + (CASHSHOP_RENDER_FONT_ICONINFO_GAB*nCount) + (CASHSHOP_FONT_LINE_HEIGHT * 2),
				GUI_FONT_COLOR_YM, buf2, 0L);

			m_pFontItem[(nCount * 3) + 2]->DrawText(CASHSHOP_RENDER_FONT_ICONINFO_X + m_nShopMovePosX + 45,
				CASHSHOP_RENDER_FONT_ICONINFO_Y + m_nShopMovePosY + (CASHSHOP_RENDER_FONT_ICONINFO_GAB*nCount) + (CASHSHOP_FONT_LINE_HEIGHT * 2),
				GUI_FONT_COLOR_YM, STRMSG_C_SHOP_0002, 0L);//"ĳ��"

			if (g_pShuttleChild->m_myShuttleInfo.Level >= item->ReqMinLevel &&
				(g_pShuttleChild->m_myShuttleInfo.Level <= item->ReqMaxLevel || item->ReqMaxLevel == 0))
			{
				if (item->ReqMaxLevel == 0)
				{
					wsprintf(buf, "\\q[Level %2.d]", item->ReqMinLevel);
					m_pFontItem[(nCount * 3) + 2]->DrawText(CASHSHOP_RENDER_FONT_ICONINFO_X + m_nShopMovePosX + 101,
						CASHSHOP_RENDER_FONT_ICONINFO_Y + m_nShopMovePosY + (CASHSHOP_RENDER_FONT_ICONINFO_GAB*nCount) + (CASHSHOP_FONT_LINE_HEIGHT * 2),
						GUI_FONT_COLOR_YM, buf, 0L);
				}
				else
				{
					wsprintf(buf, "\\q[Level %2.d~%2.d]", item->ReqMinLevel, item->ReqMaxLevel);
					m_pFontItem[(nCount * 3) + 2]->DrawText(CASHSHOP_RENDER_FONT_ICONINFO_X + m_nShopMovePosX + 83,
						CASHSHOP_RENDER_FONT_ICONINFO_Y + m_nShopMovePosY + (CASHSHOP_RENDER_FONT_ICONINFO_GAB*nCount) + (CASHSHOP_FONT_LINE_HEIGHT * 2),
						GUI_FONT_COLOR_YM, buf, 0L);
				}
			}
			else
			{
				if (item->ReqMaxLevel == 0)
				{
					wsprintf(buf, "\\r[Level %2.d]", item->ReqMinLevel);
					m_pFontItem[(nCount * 3) + 2]->DrawText(CASHSHOP_RENDER_FONT_ICONINFO_X + m_nShopMovePosX + 101,
						CASHSHOP_RENDER_FONT_ICONINFO_Y + m_nShopMovePosY + (CASHSHOP_RENDER_FONT_ICONINFO_GAB*nCount) + (CASHSHOP_FONT_LINE_HEIGHT * 2),
						GUI_FONT_COLOR_YM, buf, 0L);
				}
				else
				{
					wsprintf(buf, "\\r[Level %2.d~%2.d]", item->ReqMinLevel, item->ReqMaxLevel);
					m_pFontItem[(nCount * 3) + 2]->DrawText(CASHSHOP_RENDER_FONT_ICONINFO_X + m_nShopMovePosX + 83,
						CASHSHOP_RENDER_FONT_ICONINFO_Y + m_nShopMovePosY + (CASHSHOP_RENDER_FONT_ICONINFO_GAB*nCount) + (CASHSHOP_FONT_LINE_HEIGHT * 2),
						GUI_FONT_COLOR_YM, buf, 0L);
				}
			}

			// 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
			SHOP_ITEM* pShopInfo = FindCashShopData(nSelTab, itCash->nItemNum);
			if (pShopInfo && pShopInfo->IsLimitedEditionShopItem())
			{
				CINFImage* pImg;
				if (m_bBling)
					pImg = m_pLimitedEdtion[0];
				else
					pImg = m_pLimitedEdtion[1];

				if (pShopInfo->IsSoldOutShopItem())
					pImg = m_pLimitedEdtion[0];

				pImg->Move(CASHSHOP_RENDER_BIGICON_START_X + m_nShopMovePosX - 2,
					CASHSHOP_RENDER_BIGICON_START_Y + m_nShopMovePosY - 3 + (CASHSHOP_RENDER_BIGICON_H*nCount));
				pImg->Render();

				sprintf(buf, STRMSG_C_100127_0301, pShopInfo->RemainCountForLimitedEdition);
				m_pFontItem[(nCount * 3) + 2]->DrawText(CASHSHOP_RENDER_BIGICON_START_X + m_nShopMovePosX + 295,
					CASHSHOP_RENDER_FONT_ICONINFO_Y + m_nShopMovePosY + (CASHSHOP_RENDER_FONT_ICONINFO_GAB*nCount) + (CASHSHOP_FONT_LINE_HEIGHT * 2),
					GUI_FONT_COLOR, buf, 0L);

				if (pShopInfo->IsSoldOutShopItem())
				{
					m_pSoldOut_Item->Move(CASHSHOP_RENDER_BIGICON_START_X + m_nShopMovePosX - 2,
						CASHSHOP_RENDER_BIGICON_START_Y + m_nShopMovePosY - 3 + (CASHSHOP_RENDER_BIGICON_H*nCount));
					m_pSoldOut_Item->Render();
				}
			}
			// end 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���

			nCount++;
			itCash++;
		}
	}

	// 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
	// ���� ������ ĳ�� ������
	m_pImgSelectItem->Move(CASHSHOP_RENDER_TAB_X + m_nShopMovePosX + 9,
		CASHSHOP_RENDER_TAB_Y + m_nShopMovePosY + CASHSHOP_RENDER_SELECT_TAB_GAB + (64 * m_nItemSelect));
	m_pImgSelectItem->Render();
	// end 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���


	// 2009-01-28 by bhsohn ĳ���� ó��
	//if(m_nCurrentSelectTab == CASHSHOP_ITEM_SKIN_ITEM)		//CASHSHOP_ITEM_SKIN_ITEM				4	
	if (nSelTab != -1 && nSelTab == GetSkinItemIndex())		//CASHSHOP_ITEM_SKIN_ITEM				4	
	{
		g_pShuttleChild->RenderInvenBack();
		// 2008-10-28 by bhsohn ĳ�� ���� ���� ����
		//g_pShuttleChild->RenderMirror(1);
		POINT ptMirrorPos;
		ptMirrorPos.x = CASHSHOP_RENDER_START_X + CASH_SHOP_GEAR_UNIT_X;
		ptMirrorPos.y = CASHSHOP_RENDER_START_Y + CASH_SHOP_GEAR_UNIT_Y;
		g_pShuttleChild->RenderMirror(&ptMirrorPos);

		// �����̼� �̹���
		if (m_nButtonState == ROTATION_NONE || m_nRotationState == ROTATION_STATE_N)
		{
			m_pRotationBase->Move(CASHSHOP_RENDER_START_X + CASH_SHOP_GEAR_ROTATION_KEY_X, CASHSHOP_RENDER_START_Y + CASH_SHOP_GEAR_ROTATION_KEY_Y);
			m_pRotationBase->Render();
		}
		else
		{
			m_pDirection[m_nButtonState][m_nRotationState]->Move(CASHSHOP_RENDER_START_X + CASH_SHOP_GEAR_ROTATION_KEY_X, CASHSHOP_RENDER_START_Y + CASH_SHOP_GEAR_ROTATION_KEY_Y);
			m_pDirection[m_nButtonState][m_nRotationState]->Render();
		}

		// 2009-01-28 by bhsohn ĳ���� ó��
//		if(m_vecImgCashIcon[m_nCurrentSelectTab].size() > (m_nItemSelect+m_pScroll->GetCurrentScrollIndex()))
// 		{
// 			m_pItem = g_pDatabase->GetServerItemInfo(m_vecImgCashIcon[m_nCurrentSelectTab][m_nItemSelect+m_pScroll->GetCurrentScrollIndex()].nItemNum);
// 		}
		if (m_vecImgCashIcon[nSelTab].size() > (m_nItemSelect + m_pScroll->GetCurrentScrollIndex()))
		{
			m_pItem = g_pDatabase->GetServerItemInfo(m_vecImgCashIcon[nSelTab][m_nItemSelect + m_pScroll->GetCurrentScrollIndex()].nItemNum);
		}
	}

	// ������ ���� â ������	
	// 2009-01-28 by bhsohn ĳ���� ó��
	//else if(m_nItemSelect+m_pScroll->GetCurrentScrollIndex() < m_vecImgCashIcon[m_nCurrentSelectTab].size())
	else if (nSelTab != -1 && m_nItemSelect + m_pScroll->GetCurrentScrollIndex() < m_vecImgCashIcon[nSelTab].size())
	{
		char chDelChar[10] = { 0, };
		int nDelChar;
		wsprintf(chDelChar, "    ");
		nDelChar = m_pFontItemInfo[1]->GetStringSize(chDelChar).cx;

		memset(buf, 0x00, 512);
		memset(buf2, 0x00, 512);
		// 2009-01-28 by bhsohn ĳ���� ó��
		//ITEM *item = g_pDatabase->GetServerItemInfo(m_vecImgCashIcon[m_nCurrentSelectTab][m_nItemSelect+m_pScroll->GetCurrentScrollIndex()].nItemNum);		
		ITEM *item = g_pDatabase->GetServerItemInfo(m_vecImgCashIcon[nSelTab][m_nItemSelect + m_pScroll->GetCurrentScrollIndex()].nItemNum);

		if (m_vecImgCashIcon[nSelTab][m_nItemSelect + m_pScroll->GetCurrentScrollIndex()].vecImgCashIcon != NULL)
		{
			m_vecImgCashIcon[nSelTab][m_nItemSelect + m_pScroll->GetCurrentScrollIndex()].vecImgCashIcon->Move(CASHSHOP_ITEM_INFO_ICON_X + m_nShopMovePosX,
				CASHSHOP_ITEM_INFO_ICON_Y + m_nShopMovePosY);
			m_vecImgCashIcon[nSelTab][m_nItemSelect + m_pScroll->GetCurrentScrollIndex()].vecImgCashIcon->Render();
		}

		// 2006-06-08 by ispark, �̸� ��ġ ����
		//		int nCharlen = (strlen(item->ItemName)*CASHSHOP_SIDE_WIDTH_ENGLISH)/2;
		int nCharlen = m_pFontItemInfo[0]->GetStringSize(item->ItemName).cx / 2;
		m_pFontItemInfo[0]->DrawText(CASHSHOP_ITEM_INFO_NAME_X + m_nShopMovePosX - nCharlen, CASHSHOP_ITEM_INFO_NAME_Y + m_nShopMovePosY, RGB(255, 255, 255),
			item->ItemName);
		if (item->ReqMaxLevel == 0)
		{
			//			wsprintf( buf, "%d",(int)(item->CashPrice*1.1f));CASHSHOP_TEX
			wsprintf(buf, "%d", (int)(item->CashPrice*CASHSHOP_TEX));
			MakeCurrencySeparator(buf2, buf, 3, ',');
			wsprintf(buf, STRMSG_C_SHOP_0003, item->MinTradeQuantity, buf2, item->ReqMinLevel);//"%3.d��/1�� \\e%s ĳ�� \\q[Level %d]"
//			nCharlen = (strlen(buf)*CASHSHOP_SIDE_WIDTH_ENGLISH)/2;
			nCharlen = m_pFontItemInfo[1]->GetStringSize(buf).cx;
			nCharlen = (nCharlen - nDelChar) / 2;
			m_pFontItemInfo[1]->DrawText(CASHSHOP_ITEM_INFO_NAME_X + m_nShopMovePosX - nCharlen, CASHSHOP_ITEM_INFO_NAME_Y + m_nShopMovePosY + CASHSHOP_FONT_LINE_HEIGHT,
				RGB(178, 190, 255), buf, 0L);
		}
		else
		{
			//			wsprintf( buf, "%d",(int)(item->CashPrice*1.1f));CASHSHOP_TEX
			wsprintf(buf, "%d", (int)(item->CashPrice*CASHSHOP_TEX));
			MakeCurrencySeparator(buf2, buf, 3, ',');
			wsprintf(buf, STRMSG_C_SHOP_0004, item->MinTradeQuantity, buf2,  //"%3.d��/1�� \\e%s ĳ�� \\q[Level %d~%d]"
				item->ReqMinLevel, item->ReqMaxLevel);
			//			nCharlen = (strlen(buf)*CASHSHOP_SIDE_WIDTH_ENGLISH)/2;
			nCharlen = m_pFontItemInfo[1]->GetStringSize(buf).cx;
			nCharlen = (nCharlen - nDelChar) / 2;
			m_pFontItemInfo[1]->DrawText(CASHSHOP_ITEM_INFO_NAME_X + m_nShopMovePosX - nCharlen, CASHSHOP_ITEM_INFO_NAME_Y + m_nShopMovePosY + CASHSHOP_FONT_LINE_HEIGHT,
				RGB(178, 190, 255), buf, 0L);
		}

		// 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
		SHOP_ITEM* pShopInfo = FindCashShopData(nSelTab, item->ItemNum);
		if (pShopInfo && pShopInfo->IsLimitedEditionShopItem())
		{
			sprintf(buf, STRMSG_C_100127_0301, pShopInfo->RemainCountForLimitedEdition);	// "\\y���� ����: %d��\\y"

			m_pFontItemInfo[1]->DrawText(CASHSHOP_ITEM_INFO_NAME_X + m_nShopMovePosX - m_pFontItemInfo[1]->GetStringSize(buf).cx / 2,
				CASHSHOP_ITEM_INFO_NAME_Y + m_nShopMovePosY + CASHSHOP_FONT_LINE_HEIGHT * 2,
				GUI_FONT_COLOR, buf, 0L);
		}
		// end 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
		m_pItem = item;
	}
#endif
	// end 2011. 11. 17 by jskim EP4 UI ����

	// ���� ĳ�� ���	
//	wsprintf( buf, "%d", (int)(m_nMyCashPoint*1.1f));
	wsprintf(buf, "%d", (int)((m_nMyCashPoint + m_nGiftCardPoint)*CASHSHOP_TEX));
	MakeCurrencySeparator(buf2, buf, 3, ',');
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	SIZE Size = m_pFontItemInfo[CASH_SHOP_ITEM_INFO - 1]->GetStringSize(buf2);
	m_pFontItemInfo[CASH_SHOP_ITEM_INFO - 1]->DrawText(CASHSHOP_RENDER_MYCASH_X + m_nShopMovePosX - Size.cx, CASHSHOP_RENDER_MYCASH_Y + m_nShopMovePosY,
#else
	m_pFontItemInfo[CASH_SHOP_ITEM_INFO - 1]->DrawText(CASHSHOP_RENDER_MYCASH_X + m_nShopMovePosX, CASHSHOP_RENDER_MYCASH_Y + m_nShopMovePosY,
#endif
		GUI_FONT_COLOR_YM, buf2);

	// ��ư������
	m_pImgChargeButton[m_nCurrentMouseStateCharge]->Move(CASHSHOP_RENDER_CHARGE_BUTTON_X + m_nShopMovePosX,
		CASHSHOP_RENDER_CHARGE_BUTTON_Y + m_nShopMovePosY);
	m_pImgChargeButton[m_nCurrentMouseStateCharge]->Render();
	// 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
#ifdef C_CASHSHOP_IMAGE_REMOVE
	//
#else
	if (m_pImgGiftButton)
	{
		// �����ϱ� ��ư ������.
		m_pImgGiftButton->Render();
	}
#endif
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
#else
	RenderDescIcon();
#endif
	// ������ ����
	//RenderDescIcon();

	// ��ũ�� ������ 
	m_pScroll->Render();
	if (g_pGameMain->m_bSendCashItemBuy)
	{
#ifdef SHOP_PRICES_PER_BUILDING_NPC
		// Get the Shop Information
		SHOP_ITEM* pShopInfo = FindCashShopData(nSelTab, m_pItem->ItemNum);
		if (!pShopInfo)
		{
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_SHOP_0015, COLOR_ERROR);
			DBGOUT("ERROR: During purchase of Cash Item! The Shop Information could not be found for the Item \"%s\" Num: %d", m_pItem->ItemName, m_pItem->ItemNum);
		}
		else
		{
			if (m_nMyCashPoint + m_nGiftCardPoint >= pShopInfo->Price)
#else
		if (m_nMyCashPoint + m_nGiftCardPoint >= m_pItem->CashPrice)
#endif 

		{
			// 2005-03-07 by jschoi
			g_pD3dApp->m_bRequestEnable = FALSE;

			MSG_FC_SHOP_BUY_CASH_ITEM sMsg;
			memset(&sMsg, 0x00, sizeof(sMsg));
			sMsg.BuildingIndex = CASH_SHOP_BUILDING_INDEX;
			sMsg.ItemNum = m_pItem->ItemNum;
			sMsg.Amount = m_pItem->MinTradeQuantity;
			sMsg.GiveTargetCharacterUID = m_nGiveTargetCharacterUID ? m_nGiveTargetCharacterUID : 0;
			g_pFieldWinSocket->SendMsg(T_FC_SHOP_BUY_CASH_ITEM, (char*)&sMsg, sizeof(sMsg));
			g_pGameMain->m_bSendCashItemBuy = FALSE;
			m_nGiveTargetCharacterUID = 0;
		}
		else
		{
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_SHOP_0005, COLOR_ERROR);//"ĳ���� �����մϴ�."
			g_pGameMain->m_bSendCashItemBuy = FALSE;
		}
#ifdef SHOP_PRICES_PER_BUILDING_NPC
		}
#endif // SHOP_PRICES_PER_BUILDING_NPC

	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
POINT pt;
GetCursorPos(&pt);
ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
CheckMouseReverse(&pt);
nSelTab = GetTabToItemIdx(m_nCurrentSelectTab);
if (nSelTab != -1 && m_vecImgCashIcon[nSelTab].size() / CASHSHOP_TAB_W_SIZE > m_pScroll->GetCurrentScrollIndex())
{
	int nCount = 0;
	int nItemCountNum = 1;
	int nRenderWTab = 0;
	vector<CashItemIcon>::iterator itCash = m_vecImgCashIcon[nSelTab].begin();
	while (itCash != m_vecImgCashIcon[nSelTab].end())
	{
		int nTemp = nItemCountNum / CASHSHOP_TAB_W_SIZE;
		if (nItemCountNum % CASHSHOP_TAB_W_SIZE != 0)
		{
			nTemp += 1;
		}

		if (m_pScroll->GetCurrentScrollIndex() >= nTemp)
		{
			itCash++;
			nItemCountNum++;
			continue;
		}

		if (nRenderWTab >= CASHSHOP_TAB_W_SIZE)
		{
			nRenderWTab = 0;
			nCount += 1;
		}

		if (nCount >= CASHSHOP_TAB_H_SIZE)
		{
			break;
		}

		if (itCash->vecImgCashIcon != NULL)
		{
			if (pt.x >= m_nShopMovePosX + CASHSHOP_RENDER_BIGICON_START_X - CASHSHOP_ITME_POS_GAB + CASHSHOP_TAB_W * nRenderWTab &&
				pt.x <= m_nShopMovePosX + CASHSHOP_RENDER_BIGICON_START_X - CASHSHOP_ITME_POS_GAB + CASHSHOP_TAB_W * (nRenderWTab + 1) &&
				pt.y >= m_nShopMovePosY + CASHSHOP_RENDER_BIGICON_START_Y + CASHSHOP_TAB_H * nCount &&
				pt.y <= m_nShopMovePosY + CASHSHOP_RENDER_BIGICON_START_Y + CASHSHOP_TAB_H * (nCount + 1))
			{
				ITEM *item = g_pDatabase->GetServerItemInfo(itCash->nItemNum);
				RenderDescIcon(pt, item, nRenderWTab, nCount);
				break;
			}
		}
		nRenderWTab++;
		itCash++;
	}
}
#endif
}

void CINFCityCashShop::OnChangeWearItem()
{
	ITEM *item = NULL;
	if (m_nItemSelect < 0)
		return;

	// 2009-01-28 by bhsohn ĳ���� ó��
	int nSelTab = GetTabToItemIdx(m_nCurrentSelectTab);
	if (nSelTab == -1)
	{
		return;
	}
	// end 2009-01-28 by bhsohn ĳ���� ó��
	int nSelectTotalIdx = m_nItemSelect + m_pScroll->GetCurrentScrollIndex();

	if (m_vecItemInfo[nSelTab].size() > nSelectTotalIdx)
	{
		item = g_pDatabase->GetServerItemInfo(m_vecItemInfo[nSelTab][nSelectTotalIdx]->ItemNum);
	}
	if (item)
	{
		if (CompareBitFlag(item->ReqUnitKind, g_pShuttleChild->m_myShuttleInfo.UnitKind))
		{// ���� ����� ��Ų�� �����Ѵ�.
			// g_pGameMain->ChangeItemByShop(item->ItemNum, nUnitColor);
			CItemInfo *pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos(POS_CENTER);
			if (pItemInfo)
			{
				int nArmorColor = 0;
				if (pItemInfo->ItemInfo->SourceIndex == 0)
					nArmorColor = pItemInfo->ItemNum;
				else
					nArmorColor = pItemInfo->ItemInfo->SourceIndex;
				int nColorNum = item->ItemNum - nArmorColor;

				g_pGameMain->ChangeItemByShop(item->ItemNum, nColorNum);
			}
		}
	}
}
void CINFCityCashShop::Tick()
{
	FLOG("CINFCityCashShop::Tick()");

	if (m_bTurnLeft)
		g_pShuttleChild->MirrorTurnUp();
	if (m_bTurnRight)
		g_pShuttleChild->MirrorTurnDown();
	if (m_bTurnUp)
		g_pShuttleChild->MirrorTurnLeft();
	if (m_bTurnDown)
		g_pShuttleChild->MirrorTurnRight();

	// 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
	DWORD nCurTime = timeGetTime();
	if (nCurTime - m_nBlingStartTime > 1000)
	{
		m_nBlingStartTime = nCurTime;

		m_bBling = !m_bBling;
	}
	// end 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
}
void CINFCityCashShop::SetByPushButton(BOOL bFlag)
{
	for (int i = 0; i < 4; i++)
	{
		m_bByPush[i] = FALSE;
	}
}

int CINFCityCashShop::ProcessRotationUnit(POINT pt, UINT uMsg)
{
	if (uMsg == WM_LBUTTONUP)	SetByPushButton(FALSE);

	m_nRotationState = ROTATION_NONE;
	m_nButtonState = ROTATION_STATE_N;

	m_bTurnLeft = FALSE;
	m_bTurnRight = FALSE;
	m_bTurnUp = FALSE;
	m_bTurnDown = FALSE;


	int PosX = CASHSHOP_RENDER_START_X + CASH_SHOP_GEAR_ROTATION_KEY_X;
	int PosY = CASHSHOP_RENDER_START_Y + CASH_SHOP_GEAR_ROTATION_KEY_Y;

	if (pt.x > PosX && pt.x< PosX + 12 && pt.y>PosY + 8 && pt.y < PosY + 30)
	{	// ���� ������
		switch (uMsg)
		{
		case WM_MOUSEMOVE:
			m_nRotationState = ROTATION_LEFT;
			m_nButtonState = ROTATION_STATE_O;
			if (m_bByPush[0] == TRUE)
			{
				m_nRotationState = ROTATION_LEFT;
				m_nButtonState = ROTATION_STATE_P;
				m_bTurnLeft = TRUE;
			}
			break;
		case WM_LBUTTONDOWN:
			m_nRotationState = ROTATION_LEFT;
			m_nButtonState = ROTATION_STATE_P;
			m_bTurnLeft = TRUE;
			m_bByPush[0] = TRUE;
			return INF_MSGPROC_BREAK;
		case WM_LBUTTONUP:
			m_nRotationState = ROTATION_LEFT;
			m_nButtonState = ROTATION_STATE_N;
			return INF_MSGPROC_BREAK;
		}
	}
	else if (pt.x > PosX + 34 && pt.x< PosX + 46 && pt.y>PosY + 8 && pt.y < PosY + 30)
	{	// ������ ������
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
			m_nRotationState = ROTATION_RIGHT;
			m_nButtonState = ROTATION_STATE_P;
			m_bTurnRight = TRUE;
			m_bByPush[1] = TRUE;
			return INF_MSGPROC_BREAK;
		case WM_MOUSEMOVE:
			m_nRotationState = ROTATION_RIGHT;
			m_nButtonState = ROTATION_STATE_O;
			if (m_bByPush[1] == TRUE)
			{
				m_nRotationState = ROTATION_RIGHT;
				m_nButtonState = ROTATION_STATE_P;
				m_bTurnRight = TRUE;
			}

			break;
		case WM_LBUTTONUP:
			m_nRotationState = ROTATION_RIGHT;
			m_nButtonState = ROTATION_STATE_N;
			return INF_MSGPROC_BREAK;
		}
	}
	else if (pt.x > PosX + 13 && pt.x< PosX + 33 && pt.y>PosY && pt.y < PosY + 10)
	{	// ���� ������
		switch (uMsg)
		{
		case WM_MOUSEMOVE:
			m_nRotationState = ROTATION_UP;
			m_nButtonState = ROTATION_STATE_O;
			if (m_bByPush[2] == TRUE)
			{
				m_nRotationState = ROTATION_UP;
				m_nButtonState = ROTATION_STATE_P;
				m_bTurnUp = TRUE;
			}
			break;
		case WM_LBUTTONDOWN:
			m_nRotationState = ROTATION_UP;
			m_nButtonState = ROTATION_STATE_P;
			m_bTurnUp = TRUE;
			m_bByPush[2] = TRUE;
			return INF_MSGPROC_BREAK;
		case WM_LBUTTONUP:
			m_nRotationState = ROTATION_UP;
			m_nButtonState = ROTATION_STATE_N;

			return INF_MSGPROC_BREAK;
		}
	}
	else if (pt.x > PosX + 13 && pt.x<PosX + 33 && pt.y>PosY + 26 && pt.y < PosY + 38)
	{	// �Ʒ��� ������
		switch (uMsg)
		{
		case WM_MOUSEMOVE:
			m_nRotationState = ROTATION_DOWN;
			m_nButtonState = ROTATION_STATE_O;
			if (m_bByPush[3] == TRUE)
			{
				m_nRotationState = ROTATION_DOWN;
				m_nButtonState = ROTATION_STATE_P;
				m_bTurnDown = TRUE;
			}
			break;
		case WM_LBUTTONDOWN:
			m_nRotationState = ROTATION_DOWN;
			m_nButtonState = ROTATION_STATE_P;
			m_bTurnDown = TRUE;
			m_bByPush[3] = TRUE;
			return INF_MSGPROC_BREAK;
		case WM_LBUTTONUP:
			m_nRotationState = ROTATION_DOWN;
			m_nButtonState = ROTATION_STATE_N;
			return INF_MSGPROC_BREAK;
		}
	}
	else if (pt.x > PosX + 13 && pt.x<PosX + 33 && pt.y>PosY + 13 && pt.y < PosY + 25)
	{	// �߾� ������
		switch (uMsg)
		{
		case WM_MOUSEMOVE:
			m_nRotationState = ROTATION_ORIGINAL;
			m_nButtonState = ROTATION_STATE_O;
			break;
		case WM_LBUTTONDOWN:
			m_nRotationState = ROTATION_ORIGINAL;
			m_nButtonState = ROTATION_STATE_P;
			g_pShuttleChild->MirrorTurnOrig();
			return INF_MSGPROC_BREAK;
		case WM_LBUTTONUP:
			m_nRotationState = ROTATION_ORIGINAL;
			m_nButtonState = ROTATION_STATE_N;
			return INF_MSGPROC_BREAK;
		}
	}

	return INF_MSGPROC_NORMAL;
}
int CINFCityCashShop::ProcessRotationUnitWnd(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// �⺻ ���� X ��ġ = 16
	// �⺻ ���� Y ��ġ = nWindowPosY + 207	
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
		CheckMouseReverse(&pt);
		if (ProcessRotationUnit(pt, uMsg) == INF_MSGPROC_BREAK)
			return INF_MSGPROC_BREAK;
	}
	break;
	}
	return INF_MSGPROC_NORMAL;
}

int CINFCityCashShop::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG("CINFCityCashShop::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)");
	int nSelTmpTab = GetTabToItemIdx(m_nCurrentSelectTab);

	if (g_pGameMain->m_bBuyMessage == TRUE)
	{
		return INF_MSGPROC_NORMAL;
	}
	if (m_pScroll)
	{
		// 2009-01-28 by bhsohn ĳ���� ó��
		int nOldScroll = m_pScroll->GetCurrentScrollIndex();
		if (m_pScroll->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
		{
			// 2009-01-28 by bhsohn ĳ���� ó��
			if (nOldScroll != m_pScroll->GetCurrentScrollIndex())
			{
				UpdateGiftButton(nSelTmpTab, m_nItemSelect);
			}
			// end 2009-01-28 by bhsohn ĳ���� ó��

			return INF_MSGPROC_BREAK;
		}
		// 2009-01-28 by bhsohn ĳ���� ó��
		if (nOldScroll != m_pScroll->GetCurrentScrollIndex())
		{
			UpdateGiftButton(nSelTmpTab, m_nItemSelect);
		}
		// end 2009-01-28 by bhsohn ĳ���� ó��
	}
	// 2009-01-28 by bhsohn ĳ���� ó��
	//if(m_nCurrentSelectTab == CASHSHOP_ITEM_SKIN_ITEM)	
	if (nSelTmpTab != -1 && nSelTmpTab == GetSkinItemIndex())
	{
		if (ProcessRotationUnitWnd(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
		{
			return INF_MSGPROC_BREAK;
		}
	}

	switch (uMsg)
	{
	case WM_MOUSEWHEEL:
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
		CheckMouseReverse(&pt);
		if (pt.x >= CASHSHOP_RENDER_START_X + m_nShopMovePosX && pt.x <= CASHSHOP_RENDER_START_X + m_nShopMovePosX + (m_pImgBack->GetMaxPos().x - m_pImgBack->GetMinPos().x) &&
			pt.y >= CASHSHOP_RENDER_START_Y + m_nShopMovePosY && pt.y <= CASHSHOP_RENDER_START_Y + m_nShopMovePosY + (m_pImgBack->GetMaxPos().y - m_pImgBack->GetMinPos().y))
		{
			return INF_MSGPROC_BREAK;
		}
#endif			
		OnChangeWearItem();
	}
	break;
	case WM_MOUSEMOVE:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		CheckMouseReverse(&pt);
		// 2007-10-16 by bhsohn �ɽ��� ������ư ����
//			if( pt.y > CASHSHOP_RENDER_BUY_BUTTON_Y+m_nShopMovePosY && 
//				pt.y < CASHSHOP_RENDER_BUY_BUTTON_Y+m_nShopMovePosY + CASHSHOP_RENDER_BUY_BUTTON_H &&
//				pt.x > CASHSHOP_RENDER_BUY_BUTTON_X+m_nShopMovePosX && 
//				pt.x < CASHSHOP_RENDER_BUY_BUTTON_X+m_nShopMovePosX + CASHSHOP_RENDER_BUY_BUTTON_W)
//			{
//				if(m_bCurrentMouseStateBuy)
//					m_nCurrentMouseStateBuy = CASHSHOP_BUTTON_STATE_PUH;
//				else if(m_nCurrentMouseStateBuy != CASHSHOP_BUTTON_STATE_PUH)
//					m_nCurrentMouseStateBuy = CASHSHOP_BUTTON_STATE_OVE;
//			}
//			else 
//			{
//				m_nCurrentMouseStateBuy = CASHSHOP_BUTTON_STATE_NOM;
//			}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		m_pCloseBtn->OnMouseMove(pt);
#endif

		if (pt.y > CASHSHOP_RENDER_CHARGE_BUTTON_Y + m_nShopMovePosY &&
			pt.y < CASHSHOP_RENDER_CHARGE_BUTTON_Y + m_nShopMovePosY + CASHSHOP_RENDER_CHARGE_BUTTON_H &&
			pt.x > CASHSHOP_RENDER_CHARGE_BUTTON_X + m_nShopMovePosX &&
			pt.x < CASHSHOP_RENDER_CHARGE_BUTTON_X + m_nShopMovePosX + CASHSHOP_RENDER_CHARGE_BUTTON_W)
		{
			if (m_bCurrentMouseStateCharge)
				m_nCurrentMouseStateCharge = CASHSHOP_BUTTON_STATE_PUH;
			else if (m_nCurrentMouseStateCharge != CASHSHOP_BUTTON_STATE_PUH)
			{
				// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
				g_pInterface->m_pToolTip->SetNameToolTip(pt, "STRTOOLTIP72");
				// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����	
				m_nCurrentMouseStateCharge = CASHSHOP_BUTTON_STATE_OVE;
			}
		}
		else
		{
			// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
			g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
			// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
			m_nCurrentMouseStateCharge = CASHSHOP_BUTTON_STATE_NOM;
		}

		// â�̵�
		if (m_bMoveShop)
		{
			//m_nStartFAQPositionX = m_nFAQBoxStartXOld + pt.x - m_pointBeforeMousePoints.x;
			//m_nStartFAQPositionY = m_nFAQBoxStartYOld + pt.y - m_pointBeforeMousePoints.y;

			m_nShopMovePosX = m_nShopOldPosX + pt.x - m_pointBeforeMousePoints.x;
			m_nShopMovePosY = m_nShopOldPosY + pt.y - m_pointBeforeMousePoints.y;
		}
		// 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
#ifdef C_CASHSHOP_IMAGE_REMOVE
			//
#else
		if (m_pImgGiftButton)
			m_pImgGiftButton->OnMouseMove(pt);
#endif
	}
	break;
	case WM_LBUTTONDOWN:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		CheckMouseReverse(&pt);

		// �޼��� �ڽ������� ���¿��� ���ٿ�����
		BOOL bBreak = FALSE;
		vector<CINFMessageBox*>::iterator it = g_pGameMain->m_pInfWindow->m_vecMessageBox.begin();
		while (it != g_pGameMain->m_pInfWindow->m_vecMessageBox.end())
		{
			CINFMessageBox* pMsgBox = *it;
			// �޼��� �ڽ� �����(��ȯ)
			if (_Q_CASHITEM_BUY == pMsgBox->m_nMsgType)
			{
				bBreak = TRUE;
			}
			it++;
		}
		if (TRUE == bBreak) break;

		// 2007-10-16 by bhsohn �ɽ��� ������ư ����			
//			if( pt.y > CASHSHOP_RENDER_BUY_BUTTON_Y+m_nShopMovePosY && 
//				pt.y < CASHSHOP_RENDER_BUY_BUTTON_Y+m_nShopMovePosY + CASHSHOP_RENDER_BUY_BUTTON_H &&
//				pt.x > CASHSHOP_RENDER_BUY_BUTTON_X+m_nShopMovePosX && 
//				pt.x < CASHSHOP_RENDER_BUY_BUTTON_X+m_nShopMovePosX + CASHSHOP_RENDER_BUY_BUTTON_W)
//			{
//				m_bCurrentMouseStateBuy = TRUE;
//				m_nCurrentMouseStateBuy = CASHSHOP_BUTTON_STATE_PUH;
//				return INF_MSGPROC_BREAK;
//			}
//			else 
//			{
//				m_nCurrentMouseStateBuy = CASHSHOP_BUTTON_STATE_NOM;
//			}
// 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
#ifdef C_CASHSHOP_IMAGE_REMOVE
			//
#else
		if (m_pImgGiftButton)
			m_pImgGiftButton->OnLButtonDown(pt);
#endif

		if (pt.y > CASHSHOP_RENDER_CHARGE_BUTTON_Y + m_nShopMovePosY &&
			pt.y < CASHSHOP_RENDER_CHARGE_BUTTON_Y + m_nShopMovePosY + CASHSHOP_RENDER_CHARGE_BUTTON_H &&
			pt.x > CASHSHOP_RENDER_CHARGE_BUTTON_X + m_nShopMovePosX &&
			pt.x < CASHSHOP_RENDER_CHARGE_BUTTON_X + m_nShopMovePosX + CASHSHOP_RENDER_CHARGE_BUTTON_W)
		{
			m_bCurrentMouseStateCharge = TRUE;
			m_nCurrentMouseStateCharge = CASHSHOP_BUTTON_STATE_PUH;
			return INF_MSGPROC_BREAK;
		}
		else
		{
			m_nCurrentMouseStateCharge = CASHSHOP_BUTTON_STATE_NOM;
		}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		int i;
		int nSelTab = GetTabToItemIdx(m_nCurrentSelectTab);
		if (nSelTab != -1 && m_vecImgCashIcon[nSelTab].size() / CASHSHOP_TAB_W_SIZE > m_pScroll->GetCurrentScrollIndex())
		{
			int nCount = 0;
			int nItemCountNum = 1;
			int nRenderWTab = 0;
			vector<CashItemIcon>::iterator itCash = m_vecImgCashIcon[nSelTab].begin();
			while (itCash != m_vecImgCashIcon[nSelTab].end())
			{
				int nTemp = nItemCountNum / CASHSHOP_TAB_W_SIZE;
				if (nItemCountNum % CASHSHOP_TAB_W_SIZE != 0)
				{
					nTemp += 1;
				}

				if (m_pScroll->GetCurrentScrollIndex() >= nTemp)
				{
					itCash++;
					nItemCountNum++;
					continue;
				}

				if (nRenderWTab >= CASHSHOP_TAB_W_SIZE)
				{
					nRenderWTab = 0;
					nCount += 1;
				}

				if (nCount >= CASHSHOP_TAB_H_SIZE)
				{
					break;
				}

				if (itCash->vecImgCashIcon != NULL)
				{
					if (pt.x >= m_nShopMovePosX + CASHSHOP_RENDER_BIGICON_START_X - CASHSHOP_ITME_POS_GAB + CASHSHOP_TAB_W * nRenderWTab &&
						pt.x <= m_nShopMovePosX + CASHSHOP_RENDER_BIGICON_START_X - CASHSHOP_ITME_POS_GAB + CASHSHOP_TAB_W * (nRenderWTab + 1) &&
						pt.y >= m_nShopMovePosY + CASHSHOP_RENDER_BIGICON_START_Y + CASHSHOP_TAB_H * nCount &&
						pt.y <= m_nShopMovePosY + CASHSHOP_RENDER_BIGICON_START_Y + CASHSHOP_TAB_H * (nCount + 1))
					{
						int nSelTab = GetTabToItemIdx(m_nCurrentSelectTab);

						if (nSelTab != -1 && m_vecImgCashIcon[nSelTab].size() > (nRenderWTab + CASHSHOP_TAB_H_SIZE * nCount + m_pScroll->GetCurrentScrollIndex()))
						{
							m_nItemSelect = nRenderWTab + CASHSHOP_TAB_H_SIZE * nCount;
							if (nSelTab != -1 && nSelTab == GetSkinItemIndex())
							{
								OnChangeWearItem();
							}
						}
						UpdateGiftButton(nSelTab, m_nItemSelect);
						break;
					}
					// ���� �Ǹ� ������ 
					// 						SHOP_ITEM* pShopInfo = FindCashShopData(nSelTab, itCash->nItemNum);
					// 						if( pShopInfo && pShopInfo->IsLimitedEditionShopItem() )
					// 						{
					// 							CINFImage* pImg;
					// 							if( m_bBling )
					// 								pImg = m_pLimitedEdtion[ 0 ];
					// 							else
					// 								pImg = m_pLimitedEdtion[ 1 ];
					// 							
					// 							if( pShopInfo->IsSoldOutShopItem() )
					// 								pImg = m_pLimitedEdtion[ 0 ];
					// 							
					// 							pImg->Move( CASHSHOP_RENDER_BIGICON_START_X + m_nShopMovePosX - 2,
					// 										CASHSHOP_RENDER_BIGICON_START_Y + m_nShopMovePosY - 3 + (CASHSHOP_RENDER_BIGICON_H*nCount) );
					// 							pImg->Render();
					// 							
					// 							sprintf( buf, STRMSG_C_100127_0301, pShopInfo->RemainCountForLimitedEdition );
					// 									 m_pFontItem[(nCount*3)+2]->DrawText( CASHSHOP_RENDER_BIGICON_START_X + m_nShopMovePosX + 295, 
					// 									 CASHSHOP_RENDER_FONT_ICONINFO_Y + m_nShopMovePosY + (CASHSHOP_RENDER_FONT_ICONINFO_GAB*nCount) + (CASHSHOP_FONT_LINE_HEIGHT*2),
					// 									 GUI_FONT_COLOR, buf, 0L);
					// 							
					// 							if( pShopInfo->IsSoldOutShopItem() )
					// 							{
					// 								m_pSoldOut_Item->Move( CASHSHOP_RENDER_BIGICON_START_X + m_nShopMovePosX - 2,
					// 									CASHSHOP_RENDER_BIGICON_START_Y + m_nShopMovePosY - 3 + (CASHSHOP_RENDER_BIGICON_H*nCount) );
					// 								m_pSoldOut_Item->Render();
					// 							}
					// 						}
				}
				nRenderWTab++;
				itCash++;
			}
		}
#else
		// ������ ����
		for (int i = 0; i < CASHSHOP_SHOW_ICON_MAX; i++)
		{
			if (pt.y > CASHSHOP_ITEM_SELECT_RECT_Y + m_nShopMovePosY &&
				pt.y < CASHSHOP_ITEM_SELECT_RECT_Y + m_nShopMovePosY + CASHSHOP_ITEM_SELECT_RECT_H + (i*CASHSHOP_ITEM_SELECT_RECT_H) &&
				pt.x > CASHSHOP_ITEM_SELECT_RECT_X + m_nShopMovePosX &&
				pt.x < CASHSHOP_ITEM_SELECT_RECT_X + m_nShopMovePosX + CASHSHOP_ITEM_SELECT_RECT_W)
			{
				// 2009-01-28 by bhsohn ĳ���� ó��					
				int nSelTab = GetTabToItemIdx(m_nCurrentSelectTab);

				//if(m_vecImgCashIcon[m_nCurrentSelectTab].size() > (i+m_pScroll->GetCurrentScrollIndex()))
				if (nSelTab != -1 && m_vecImgCashIcon[nSelTab].size() > (i + m_pScroll->GetCurrentScrollIndex()))
				{
					m_nItemSelect = i;
					// 2009-01-28 by bhsohn ĳ���� ó��
					//if(m_nCurrentSelectTab == CASHSHOP_ITEM_SKIN_ITEM)
					if (nSelTab != -1 && nSelTab == GetSkinItemIndex())
					{
						OnChangeWearItem();
					}
					// 2009-01-28 by bhsohn ĳ���� ó��
					UpdateGiftButton(nSelTab, m_nItemSelect);
					// end 2009-01-28 by bhsohn ĳ���� ó��
				}
				return INF_MSGPROC_BREAK;
			}
		}
#endif
		// �� ����
		for (i = 0; i < CASH_SHOP_VIEW_TAB; i++)
		{
			// 2008-10-28 by bhsohn ĳ�� ���� ���� ����
// 				if(m_nShopTapEndPosX[i] < 0)
// 				{
// 					continue;
// 				}
				// end 2008-10-28 by bhsohn ĳ�� ���� ���� ����
				// 2009-01-28 by bhsohn ĳ���� ó��
			int nSelTab = GetTabToItemIdx(i);
			if (nSelTab == -1)
			{
				continue;
			}
			else if (m_vecItemInfo[nSelTab].empty())
			{
				continue;
			}
			// end 2009-01-28 by bhsohn ĳ���� ó��
			if (pt.y > CASHSHOP_ITEM_TAB_Y + m_nShopMovePosY &&
				pt.y < CASHSHOP_ITEM_TAB_Y + m_nShopMovePosY + CASHSHOP_ITEM_TAB_H &&
				pt.x > CASHSHOP_ITEM_TAB_X + m_nShopMovePosX &&
				//					pt.x < CASHSHOP_ITEM_TAB_X+m_nShopMovePosX + CASHSHOP_ITEM_TAB_W+(i*CASHSHOP_ITEM_TAB_W))
				pt.x < CASHSHOP_ITEM_TAB_X + m_nShopMovePosX + m_nShopTapEndPosX[i])
			{
				// 2009-01-28 by bhsohn ĳ���� ó��
				m_nCurrentSelectTab = i;
				// 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
#ifdef C_CASHSHOP_IMAGE_REMOVE
					//
#else
				if (m_pImgGiftButton)
				{
					// 2007-11-14 by dgwoo ����� ���̳� ��Ų ���ϰ�� �����ϱ� ��ư�� ��Ȱ��ȭ �Ѵ�.
					// 2009-01-28 by bhsohn ĳ���� ó��
// 						if(m_nCurrentSelectTab == CASHSHOP_ITEM_PREMIUM_CARD ||
// 							m_nCurrentSelectTab == CASHSHOP_ITEM_SKIN_ITEM) 
// 						{
// 							m_pImgGiftButton->EnableBtn(FALSE);
// 						}else
// 						{
// 							m_pImgGiftButton->EnableBtn(TRUE);
// 						}
						// end 2009-01-28 by bhsohn ĳ���� ó��
				}
#endif
				// end 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
				m_nItemSelect = 0;
				// 2009-01-28 by bhsohn ĳ���� ó��
				int nSelTab = GetTabToItemIdx(m_nCurrentSelectTab);
				// end 2009-01-28 by bhsohn ĳ���� ó��

				if (nSelTab != -1)
				{
					m_pScroll->Reset();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
					int temp = m_vecImgCashIcon[nSelTab].size() / CASHSHOP_TAB_W_SIZE;
					if (m_vecImgCashIcon[nSelTab].size() % CASHSHOP_TAB_W_SIZE != 0)
					{
						temp += 1;
					}
					m_pScroll->SetNumberOfData(temp + 1);
#else
					m_pScroll->SetNumberOfData(m_vecImgCashIcon[nSelTab].size());
#endif
				}

				// 2007-08-10 by cmkwon, �Ƹ� �÷� ���� ��� �÷��� �������ش�
				// 2009-01-28 by bhsohn ĳ���� ó��
				//if(m_nCurrentSelectTab == CASHSHOP_ITEM_SKIN_ITEM)
				if (nSelTab != -1 && nSelTab == GetSkinItemIndex())
				{
					OnChangeWearItem();
				}
				// 2009-01-28 by bhsohn ĳ���� ó��
				// �����ϱ� ��ư ����
				UpdateGiftButton(nSelTab, m_nItemSelect);
				// end 2009-01-28 by bhsohn ĳ���� ó��
				return INF_MSGPROC_BREAK;
			}
		}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM 
		if (m_pCloseBtn->OnLButtonDown(pt))
#else
		// �ݱ� ��ư Ŭ��
		if (pt.y > CASHSHOP_ITEM_CLOSE_Y + m_nShopMovePosY &&
			pt.y < CASHSHOP_ITEM_CLOSE_Y + m_nShopMovePosY + CASHSHOP_ITEM_CLOSE_BUTTON_H &&
			pt.x > CASHSHOP_ITEM_CLOSE_X + m_nShopMovePosX &&
			pt.x < CASHSHOP_ITEM_CLOSE_X + m_nShopMovePosX + CASHSHOP_ITEM_CLOSE_BUTTON_W)
#endif
		{
			g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
		}

		// â�̵�
// 2007-08-10 by dgwoo ĳ�� ���� �̵� �Ұ�
//			if( pt.y > CASHSHOP_RENDER_START_Y+m_nShopMovePosY && 
//				pt.y < CASHSHOP_RENDER_START_Y+m_nShopMovePosY+CASHSHOP_ITEM_SHOP_MOVE_H &&
//				pt.x > CASHSHOP_RENDER_START_X+m_nShopMovePosX && 
//				pt.x < CASHSHOP_RENDER_START_X+m_nShopMovePosX + CASHSHOP_ITEM_SHOP_MOVE_W)
//			{
//				m_pointBeforeMousePoints	= pt;
//				m_nShopOldPosX				= m_nShopMovePosX;
//				m_nShopOldPosY				= m_nShopMovePosY;
//				
//				m_bMoveShop					= TRUE;				
//			}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM 
		if (pt.x >= CASHSHOP_RENDER_START_X + m_nShopMovePosX && pt.x <= CASHSHOP_RENDER_START_X + m_nShopMovePosX + (m_pImgBack->GetMaxPos().x - m_pImgBack->GetMinPos().x) &&
			pt.y >= CASHSHOP_RENDER_START_Y + m_nShopMovePosY && pt.y <= CASHSHOP_RENDER_START_Y + m_nShopMovePosY + (m_pImgBack->GetMaxPos().y - m_pImgBack->GetMinPos().y))
		{
			return INF_MSGPROC_BREAK;
		}
#else
			// �簢 ������ ������ Ŭ�� ����
		if (pt.x >= CASHSHOP_RENDER_START_X + m_nShopMovePosX && pt.x <= CASHSHOP_RENDER_START_X + m_nShopMovePosX + CASHSHOP_ITEM_SHOP_MOVE_W &&
			pt.y >= CASHSHOP_RENDER_START_Y + m_nShopMovePosY && pt.y <= CASHSHOP_RENDER_START_Y + m_nShopMovePosY + 397)
		{
			return INF_MSGPROC_BREAK;
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

		// 2007-10-16 by bhsohn �ɽ��� ������ư ����
//			if( pt.y > CASHSHOP_RENDER_BUY_BUTTON_Y+m_nShopMovePosY && 
//				pt.y < CASHSHOP_RENDER_BUY_BUTTON_Y+m_nShopMovePosY + CASHSHOP_RENDER_BUY_BUTTON_H &&
//				pt.x > CASHSHOP_RENDER_BUY_BUTTON_X+m_nShopMovePosX && 
//				pt.x < CASHSHOP_RENDER_BUY_BUTTON_X+m_nShopMovePosX + CASHSHOP_RENDER_BUY_BUTTON_W &&
//				m_bCurrentMouseStateBuy)
//			{
//				
//				// ���� ������
//				SendChargeCash();
//			}
//			else 
		if (pt.y > CASHSHOP_RENDER_CHARGE_BUTTON_Y + m_nShopMovePosY &&
			pt.y < CASHSHOP_RENDER_CHARGE_BUTTON_Y + m_nShopMovePosY + CASHSHOP_RENDER_CHARGE_BUTTON_H &&
			pt.x > CASHSHOP_RENDER_CHARGE_BUTTON_X + m_nShopMovePosX &&
			pt.x < CASHSHOP_RENDER_CHARGE_BUTTON_X + m_nShopMovePosX + CASHSHOP_RENDER_CHARGE_BUTTON_W &&
			m_bCurrentMouseStateCharge)
		{
			// ���� ������
			if (m_pItem != NULL)
			{
				// 2008-08-13 by dgwoo ���� �޽���â�� ��������쿣 ��ũ�� �� �ٸ� ��Ʈ���� �ȵǵ��� ����.
				g_pGameMain->m_bBuyMessage = TRUE;
				SendBuyItem(m_pItem);
			}
		}
		// 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
#ifdef C_CASHSHOP_IMAGE_REMOVE
			//
#else
			// 2007-11-14 by dgwoo ���� ��ư Ŭ��.
		if (m_pImgGiftButton &&
			m_pImgGiftButton->OnLButtonUp(pt))
		{
			// 2013-07-09 by bhsohn 70 ���� �̻� �����ϱ� �����ϰ� ����
#ifdef C_LEAST_LV_GIFT
			const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
			if (myShuttleInfo.Level < 70)
			{
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_130708_0001, COLOR_SYSTEM, CHAT_TAB_SYSTEM);
				return INF_MSGPROC_BREAK;
			}
#endif
			// END 2013-07-09 by bhsohn 70 ���� �̻� �����ϱ� �����ϰ� ����
			// 2008-08-13 by dgwoo ���� �޽���â�� ��������쿣 ��ũ�� �� �ٸ� ��Ʈ���� �ȵǵ��� ����.
			if (m_pItem) // 2013-05-16 by bhsohn �����ϱ� Exeption �����ڵ� �߰�
			{
				g_pGameMain->m_bBuyMessage = TRUE;
				g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_071115_0100, _Q_GIFT_CHARACTER_TEXT, 0, 0, 0, m_pItem->ItemNum);
				return INF_MSGPROC_BREAK;
			}
		}
#endif
		// end 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
		m_bMoveShop = FALSE;
		m_bCurrentMouseStateCharge = FALSE;
		m_bCurrentMouseStateBuy = FALSE;
		m_nCurrentMouseStateCharge = CASHSHOP_BUTTON_STATE_NOM;
		m_nCurrentMouseStateBuy = CASHSHOP_BUTTON_STATE_NOM;
	}
	break;
	}
	return INF_MSGPROC_NORMAL;
}
void CINFCityCashShop::RenderDescIcon()
{
	// 2006-06-08 by ispark, �������� ����
	char buff[CASH_SHOP_ITEM_INFO_DES][128];
	memset(buff, 0x00, CASH_SHOP_ITEM_INFO_DES * 128);
	//	char *token;
	//	char seps[] = "$";
	//	char buffer[1024];
	vector<string> m_vecCashDesc;

	if (m_pItem)
	{
		//		strncpy(buffer, m_pItem->Description, sizeof(m_pItem->Description));
		STRING_CULL(m_pItem->Description, CASHSHOP_ITEM_DESC_MAX, &m_vecCashDesc, m_pFontItemInfo[2]);
		
		int i = 0;
		vector<string>::iterator it = m_vecCashDesc.begin();
		while (it != m_vecCashDesc.end())
		{
			m_pFontItemInfo[i + 2]->DrawText(CASHSHOP_ITEM_INFO_NAME_X + m_nShopMovePosX - 80,
				CASHSHOP_ITEM_INFO_NAME_Y + m_nShopMovePosY + CASHSHOP_FONT_LINE_HEIGHT + (CASHSHOP_FONT_LINE_HEIGHT*(i + 1)) + 23,
				GUI_FONT_COLOR, (char*)(*it).c_str(), 0L);

			i++;
			it++;
		}

				// 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
		SHOP_ITEM* pShopInfo = FindCashShopData(m_nCurrentSelectTab, m_pItem->ItemNum);
		if (pShopInfo && pShopInfo->IsLimitedEditionShopItem() && pShopInfo->IsSoldOutShopItem())
		{
			m_pSoldOut_ToolTip->Move(CASHSHOP_RENDER_START_X + 467, CASHSHOP_RENDER_START_Y + 41);
			m_pSoldOut_ToolTip->Render();
		}
		// end 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
	}
}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
void CINFCityCashShop::RenderDescIcon(POINT pPos, ITEM* pItem, int nWidht, int nHeight)
{
	char buff[CASH_SHOP_ITEM_INFO_DES][128];
	memset(buff, 0x00, CASH_SHOP_ITEM_INFO_DES * 128);
	int nW = 0, nH = 0;
	int nFinalW, nFinalH;
	vector<string> m_vecCashDesc;
	if (m_pItem)
	{
		char buf[512];
		memset(buf, 0x00, 512);

		char buf2[512];
		memset(buf2, 0x00, 512);

		char buf2_1[512];
		memset(buf2_1, 0x00, 512);

		char buf3[512];
		memset(buf3, 0x00, 512);
		nH += RENDER_TOOLTIP_TOP_GAB;

		// 2013-01-02 by mspark, ĳ�� �������� ������ ��ȣ ���
		char bufItemInfo[512] = { '\0', };
		memset(bufItemInfo, 0x00, 512);
		// end 2013-01-02 by mspark, ĳ�� �������� ������ ��ȣ ���

		SIZE Size = m_pFontItemInfo[0]->GetStringSize(pItem->ItemName);
		nW = max(nW, Size.cx);
		nH += Size.cy;

		wsprintf(buf, STRMSG_C_SHOP_0001, pItem->MinTradeQuantity);//"%d��/1��"
#ifdef SHOP_PRICES_PER_BUILDING_NPC
		// Get Tab Information to acquire the Shop Information
		int nSelTab = GetTabToItemIdx(m_nCurrentSelectTab);
		SHOP_ITEM* pShopInfo = NULL;
		if (nSelTab != -1)
		{
			// Get the Shop Information for the selected Item
			pShopInfo = FindCashShopData(nSelTab, pItem->ItemNum);
			if (pShopInfo)
			{
				wsprintf(buf2_1, "%d", (int)(pShopInfo->Price * CASHSHOP_TEX));
			}
		}
		if (!pShopInfo)
		{
			DBGOUT("ERROR: CINFCityCashShop::RenderDescIcon, could not render the Description Icon properly!");
			wsprintf(buf2_1, "%d", 0x7fffffff);
		}
#else
		wsprintf(buf2_1, "%d", (int)(pItem->CashPrice * CASHSHOP_TEX));
#endif // SHOP_PRICES_PER_BUILDING_NPC


		MakeCurrencySeparator(buf2, buf2_1, 3, ',');
		if (pItem->ReqMaxLevel == 0)
		{
			wsprintf(buf3, "\\q[Level %2.d]", pItem->ReqMinLevel);
		}
		else
		{
			wsprintf(buf3, "\\q[Level %2.d~%2.d]", pItem->ReqMinLevel, pItem->ReqMaxLevel);
		}

		wsprintf(buf, "%s %s %s %s", buf, buf2, STRMSG_C_SHOP_0002, buf3);// ĳ��

		Size = m_pFontItemInfo[1]->GetStringSize(buf);

		nW = max(nW, Size.cx);
		nH += Size.cy;

		nH += RENDER_TOOLTIP_CEN_GAB;

		STRING_CULL(pItem->Description, CASHSHOP_ITEM_DESC_MAX, &m_vecCashDesc, m_pFontItemInfo[1]);

		int i;
		for (i = 0; i < m_vecCashDesc.size(); i++)
		{
			Size = m_pFontItemInfo[1]->GetStringSize(((char*)m_vecCashDesc[i].c_str()));
			nW = max(nW, Size.cx);
			nH += Size.cy;
		}

		nW += RENDER_TOOLTIP_LEFT_GAB + RENDER_TOOLTIP_RIGHT_GAB;
		nH += RENDER_TOOLTIP_BOT_GAB;

		nFinalW = nW;
		nFinalH = nH;

		((CINFGameMain*)m_pParent)->m_pInfWindow->RenderCenterWindow(pPos.x, pPos.y, nFinalW, nFinalH, FALSE);

		nH = 0;
		nH += RENDER_TOOLTIP_TOP_GAB;

		//Size = m_pFontItemInfo[0]->GetStringSize( pItem->ItemName );
		//m_pFontItemInfo[0]->DrawText( pPos.x + ( nFinalW / 2 ) - ( Size.cx / 2 ), pPos.y + nH, RGB(255, 255, 255), pItem->ItemName, 0L);
		// 2013-01-02 by mspark, ĳ�� �������� ������ ��ȣ ���
		const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();

		if (COMPARE_RACE(myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
		{
			wsprintf(bufItemInfo, "%s (Idx:%d)", pItem->ItemName, pItem->ItemNum);
			Size = m_pFontItemInfo[0]->GetStringSize(bufItemInfo);
			m_pFontItemInfo[0]->DrawText(pPos.x + (nFinalW / 2) - (Size.cx / 2), pPos.y + nH, RGB(255, 255, 255), bufItemInfo, 0L);
		}
		else
		{
			Size = m_pFontItemInfo[0]->GetStringSize(pItem->ItemName);
			m_pFontItemInfo[0]->DrawText(pPos.x + (nFinalW / 2) - (Size.cx / 2), pPos.y + nH, RGB(255, 255, 255), pItem->ItemName, 0L);
		}
		// end 2013-01-02 by mspark, ĳ�� �������� ������ ��ȣ ���
		nH += Size.cy;

		wsprintf(buf, STRMSG_C_SHOP_0001, pItem->MinTradeQuantity);//"%d��/1��"
#ifdef SHOP_PRICES_PER_BUILDING_NPC
		if (pShopInfo)
		{
			wsprintf(buf2_1, "%d", (int)(pShopInfo->Price * CASHSHOP_TEX));
		}
		else
		{
			DBGOUT("ERROR: CINFCityCashShop::RenderDescIcon, could not render the Description Icon properly!");
			wsprintf(buf2_1, "%d", 0x7fffffff);
		}
#else
		wsprintf(buf2_1, "%d", (int)(pItem->CashPrice * CASHSHOP_TEX));
#endif // SHOP_PRICES_PER_BUILDING_NPC


		MakeCurrencySeparator(buf2, buf2_1, 3, ',');

		if (pItem->ReqMaxLevel == 0)
		{
			wsprintf(buf3, "\\q[Level %2.d]", pItem->ReqMinLevel);
		}
		else
		{
			wsprintf(buf3, "\\q[Level %2.d~%2.d]", pItem->ReqMinLevel, pItem->ReqMaxLevel);
		}
		wsprintf(buf, "%s %s %s %s", buf, buf2, STRMSG_C_SHOP_0002, buf3);// ĳ��
		Size = m_pFontItemInfo[1]->GetStringSize(buf);
		m_pFontItemInfo[1]->DrawText(pPos.x + (nFinalW / 2) - (Size.cx / 2), pPos.y + nH, RGB(255, 255, 255), buf, 0L);

		nH += Size.cy;

		nH += RENDER_TOOLTIP_CEN_GAB;

		for (i = 0; i < m_vecCashDesc.size(); i++)
		{
			Size = m_pFontItemInfo[1]->GetStringSize(((char*)m_vecCashDesc[i].c_str()));

			m_pFontItemInfo[1]->DrawText(pPos.x + (nFinalW / 2) - (Size.cx / 2), pPos.y + nH, RGB(255, 255, 255), ((char*)m_vecCashDesc[i].c_str()), 0L);
			nH += Size.cy;
		}
	}
}
#endif

void CINFCityCashShop::SendBuyItem(ITEM *pItem)
{
	// ������ ���
	char buf[512];
	memset(buf, 0x00, 512);
	wsprintf(buf, STRMSG_C_SHOP_0006, pItem->ItemName);//"%s �������� ���� �Ͻðڽ��ϱ�?"
	m_nGiveTargetCharacterUID = 0;

	//void AddMsgBox(char* strMsg, int nType, DWORD dwData1=0, DWORD dwData2=0, int nLiveTime=0,UID64_t UniqueNumber=0, char* strName=NULL);
	// 2007-10-16 by bhsohn Ư�� �۾��� ����� ó��
	//g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_CASHITEM_BUY);	
	g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_CASHITEM_BUY, 0, 0, 0, 0, pItem->ItemName);
}
void CINFCityCashShop::SendChargeCash()
{
	// 2008-04-30 by bhsohn �±� ���� �߰�
//#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM) || defined(YEDANG_RELEASE)|| defined(LANGUAGE_CHINA) 
//	// 2006-06-23 by ispark, ���� Ȩ������ �޼��� �ڽ� ���
//	g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_060623_0000, _MESSAGE);
//
//#else
//	// ĳ�� �����ϱ�
//	g_pD3dApp->ShowWebWindow();
//#endif
	// end 2008-04-30 by bhsohn �±� ���� �߰�
}

struct CompareKindCashShop
{
	template<typename T>
	bool operator()(const T pPtr1, const T pPtr2)
	{
		ITEM* pITEM1 = g_pDatabase->GetServerItemInfo(pPtr1.nItemNum);
		ITEM* pITEM2 = g_pDatabase->GetServerItemInfo(pPtr2.nItemNum);
		if (pITEM1 == NULL || pITEM2 == NULL)
		{
			return false;
		}
		if (pITEM1->Kind < pITEM2->Kind)
		{
			return true;
		}
		else if (pITEM1->Kind == pITEM2->Kind)
		{
			if (pITEM1->ReqMinLevel < pITEM2->ReqMinLevel)
			{
				return true;
			}
			else if (pITEM1->ReqMinLevel == pITEM2->ReqMinLevel)
			{
				if (pITEM1->ItemNum < pITEM2->ItemNum)
				{
					return true;
				}
			}
		}
		return false;
	}
};

void CINFCityCashShop::CashItemSort()
{
	for (int i = 0; i < MAX_CASH_SHOP_TAB; i++)
	{
		sort(m_vecImgCashIcon[i].begin(), m_vecImgCashIcon[i].end(), CompareKindCashShop());
	}
}

// 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
void	CINFCityCashShop::UpdateCashItemRemainCount(ItemNum_t nItemNum, INT nRemainCount)
{
	for (int i = 0; i < MAX_CASH_SHOP_TAB; ++i)
	{
		SHOP_ITEM* pItem = FindCashShopData(i, nItemNum);
		if (pItem)
			pItem->RemainCountForLimitedEdition = nRemainCount;
	}
}
// end 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCityCashShop::FindCashShopData(int nTabIndex, SHOP_ITEM *pCashShop)
/// \brief		ĳ�� ����Ÿ �˻�
/// \author		ispark
/// \date		2006-01-11 ~ 2006-01-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009-01-28 by bhsohn ĳ���� ó��
SHOP_ITEM* CINFCityCashShop::FindCashShopData(int nTabIndex, UINT ItemNum)
{
	vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[nTabIndex].begin();
	while (it != m_vecItemInfo[nTabIndex].end())
	{
		SHOP_ITEM* pitCashShop = *it;
		if (ItemNum == pitCashShop->ItemNum)
		{
			return pitCashShop;
		}

		it++;
	}

	return NULL;
}
void CINFCityCashShop::InputCashColorShopItem()
{
	// 2009-01-28 by bhsohn ĳ���� ó��
	// Į����� ���ý� Ŭ���̾�Ʈ ������ �˻��� ������ ����
// 	vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[CASHSHOP_ITEM_SKIN_ITEM].begin();
// 	while(it != m_vecItemInfo[CASHSHOP_ITEM_SKIN_ITEM].end())
// 	{
// 		SHOP_ITEM* pItem = *it;
// 		delete pItem;
// 		pItem = NULL;
// 		it++;
// 	}
// 	m_vecItemInfo[CASHSHOP_ITEM_SKIN_ITEM].clear();
// 	CashItemIcon pTemp;
// 	CItemInfo *pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos(POS_CENTER);
// 	if(pItemInfo)
// 	{
// 		for(int i=1; i <= g_pD3dApp->GetUnitArmorColorMax(); i++)
// 		{			
// 			ITEM *item = g_pDatabase->GetServerItemInfo(SOURCEINDEXTOITEMNUM(pItemInfo->ItemInfo->SourceIndex, i));
// 			if(item)
// 			{
// 				if(COMPARE_BIT_FLAG(item->ItemAttribute, ITEM_ATTR_CASH_ITEM))
// 				{
// 					SHOP_ITEM* pItemShop = new SHOP_ITEM;
// 					pItemShop->ItemNum							= item->ItemNum;
// 					strcpy(pItemShop->ItemName, item->ItemName);
// 					pItemShop->MinTradeQuantity					= item->MinTradeQuantity;
// 					pItemShop->Price							= item->CashPrice;
// 					pItemShop->ItemKind							= item->Kind;
// 					m_vecItemInfo[CASHSHOP_ITEM_SKIN_ITEM].push_back(pItemShop);
// 
// 					pTemp.nItemNum = item->ItemNum;
// 					
// 					// 2007-08-21 by bhsohn ĳ�� ������ alt+tab���� ����
// 					//pTemp.vecImgCashIcon = g_pGameMain->m_pItemInfo->FindBigIcon(item->SourceIndex);
// 					pTemp.pDataHeader = NULL;
// 					pTemp.vecImgCashIcon = FindCashIcon(item->SourceIndex, pTemp.pDataHeader);
// 					
// 					m_vecImgCashIcon[CASHSHOP_ITEM_SKIN_ITEM].push_back(pTemp);
// 				}
// 			}
// 		}
// 	}	
	// CASH_ITEMKIND_COLOR_ITEM(9) ���ʹ� ������ �ƸӰ� �ִ�.
	// �ִ� ���ͼ��� 10���̹Ƿ� 9���� � ���� ����.
#ifndef C_EPSODE4_UI_CHANGE_JSKIM
	ReLoadSkinItemVector();
#endif

	RefreshGetSkinItemIndex();	// ���� ���� ����		
	int nVecPos = 0;	// ���� �տ��ִ� ������ ũ��
	if (m_vecImgCashIcon[nVecPos].size() == 0)
	{
		nVecPos = GetNonEmptyNextIndex(0);	// ���� �տ��ִ� ������ ũ��
	}
	// 2009-03-02 by bhsohn ĳ���� ���� ����
	m_pScroll->Reset();
	// end 2009-03-02 by bhsohn ĳ���� ���� ����
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	int temp = m_vecImgCashIcon[nVecPos].size() / CASHSHOP_TAB_W_SIZE;
	if (m_vecImgCashIcon[nVecPos].size() % CASHSHOP_TAB_W_SIZE != 0)
	{
		temp += 1;
	}
	m_pScroll->SetNumberOfData(temp);
#else
	m_pScroll->SetNumberOfData(m_vecImgCashIcon[nVecPos].size());
	// end 2009-01-28 by bhsohn ĳ���� ó��
#endif

	// 2009-03-02 by bhsohn ĳ���� ���� ����
	{
		// ������ �ޱ� �Ϸ�
		int nSelTab = GetTabToItemIdx(m_nCurrentSelectTab);
		if (nSelTab != -1)
		{
			UpdateGiftButton(nSelTab, m_nItemSelect);
		}
	}
	// end 2009-03-02 by bhsohn ĳ���� ���� ����
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFCityCashShop::RefreshGetSkinItemIndex()
/// \brief		
/// \author		// 2009-01-28 by bhsohn ĳ���� ó��
/// \date		
/// \warning	2015-05-28 Future, function is no longer in use since ep 4
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityCashShop::ReLoadSkinItemVector()
{
	if (m_vecItemInfo[CASH_ITEMKIND_COLOR_ITEM].empty())
	{
		// �Ƹ� ������ ������ ������ ���Ѵ�.
		return;
	}
	vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[CASH_ITEMKIND_COLOR_ITEM].begin();
	while (it != m_vecItemInfo[CASH_ITEMKIND_COLOR_ITEM].end())
	{
		SHOP_ITEM* pItem = *it;
		util::del(pItem);
		it++;
	}
	m_vecItemInfo[CASH_ITEMKIND_COLOR_ITEM].clear();

	// 2009-01-28 by bhsohn ĳ���� ó��
	{
		for (auto img : m_vecImgCashIcon[CASH_ITEMKIND_COLOR_ITEM])
		{
			util::del(img.pDataHeader);

			if (img.vecImgCashIcon != NULL)
			{
				img.vecImgCashIcon->DeleteDeviceObjects();
				util::del(img.vecImgCashIcon);
			}
		}
		m_vecImgCashIcon[CASH_ITEMKIND_COLOR_ITEM].clear();
	}
	// end 2009-01-28 by bhsohn ĳ���� ó��

	CashItemIcon pTemp;
	CItemInfo *pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos(POS_CENTER);
	if (pItemInfo)
	{
		for (int i = 1; i <= g_pD3dApp->GetUnitArmorColorMax(); i++)
		{
			ITEM *item = g_pDatabase->GetServerItemInfo(SOURCEINDEXTOITEMNUM(pItemInfo->ItemInfo->SourceIndex, i));
			if (item)
			{
				if (COMPARE_BIT_FLAG(item->ItemAttribute, ITEM_ATTR_CASH_ITEM))
				{
					SHOP_ITEM* pItemShop = new SHOP_ITEM;
					pItemShop->ItemNum = item->ItemNum;
					strcpy(pItemShop->ItemName, item->ItemName);
					pItemShop->MinTradeQuantity = item->MinTradeQuantity;
#ifdef SHOP_PRICES_PER_BUILDING_NPC
#pragma message ("CINFCityCashShop::ReLoadSkinItemVector may show Invalid Prices if compiled with SHOP_PRICES_PER_BUILDING_NPC and not with C_EPSODE4_UI_CHANGE_JSKIM")
					DBGOUT("WARNING: CINFCityCashShop::ReLoadSkinItemVector may show Invalid Prices\
						   due to the Shop Prices by BNPC change!!! Do not use this function anymore");
					pItemShop->Price = 0x7fffffff;
#else
					pItemShop->Price = item->CashPrice;
#endif	// SHOP_PRICES_PER_BUILDING_NPC
					pItemShop->ItemKind = item->Kind;
					m_vecItemInfo[CASH_ITEMKIND_COLOR_ITEM].push_back(pItemShop);

					pTemp.nItemNum = item->ItemNum;

					// ĳ�� ������ alt+tab���� ����					
					pTemp.pDataHeader = NULL;
					pTemp.vecImgCashIcon = FindCashIcon(item->SourceIndex, pTemp.pDataHeader);

					m_vecImgCashIcon[CASH_ITEMKIND_COLOR_ITEM].push_back(pTemp);
				}
			}
		}
	}

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::LoadNPCImage(int nNPCIndex)
/// \brief		
/// \author		// 2007-08-21 by bhsohn ĳ�� ������ alt+tab���� ����
/// \date		
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CINFImageEx* CINFCityCashShop::FindCashIcon(int nItemNum, DataHeader*	pDataHeader)
{

	util::del(pDataHeader);
	CGameData gameData;
	gameData.SetFile(".\\Res-Tex\\bigitem.tex", FALSE, NULL, 0, FALSE);
	char szName[32];
	wsprintf(szName, "%08d", nItemNum);
	pDataHeader = gameData.FindFromFile(szName);
	if (pDataHeader == NULL)
	{
		return NULL;
	}

	CINFImageEx *pImage = new CINFImageEx;
	pImage->InitDeviceObjects(pDataHeader);
	pImage->RestoreDeviceObjects();
	return pImage;

}
void CINFCityCashShop::SendMsgGift(MSG_FC_SHOP_CHECK_GIVE_TARGET_OK* pMsg)
{
	char buf[SIZE_MAX_STRING_256];
	wsprintf(buf, STRMSG_C_071115_0101, m_pItem->ItemName);
	m_nGiveTargetCharacterUID = pMsg->GiveTargetCharUID;
	g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_GIFT_ITEM, 0, 0, 0, 0, m_pItem->ItemName);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::LoadNPCImage(int nNPCIndex)
/// \brief		
/// \author		// 2009-01-28 by bhsohn ĳ���� ó��
/// \date		
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityCashShop::UpdateGiftButton(int i_nCurrentSelectTab, int i_nItemSelect)
{
	// 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
#ifdef C_CASHSHOP_IMAGE_REMOVE
	return;
#else
	if (NULL == m_pImgGiftButton)
	{
		return;
	}
	//int nSelectTotalIdx = i_nItemSelect + m_pScroll->GetCurrentScrollIndex();
	int nSelectTotalIdx = i_nItemSelect + (m_pScroll->GetCurrentScrollIndex() * CASHSHOP_TAB_W_SIZE);	// 2013-07-30 by ssjung ĳ���?�����ϱ� ���� �̽� �ذ� 

	if ((i_nCurrentSelectTab < 0)
		|| (i_nCurrentSelectTab >= (MAX_CASH_SHOP_TAB - 1))
		|| (m_vecImgCashIcon[i_nCurrentSelectTab].size() <= nSelectTotalIdx))
	{
		// �ʰ� ����
		m_pImgGiftButton->EnableBtn(FALSE);
		return;
	}
	else if (i_nCurrentSelectTab == GetSkinItemIndex())
	{
		// ��Ų��
		m_pImgGiftButton->EnableBtn(FALSE);
		return;
	}

	SHOP_ITEM* pShotItem = m_vecItemInfo[i_nCurrentSelectTab][nSelectTotalIdx];
	if (!pShotItem)
	{
		m_pImgGiftButton->EnableBtn(FALSE);
		return;
	}

	BOOL bEnableGift = TRUE;
	if (pShotItem->ItemNum == 0)
	{
		bEnableGift = FALSE;
	}
	else
	{
		ITEM *pSelItem = g_pDatabase->GetServerItemInfo(pShotItem->ItemNum);
		if (pSelItem)
		{
			if (COMPARE_BIT_FLAG(pSelItem->ItemAttribute, ITEM_ATTR_CASH_ITEM_PREMIUM_CARD))
			{
				// �����̾� ī��� �����ϱ� �Ұ�
				bEnableGift = FALSE;
			}
		}
	}
	m_pImgGiftButton->EnableBtn(bEnableGift);
#endif
	// end 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFCityCashShop::RefreshGetSkinItemIndex()
/// \brief		
/// \author		// 2009-01-28 by bhsohn ĳ���� ó��
/// \date		
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityCashShop::RefreshGetSkinItemIndex()
{
	// ������ �Ѵ�.	
	int nTapIdx = 0;
	{
		int nNextTap = 0;
		int nTap = 0;
		int nCnt = 0;
		BOOL bSelIdx[MAX_CASH_SHOP_TAB];
		for (nTap = 0; nTap < (MAX_CASH_SHOP_TAB); nTap++)
		{
			m_nTabToItemIdx[nTap] = -1;
			bSelIdx[nTap] = FALSE;
		}

		for (nTap = 0; nTap < (MAX_CASH_SHOP_TAB - 1); nTap++)
		{
			if (bSelIdx[nTap])
			{
				// �̹� ������ ������ 
				continue;
			}
			if (m_vecItemInfo[nTap].size() > 0)
			{
				m_nTabToItemIdx[nTapIdx] = nTap;
				bSelIdx[nTap] = TRUE;		// ���õǾ���.
				nTapIdx++;
			}
			else
			{
				// ���� ���� 0�̸� ������ 0�� �ƴ� ���� ã�´�.
				nNextTap = GetNonEmptyNextIndex(nTap);
				if (nTap != nNextTap && !bSelIdx[nNextTap])	// �ѹ��� �ȼ����� ���̴�.
				{
					//SwapItemVector(nTap, nNextTap); // ������ �̵��� �ʿ䰡 ����.
					m_nTabToItemIdx[nTapIdx] = nNextTap;
					bSelIdx[nNextTap] = TRUE;		// ���õǾ���.
					nTapIdx++;
				}
			}
		}
	}
	m_nSkinItemIdx = -2;	//
#ifndef C_EPSODE4_UI_CHANGE_JSKIM
	// �� ���͸� ã�´�.	
	if (m_vecItemInfo[CASH_ITEMKIND_COLOR_ITEM].size() > 0)	// ���� ��Ų ���Ϳ� ���� �־���.
	{
		m_nSkinItemIdx = CASH_ITEMKIND_COLOR_ITEM;
	}
#endif
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFCityCashShop::RefreshGetSkinItemIndex()
/// \brief		
/// \author		// 2009-01-28 by bhsohn ĳ���� ó��
/// \date		
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFCityCashShop::GetSkinItemIndex()
{
	return m_nSkinItemIdx;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFCityCashShop::RefreshGetSkinItemIndex()
/// \brief		
/// \author		// 2009-01-28 by bhsohn ĳ���� ó��
/// \date		
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFCityCashShop::GetNonEmptyNextIndex(int i_nStartIdx)
{
	int nNonEmptyIdx = i_nStartIdx;
	int nTap = 0;
	for (nTap = (i_nStartIdx + 1); nTap < (MAX_CASH_SHOP_TAB - 1); nTap++)
	{
		if (m_vecItemInfo[nTap].size() > 0)
		{
			// ũ�Ⱑ 0�� �ƴ� ���� �����Ѵ�.
			nNonEmptyIdx = nTap;
			break;
		}

	}
	return nNonEmptyIdx;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFCityCashShop::RefreshGetSkinItemIndex()
/// \brief		
/// \author		// 2009-01-28 by bhsohn ĳ���� ó��
/// \date		
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFCityCashShop::GetTabToItemIdx(int i_nIdx)
{
	if (i_nIdx < 0 || i_nIdx >= MAX_CASH_SHOP_TAB)
	{
		return -1;
	}
	return m_nTabToItemIdx[i_nIdx];

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFCityCashShop::RenderNewShopInfo(int i_nPosX, int i_nPosY, SHOP_ITEM* i_pShopInfo)
/// \brief		
/// \author		// 2009-01-28 by bhsohn ĳ���� ó��
/// \date		
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityCashShop::RenderNewShopInfo(int i_nPosX, int i_nPosY, SHOP_ITEM* i_pShopInfo)
{
	m_fNewPlayTime -= g_pD3dApp->GetCheckElapsedTime();

	if (m_fNewPlayTime < 0)
	{
		m_fNewPlayTime = 1.0f;
		m_nNewPlayIndex++;
		if (m_nNewPlayIndex >= MAX_NEW_CASHSHOP_IMAGE)
		{
			m_nNewPlayIndex = 0;
		}
	}
	BYTE byCashIndex = GET_TABINDEX_BY_CASHSHOPINDEX(i_pShopInfo->CashShopIndex);
	BYTE byReNewIndex = GET_CASHSHOP_BIT_BY_CASHSHOPINDEX(i_pShopInfo->CashShopIndex);

	if (!IS_NEW_CASHSHOPITEM_BY_CASHSHOPINDEX(byReNewIndex))
	{
		// ���� �������� �ƴϴ�.
		return;
	}
	else if (NULL == m_pImgNewTab[m_nNewPlayIndex])
	{
		return;
	}
	int nPosX = i_nPosX;
	int nPosY = i_nPosY;
	m_pImgNewTab[m_nNewPlayIndex]->Move(nPosX, nPosY);
	m_pImgNewTab[m_nNewPlayIndex]->Render();
}