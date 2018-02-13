// INFWarDeclare.cpp: implementation of the CINFWarDeclare class.
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
#include "ClientParty.h"

#include "INFWarDeclare.h"
#include "INFGroupImage.h"						  // 2011. 10. 10 by jskim UI�ý��� ����
#include "INFGroupManager.h"					  // 2011. 10. 10 by jskim UI�ý��� ����
#include "INFToolTip.h"			// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����

// 2009. 01. 12 by ckPark ���� ���� �ý���

#include "CHAT.h"

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	#define MOTHERSHIP_WAR_DECLARE_BG_W			395
	#define MOTHERSHIP_WAR_DECLARE_BG_H			420

	#define MOTHERSHIP_WAR_DECLARE_BAR_W		MOTHERSHIP_WAR_DECLARE_BG_W
	#define MOTHERSHIP_WAR_DECLARE_BAR_H		20

	#define MOTHERSHIP_WAR_DECLARE_CAL_LEFT_X	283
	#define MOTHERSHIP_WAR_DECLARE_CAL_LEFT_Y	69

	#define MOTHERSHIP_WAR_DECLARE_CAL_RIGHT_X	343
	#define MOTHERSHIP_WAR_DECLARE_CAL_RIGHT_Y	69

	#define MOTHERSHIP_WAR_DECLARE_DAY_X		182
	#define MOTHERSHIP_WAR_DECLARE_DAY_Y		96

	#define MOTHERSHIP_WAR_DECLARE_DAY_RECT_X	174
	#define MOTHERSHIP_WAR_DECLARE_DAY_RECT_Y	94

	#define MOTHERSHIP_WAR_DECLARE_CAL_X		182
	#define MOTHERSHIP_WAR_DECLARE_CAL_Y		120

	#define MOTHERSHIP_WAR_DECLARE_CAL_GAP_W	26
	#define MOTHERSHIP_WAR_DECLARE_CAL_GAP_H	24

	#define MOTHERSHIP_WAR_DECLARE_TIMESELUP_X	233
	#define MOTHERSHIP_WAR_DECLARE_TIMESELUP_Y	315

	#define MOTHERSHIP_WAR_DECLARE_TIMESELDOWN_X	233
	#define MOTHERSHIP_WAR_DECLARE_TIMESELDOWN_Y	328

	#define MOTHERSHIP_WAR_DECLARE_FLAG_X		27
	#define MOTHERSHIP_WAR_DECLARE_FLAG_Y		61

	#define	MOTHERSHIP_WAR_DECLARE_GIVEUP_X		265		// 2012-06-29 by isshin UI ��ǥ ���� ( 275 -> 265 )
	#define	MOTHERSHIP_WAR_DECLARE_GIVEUP_Y		419

	#define MOTHERSHIP_WAR_DECLARE_SAVE_X		155		// 2012-06-29 by isshin UI ��ǥ ���� ( 165 -> 155 )
	#define MOTHERSHIP_WAR_DECLARE_SAVE_Y		419

	#define MOTHERSHIP_WAR_DECLARE_CLOSE_X		256		// 2012-06-29 by isshin UI ��ǥ ���� ( 266 -> 256 )
	#define MOTHERSHIP_WAR_DECLARE_CLOSE_Y		419

	#define MOTHERSHIP_WAR_DECLARE_YEAR_X		203
	#define MOTHERSHIP_WAR_DECLARE_YEAR_Y		68

	#define MOTHERSHIP_WAR_DECLARE_MONTH_X		308
	#define MOTHERSHIP_WAR_DECLARE_MONTH_Y		68

	#define MOTHERSHIP_WAR_DECLARE_NOON_X		200
	#define MOTHERSHIP_WAR_DECLARE_NOON_Y		320

	#define MOTHERSHIP_WAR_DECLARE_TIME_HOUR_X	257
	#define MOTHERSHIP_WAR_DECLARE_TIME_HOUR_Y	319

	// 2009. 05. 27 by ckPark �������� ���ڿ� ��ġ ����

	// #define MOTHERSHIP_WAR_DECLARE_DESC_X		147
	#define MOTHERSHIP_WAR_DECLARE_DESC_X		30

	// end 2009. 05. 27 by ckPark �������� ���ڿ� ��ġ ����

	#define MOTHERSHIP_WAR_DECLARE_DESC_Y		364		// 2012-06-29 by isshin UI ��ǥ ���� ( 318 -> 364 )

	// end 2009. 01. 12 by ckPark ���� ���� �ý���


	// 2009. 05. 27 by ckPark �������� ���ڿ� ��ġ ����
	#define MOTHERSHIP_WAR_DECLARE_YEAR_CENTER_X	222
	#define MOTHERSHIP_WAR_DECLARE_MONTH_CENTER_X	318
	// end 2009. 05. 27 by ckPark �������� ���ڿ� ��ġ ����
#else
#define MOTHERSHIP_WAR_DECLARE_BG_W			395
#define MOTHERSHIP_WAR_DECLARE_BG_H			420

#define MOTHERSHIP_WAR_DECLARE_BAR_W		MOTHERSHIP_WAR_DECLARE_BG_W
#define MOTHERSHIP_WAR_DECLARE_BAR_H		20

#define MOTHERSHIP_WAR_DECLARE_CAL_LEFT_X	283
#define MOTHERSHIP_WAR_DECLARE_CAL_LEFT_Y	69

#define MOTHERSHIP_WAR_DECLARE_CAL_RIGHT_X	343
#define MOTHERSHIP_WAR_DECLARE_CAL_RIGHT_Y	69

#define MOTHERSHIP_WAR_DECLARE_DAY_X		182
#define MOTHERSHIP_WAR_DECLARE_DAY_Y		96

#define MOTHERSHIP_WAR_DECLARE_DAY_RECT_X	174
#define MOTHERSHIP_WAR_DECLARE_DAY_RECT_Y	94

#define MOTHERSHIP_WAR_DECLARE_CAL_X		182
#define MOTHERSHIP_WAR_DECLARE_CAL_Y		120

#define MOTHERSHIP_WAR_DECLARE_CAL_GAP_W	26
#define MOTHERSHIP_WAR_DECLARE_CAL_GAP_H	24

#define MOTHERSHIP_WAR_DECLARE_TIMESELUP_X	233
#define MOTHERSHIP_WAR_DECLARE_TIMESELUP_Y	284

#define MOTHERSHIP_WAR_DECLARE_TIMESELDOWN_X	233
#define MOTHERSHIP_WAR_DECLARE_TIMESELDOWN_Y	295

#define MOTHERSHIP_WAR_DECLARE_FLAG_X		27
#define MOTHERSHIP_WAR_DECLARE_FLAG_Y		61

#define	MOTHERSHIP_WAR_DECLARE_GIVEUP_X		65
#define	MOTHERSHIP_WAR_DECLARE_GIVEUP_Y		383

#define MOTHERSHIP_WAR_DECLARE_SAVE_X		165
#define MOTHERSHIP_WAR_DECLARE_SAVE_Y		383

#define MOTHERSHIP_WAR_DECLARE_CLOSE_X		266
#define MOTHERSHIP_WAR_DECLARE_CLOSE_Y		383

#define MOTHERSHIP_WAR_DECLARE_YEAR_X		203
#define MOTHERSHIP_WAR_DECLARE_YEAR_Y		68

#define MOTHERSHIP_WAR_DECLARE_MONTH_X		308
#define MOTHERSHIP_WAR_DECLARE_MONTH_Y		68

#define MOTHERSHIP_WAR_DECLARE_NOON_X		200
#define MOTHERSHIP_WAR_DECLARE_NOON_Y		288

#define MOTHERSHIP_WAR_DECLARE_TIME_HOUR_X	247
#define MOTHERSHIP_WAR_DECLARE_TIME_HOUR_Y	287

// 2009. 05. 27 by ckPark �������� ���ڿ� ��ġ ����

// #define MOTHERSHIP_WAR_DECLARE_DESC_X		147
#define MOTHERSHIP_WAR_DECLARE_DESC_X		30

// end 2009. 05. 27 by ckPark �������� ���ڿ� ��ġ ����

#define MOTHERSHIP_WAR_DECLARE_DESC_Y		332

// end 2009. 01. 12 by ckPark ���� ���� �ý���


// 2009. 05. 27 by ckPark �������� ���ڿ� ��ġ ����
#define MOTHERSHIP_WAR_DECLARE_YEAR_CENTER_X	222
#define MOTHERSHIP_WAR_DECLARE_MONTH_CENTER_X	318
// end 2009. 05. 27 by ckPark �������� ���ڿ� ��ġ ����
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFWarDeclare::CINFWarDeclare(CGameData * pData)
{
	// 2009. 01. 12 by ckPark ���� ���� �ý���

	m_pGameData					= pData;

	m_bLButtonClick				= NULL;
	
	m_bWarDeclareAct			= FALSE;
	m_pImgWarDeclareBG			= NULL;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	m_pImgWarDeclareControl		= NULL;
#endif
	
	m_pCalendarLeftBtn			= NULL;
	m_pCalendarRightBtn			= NULL;
	
	m_pCalendarRect				= NULL;
	m_pCalendarCheck			= NULL;
	
	m_pWarDeclareNoonSelBtnUp	= NULL;
	m_pWarDeclareNoonSelBtnDown	= NULL;
	
	m_pWarDeclareHourEditBox	= NULL;
	m_pWarDeclareMinEditBox		= NULL;
	
	m_pANIWhiteFlag				= NULL;
	m_pBCUWhiteFlag				= NULL;
	m_pANICombatFlag			= NULL;
	m_pBCUCombatFlag			= NULL;
	
	m_pGiveupWarBtn				= NULL;
	m_pSaveWarDeclareOptionBtn	= NULL;
	m_pCloseDeclareOption		= NULL;
	
	m_pFontWarDeclare			= NULL;
	
	// end 2009. 01. 12 by ckPark ���� ���� �ý���

	// 2009. 02. 10 by ckPark ���� ���� �ý��� �߰� ����
	m_bDisableWarDeclare		= FALSE;
	// end 2009. 02. 10 by ckPark ���� ���� �ý��� �߰� ����
}

CINFWarDeclare::~CINFWarDeclare()
{

}

HRESULT CINFWarDeclare::InitDeviceObjects()
{
	// 2009. 01. 12 by ckPark ���� ���� �ý���
	char buf[MAX_PATH];
	DataHeader	* pDataHeader;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("chief_de");
	m_pImgWarDeclareBG = g_pGameMain->m_GruopImagemanager->GetGroupImage(pDataHeader);
	m_pImgWarDeclareBG->InitDeviceObjects( g_pD3dApp->m_pImageList );

	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("chief_btn2");
	m_pImgWarDeclareControl = g_pGameMain->m_GruopImagemanager->GetGroupImage(pDataHeader);
#else
	wsprintf(buf,"wdbg");
	pDataHeader = FindResource(buf);
	if(pDataHeader == NULL)
		return S_FALSE;
	m_pImgWarDeclareBG	= new CINFImageEx;
	m_pImgWarDeclareBG->InitDeviceObjects( pDataHeader );
#endif
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "call3");
		wsprintf(szDownBtn, "call1");
		wsprintf(szSelBtn, "call0");
		wsprintf(szDisBtn, "call2");
		if(NULL == m_pCalendarLeftBtn)
		{
			m_pCalendarLeftBtn	= new CINFImageBtn;
		}
		m_pCalendarLeftBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
	}

	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "calr3");
		wsprintf(szDownBtn, "calr1");
		wsprintf(szSelBtn, "calr0");
		wsprintf(szDisBtn, "calr2");
		if(NULL == m_pCalendarRightBtn)
		{
			m_pCalendarRightBtn	= new CINFImageBtn;
		}
		m_pCalendarRightBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
	}

	wsprintf(buf,"calrect");
	pDataHeader = FindResource(buf);
	if(pDataHeader == NULL)
		return S_FALSE;

	m_pCalendarRect		= new CINFImageEx;
	m_pCalendarRect->InitDeviceObjects( pDataHeader  );

	wsprintf(buf,"calc");
	pDataHeader = FindResource(buf);
	if(pDataHeader == NULL)
		return S_FALSE;

	m_pCalendarCheck	= new CINFImageEx;
	m_pCalendarCheck->InitDeviceObjects( pDataHeader );

	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "ts3");
		wsprintf(szDownBtn, "ts1");
		wsprintf(szSelBtn, "ts0");
		wsprintf(szDisBtn, "ts2");
		if(NULL == m_pWarDeclareNoonSelBtnUp)
		{
			m_pWarDeclareNoonSelBtnUp	= new CINFImageBtn;
		}
		m_pWarDeclareNoonSelBtnUp->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
	}

	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "tsb3");
		wsprintf(szDownBtn, "tsb1");
		wsprintf(szSelBtn, "tsb0");
		wsprintf(szDisBtn, "tsb2");
		if(NULL == m_pWarDeclareNoonSelBtnDown)
		{
			m_pWarDeclareNoonSelBtnDown	= new CINFImageBtn;
		}
		m_pWarDeclareNoonSelBtnDown->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
	}

	{
		if(NULL == m_pWarDeclareHourEditBox)
			m_pWarDeclareHourEditBox = new CINFNumEditBox;

		char chBuff[32];
		char chMaxMixCnt[64];
		
		wsprintf(chBuff, "0");
		wsprintf(chMaxMixCnt, "%d", 12);

		POINT ptPos = {m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_X, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_Y};
		m_pWarDeclareHourEditBox->InitDeviceObjects(9, ptPos, 35, TRUE, 20);
		m_pWarDeclareHourEditBox->SetMaxStringLen(strlen(chMaxMixCnt));
		m_pWarDeclareHourEditBox->SetString(chBuff, 32);
	}

	{
		if(NULL == m_pWarDeclareMinEditBox)
			m_pWarDeclareMinEditBox = new CINFNumEditBox;
		
		char chBuff[32];
		char chMaxMixCnt[64];
		
		wsprintf(chBuff, "0");
		wsprintf(chMaxMixCnt, "%d", 12);
		
		POINT ptPos = {m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_X, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_Y};
		m_pWarDeclareMinEditBox->InitDeviceObjects(9, ptPos, 35, TRUE, 20);
		m_pWarDeclareMinEditBox->SetMaxStringLen(strlen(chMaxMixCnt));
		m_pWarDeclareMinEditBox->SetString(chBuff, 32);
	}
	
	wsprintf(buf,"aniwf");
	pDataHeader = FindResource(buf);
	if(pDataHeader == NULL)
		return S_FALSE;
	m_pANIWhiteFlag		= new CINFImageEx;
	m_pANIWhiteFlag->InitDeviceObjects( pDataHeader );
	
	wsprintf(buf,"bcuwf");
	pDataHeader = FindResource(buf);
	if(pDataHeader == NULL)
		return S_FALSE;
	m_pBCUWhiteFlag		= new CINFImageEx;
	m_pBCUWhiteFlag->InitDeviceObjects( pDataHeader );

	wsprintf(buf,"anicf");
	pDataHeader = FindResource(buf);
	if(pDataHeader == NULL)
		return S_FALSE;
	m_pANICombatFlag	= new CINFImageEx;
	m_pANICombatFlag->InitDeviceObjects( pDataHeader );

	wsprintf(buf,"bcucf");
	pDataHeader = FindResource(buf);
	if(pDataHeader == NULL)
		return S_FALSE;
	m_pBCUCombatFlag	= new CINFImageEx;
	m_pBCUCombatFlag->InitDeviceObjects( pDataHeader );

	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "wdgbtn3");
		wsprintf(szDownBtn, "wdgbtn1");
		wsprintf(szSelBtn, "wdgbtn0");
		wsprintf(szDisBtn, "wdgbtn2");
		if(NULL == m_pGiveupWarBtn)
		{
			m_pGiveupWarBtn	= new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
		//m_pGiveupWarBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		m_pGiveupWarBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP86");
		// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	}
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		wsprintf(szUpBtn, "apps03");
		wsprintf(szDownBtn, "apps01");
		wsprintf(szSelBtn, "apps00");
		wsprintf(szDisBtn, "apps02");
#else
		wsprintf(szUpBtn, "wdsbtn3");
		wsprintf(szDownBtn, "wdsbtn1");
		wsprintf(szSelBtn, "wdsbtn0");
		wsprintf(szDisBtn, "wdsbtn2");
#endif
		if(NULL == m_pSaveWarDeclareOptionBtn)
		{
			m_pSaveWarDeclareOptionBtn	= new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
		//m_pSaveWarDeclareOptionBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		m_pSaveWarDeclareOptionBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP11");
		// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	}

	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		wsprintf(szUpBtn, "cans03");
		wsprintf(szDownBtn, "cans01");
		wsprintf(szSelBtn, "cans00");
		wsprintf(szDisBtn, "cans02");
#else
		wsprintf(szUpBtn, "shnpc063");
		wsprintf(szDownBtn, "shnpc061");
		wsprintf(szSelBtn, "shnpc060");
		wsprintf(szDisBtn, "shnpc062");
#endif
		if(NULL == m_pCloseDeclareOption)
		{
			m_pCloseDeclareOption	= new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
		//m_pCloseDeclareOption->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		m_pCloseDeclareOption->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP90");
		// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	}

	m_pFontWarDeclare = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 8, D3DFONT_ZENABLE, TRUE, 256, 32);
	m_pFontWarDeclare->InitDeviceObjects(g_pD3dDev);

	return S_OK;
	// end 2009. 01. 12 by ckPark ���� ���� �ý���
}

HRESULT CINFWarDeclare::RestoreDeviceObjects()
{
	// 2009. 01. 12 by ckPark ���� ���� �ý���
	
	m_pImgWarDeclareBG->RestoreDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	
#endif
	m_pCalendarLeftBtn->RestoreDeviceObjects();
	m_pCalendarRightBtn->RestoreDeviceObjects();
	
	m_pCalendarRect->RestoreDeviceObjects();
	
	m_pCalendarCheck->RestoreDeviceObjects();
	
	m_pWarDeclareNoonSelBtnUp->RestoreDeviceObjects();
	m_pWarDeclareNoonSelBtnDown->RestoreDeviceObjects();
	
	m_pWarDeclareHourEditBox->RestoreDeviceObjects();
	m_pWarDeclareMinEditBox->RestoreDeviceObjects();
	
	m_pANIWhiteFlag->RestoreDeviceObjects();
	m_pBCUWhiteFlag->RestoreDeviceObjects();
	m_pANICombatFlag->RestoreDeviceObjects();
	m_pBCUCombatFlag->RestoreDeviceObjects();
	
	m_pGiveupWarBtn->RestoreDeviceObjects();
	m_pSaveWarDeclareOptionBtn->RestoreDeviceObjects();
	m_pCloseDeclareOption->RestoreDeviceObjects();
	
	m_pFontWarDeclare->RestoreDeviceObjects();

	return S_OK;
	// end 2009. 01. 12 by ckPark ���� ���� �ý���
}

HRESULT CINFWarDeclare::DeleteDeviceObjects()
{
	// 2009. 01. 12 by ckPark ���� ���� �ý���
	if(m_pImgWarDeclareBG)
	{
		m_pImgWarDeclareBG->DeleteDeviceObjects();
		util::del(m_pImgWarDeclareBG);
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	if( m_pImgWarDeclareControl )
	{
		m_pImgWarDeclareControl->DeleteDeviceObjects();
		util::del( m_pImgWarDeclareControl );
	}	
#endif
	
	if(m_pCalendarLeftBtn)
	{
		m_pCalendarLeftBtn->DeleteDeviceObjects();
		util::del(m_pCalendarLeftBtn);
	}
	
	if(m_pCalendarRightBtn)
	{
		m_pCalendarRightBtn->DeleteDeviceObjects();
		util::del(m_pCalendarRightBtn);
	}
	
	if(m_pCalendarRect)
	{
		m_pCalendarRect->DeleteDeviceObjects();
		util::del(m_pCalendarRect);
	}
	
	if(m_pCalendarCheck)
	{
		m_pCalendarCheck->DeleteDeviceObjects();
		util::del(m_pCalendarCheck);
	}
	
	if(m_pWarDeclareNoonSelBtnUp)
	{
		m_pWarDeclareNoonSelBtnUp->DeleteDeviceObjects();
		util::del(m_pWarDeclareNoonSelBtnUp);
	}

	if(m_pWarDeclareNoonSelBtnDown)
	{
		m_pWarDeclareNoonSelBtnDown->DeleteDeviceObjects();
		util::del(m_pWarDeclareNoonSelBtnDown);
	}
	
	if(m_pWarDeclareHourEditBox)
	{
		m_pWarDeclareHourEditBox->DeleteDeviceObjects();
		util::del(m_pWarDeclareHourEditBox);
	}
	
	if(m_pWarDeclareMinEditBox)
	{
		m_pWarDeclareMinEditBox->DeleteDeviceObjects();
		util::del(m_pWarDeclareMinEditBox);
	}
	
	if(m_pANIWhiteFlag)
	{
		m_pANIWhiteFlag->DeleteDeviceObjects();
		util::del(m_pANIWhiteFlag);
	}
	
	if(m_pBCUWhiteFlag)
	{
		m_pBCUWhiteFlag->DeleteDeviceObjects();
		util::del(m_pBCUWhiteFlag);
	}
	
	if(m_pANICombatFlag)
	{
		m_pANICombatFlag->DeleteDeviceObjects();
		util::del(m_pANICombatFlag);
	}
	
	if(m_pBCUCombatFlag)
	{
		m_pBCUCombatFlag->DeleteDeviceObjects();
		util::del(m_pBCUCombatFlag);
	}
	
	if(m_pGiveupWarBtn)
	{
		m_pGiveupWarBtn->DeleteDeviceObjects();
		util::del(m_pGiveupWarBtn);
	}
	
	if(m_pSaveWarDeclareOptionBtn)
	{
		m_pSaveWarDeclareOptionBtn->DeleteDeviceObjects();
		util::del(m_pSaveWarDeclareOptionBtn);
	}
	
	if(m_pCloseDeclareOption)
	{
		m_pCloseDeclareOption->DeleteDeviceObjects();
		util::del(m_pCloseDeclareOption);
	}
	
	if(m_pFontWarDeclare)
	{
		m_pFontWarDeclare->DeleteDeviceObjects();
		util::del(m_pFontWarDeclare);
	}
	
	return S_OK;
	// end 2009. 01. 12 by ckPark ���� ���� �ý���
}

HRESULT CINFWarDeclare::InvalidateDeviceObjects()
{
	// 2009. 01. 12 by ckPark ���� ���� �ý���
	m_pImgWarDeclareBG->InvalidateDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	if( m_pImgWarDeclareControl )
	{
		m_pImgWarDeclareControl->InvalidateDeviceObjects();
	}	
#endif
	m_pCalendarLeftBtn->InvalidateDeviceObjects();
	m_pCalendarRightBtn->InvalidateDeviceObjects();

	m_pCalendarRect->InvalidateDeviceObjects();

	m_pCalendarCheck->InvalidateDeviceObjects();

	m_pWarDeclareNoonSelBtnUp->InvalidateDeviceObjects();

	m_pWarDeclareNoonSelBtnDown->InvalidateDeviceObjects();

	m_pWarDeclareHourEditBox->InvalidateDeviceObjects();
	m_pWarDeclareMinEditBox->InvalidateDeviceObjects();

	m_pANIWhiteFlag->InvalidateDeviceObjects();
	m_pBCUWhiteFlag->InvalidateDeviceObjects();
	m_pANICombatFlag->InvalidateDeviceObjects();
	m_pBCUCombatFlag->InvalidateDeviceObjects();

	m_pGiveupWarBtn->InvalidateDeviceObjects();
	m_pSaveWarDeclareOptionBtn->InvalidateDeviceObjects();
	m_pCloseDeclareOption->InvalidateDeviceObjects();

	m_pFontWarDeclare->InvalidateDeviceObjects();

	return S_OK;
	// end 2009. 01. 12 by ckPark ���� ���� �ý���
}

// 2009. 01. 12 by ckPark ���� ���� �ý���
void	CINFWarDeclare::SetWarDeclareForbidPeriod(const BYTE nForbidDayOfWeek, const ATUM_DATE_TIME* pStart, const ATUM_DATE_TIME* pEnd)
{
	// ����, ����, ��
	m_nForbidDayOfWeek				= nForbidDayOfWeek;
	m_WarDeclareForbidStartPeriod	= *pStart;
	m_WarDeclareForbidEndPeriod		= *pEnd;
}


void	CINFWarDeclare::SetWarDecalreEnablePeriod(const ATUM_DATE_TIME* pStart, const ATUM_DATE_TIME* pEnd)
{
	// ���� ��
	m_WarDeclareEnableStartPeriod	= *pStart;
	m_WarDeclareEnableEndPeriod		= *pEnd;

	// 2009. 07. 08 by ckPark ���������ֱ� ������ ��, �ð��� 00:00�� ��� ���� 23:59���� �����ϵ��� ����
	if( m_WarDeclareEnableEndPeriod.Hour == 0 && m_WarDeclareEnableEndPeriod.Minute == 0 )
	{
		// �ֱ� ���������� 1���� �ƴ϶�� �ܼ��� �����ϼ����� 1�� ���� ������ �ذ�ȴ�
		if( m_WarDeclareEnableEndPeriod.Day > 1 )
			--(m_WarDeclareEnableEndPeriod.Day);
		else
		{
			// �ֱ� ���������� 1���̸� ����, ������ �����ؼ� ����ؾ� �Ѵ�
			
			// ���� ��, ��
			int		nYear, nMon;

			// �̹����� 1���� �ƴ϶��
			if( m_WarDeclareEnableEndPeriod.Month != 1 )
			{
				nYear	= m_WarDeclareEnableEndPeriod.Year;		// �����
				nMon	= m_WarDeclareEnableEndPeriod.Month - 1;// ����
			}
			else
			{// �̹����� 1���̶��
				nYear	= m_WarDeclareEnableEndPeriod.Year - 1;	// ����
				nMon	= 12;									// 12��
			}
			
			// ���� �ϼ�
			int		nPrevDays;
			nPrevDays = CCalendarBuilder().CalMonthDay( nYear, nMon );

			// ���� ��, ��, �����ϼ��� �����Ѵ�
			m_WarDeclareEnableEndPeriod.Year	= nYear;
			m_WarDeclareEnableEndPeriod.Month	= nMon;
			m_WarDeclareEnableEndPeriod.Day		= nPrevDays;
		}

		m_WarDeclareEnableEndPeriod.Hour	= 23;
		m_WarDeclareEnableEndPeriod.Minute	= 59;
	}
	// end 2009. 07. 08 by ckPark ���������ֱ� ������ ��, �ð��� 00:00�� ��� ���� 23:59���� �����ϵ��� ����
}

void	CINFWarDeclare::ShowWarDeclare(void)
{
	if (!COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race, RACE_INFLUENCE_LEADER | RACE_GAMEMASTER | RACE_OPERATION | RACE_MONITOR))
	{
		g_pGameMain->CreateChatChild(STRMSG_C_090113_0316, COLOR_ERROR); // "\\y�����ڸ� �̿��� �� �ֽ��ϴ�.\\y"
		return;
	}

	m_bWarDeclareAct	= TRUE;
	m_nWarDeclarePosX	= (g_pD3dApp->GetBackBufferDesc().Width - MOTHERSHIP_WAR_DECLARE_BG_W)/2;
	m_nWarDeclarePosY	= (g_pD3dApp->GetBackBufferDesc().Height - MOTHERSHIP_WAR_DECLARE_BG_H)/2;

	// ���� �ð����� �޷� ����
	ATUM_DATE_TIME serverTime	= GetServerDateTime();
	m_nCurCalYear				= serverTime.Year;
	m_nCurCalMonth				= serverTime.Month;
	CCalendarBuilder().BuildCalendar(m_nCurCalYear, m_nCurCalMonth, &m_Calendar);

	// �� �ʱ�ȭ
	m_bWarDeclareNoon		= FALSE;

	m_WarDeclareDate.Year	= 0;
	m_WarDeclareDate.Month	= 0;
	m_WarDeclareDate.Day	= 0;
	m_WarDeclareDate.Hour	= 0;
	m_WarDeclareDate.Minute	= 0;

	m_WarDeclareDateView			= m_WarDeclareDate;

	m_WarDeclareEnableStartPeriod	= m_WarDeclareDate;
	m_WarDeclareEnableEndPeriod		= m_WarDeclareDate;

	CalcCalendarRect();

	m_nWarDeclareLeftChance	= 0;

	// Lock
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_FC_INFO_DECLARATION_MSWAR_INFO);
		vecUnLockMsg.push_back(T_FC_INFO_DECLARATION_MSWAR_INFO_OK);
		// �ϳ��� ��Ī �Ǿ ���� Ǯ����. 
		g_pD3dApp->EnterMultiLock(TRUE, T_FC_INFO_DECLARATION_MSWAR_INFO, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}

	g_pFieldWinSocket->SendMsg(T_FC_INFO_DECLARATION_MSWAR_INFO, 0, 0);
}

void	CINFWarDeclare::HideWarDeclare(void)
{
	// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
	// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	m_bWarDeclareAct		= FALSE;

	m_pWarDeclareMinEditBox->EnableEdit(FALSE);
	m_pWarDeclareHourEditBox->EnableEdit(FALSE);
}

void	CINFWarDeclare::RenderCalendar(void)
{
	// �޷� ���� ��ư
	m_pCalendarLeftBtn->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_CAL_LEFT_X,
									   m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_CAL_LEFT_Y);
	m_pCalendarLeftBtn->Render();
	
	// �޷� ������ ��ư
	m_pCalendarRightBtn->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_CAL_RIGHT_X,
										m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_CAL_RIGHT_Y);
	m_pCalendarRightBtn->Render();
	
	char szBuff[MAX_PATH];
	
	// �޷� ��
	sprintf(szBuff, STRMSG_C_090113_0303, m_nCurCalYear);


	// 2009. 05. 27 by ckPark �������� ���ڿ� ��ġ ����
// 	m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_YEAR_X,
// 								m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_YEAR_Y,
// 								GUI_FONT_COLOR,
// 								szBuff);
	SIZE sz = m_pFontWarDeclare->GetStringSize( szBuff );
	m_pFontWarDeclare->DrawText(MOTHERSHIP_WAR_DECLARE_YEAR_CENTER_X - (sz.cx/2) + m_nWarDeclarePosX,
								m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_YEAR_Y,
								GUI_FONT_COLOR,
								szBuff);
	// end 2009. 05. 27 by ckPark �������� ���ڿ� ��ġ ����
	
	// �޷� ��
	sprintf(szBuff, STRMSG_C_090113_0304, m_nCurCalMonth);	


	// 2009. 05. 27 by ckPark �������� ���ڿ� ��ġ ����
// 	m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_MONTH_X,
// 								m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_MONTH_Y,
// 								GUI_FONT_COLOR,
// 								szBuff);
	sz = m_pFontWarDeclare->GetStringSize( szBuff );
	
	m_pFontWarDeclare->DrawText( MOTHERSHIP_WAR_DECLARE_MONTH_CENTER_X - (sz.cx/2) + m_nWarDeclarePosX,
								m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_MONTH_Y,
								GUI_FONT_COLOR,
								szBuff);
	// end 2009. 05. 27 by ckPark �������� ���ڿ� ��ġ ����

	// ���� �迭
	char*	pTemp[MAX_DAY] = {STRMSG_C_090113_0309,
							  STRMSG_C_090113_0310,
							  STRMSG_C_090113_0311,
							  STRMSG_C_090113_0312,
							  STRMSG_C_090113_0313,
							  STRMSG_C_090113_0314,
							  STRMSG_C_090113_0315};

	// ���� ������
	int i;
	for(i=0; i<MAX_DAY; ++i)
	{
		// ����
		m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_DAY_X + (i * MOTHERSHIP_WAR_DECLARE_CAL_GAP_W),
									m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_DAY_Y,
									GUI_FONT_COLOR,
									pTemp[i]);
	}

	// 7�� 6��¥�� �޷�
	int j;
	for(i=0; i<MAX_CALENDAR_LINE; ++i)
	{
		for(j=0; j<MAX_DAY; ++j)
		{
			// ���� ���� �Ⱓ���� Ȯ��, ��, ��, �ʴ� ����
			ATUM_DATE_TIME temp, start(m_WarDeclareEnableStartPeriod), end(m_WarDeclareEnableEndPeriod);
			temp.SetDateTime(m_Calendar[i][j].nYear, m_Calendar[i][j].nMonth, m_Calendar[i][j].nDay, 0, 0, 0);
			start.Hour	=	end.Hour	= 0;
			start.Minute=	end.Minute	= 0;
			start.Second=	end.Second	= 0;
			
			// ���� ���� �Ⱓ�̸� �ڽ� ������
			if(temp >= start && temp <= end)
			{
				m_pCalendarRect->Move(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_DAY_RECT_X + (j * m_pCalendarRect->GetImgSize().x),
									  m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_DAY_RECT_Y + ( (i+1) * MOTHERSHIP_WAR_DECLARE_CAL_GAP_H));
				m_pCalendarRect->Render();
			}
			
			sprintf(szBuff, "%2d", m_Calendar[i][j].nDay);

			// ���� ���� �ƴ϶�� ȸ������ ���� ǥ��
			if(m_Calendar[i][j].nMonth != m_nCurCalMonth)
			{
				m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_CAL_X + (j * MOTHERSHIP_WAR_DECLARE_CAL_GAP_W),
											m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_CAL_Y + (i * MOTHERSHIP_WAR_DECLARE_CAL_GAP_H),
											GUI_FONT_COLOR_G,
											szBuff);
			}
			else
			{// ���� ���� ������� ���� ǥ��
				m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_CAL_X + (j * MOTHERSHIP_WAR_DECLARE_CAL_GAP_W),
											m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_CAL_Y + (i * MOTHERSHIP_WAR_DECLARE_CAL_GAP_H),
											GUI_FONT_COLOR_W,
											szBuff);
			}

			// ���� ���� ��¥�� v�� ǥ��
			if(m_Calendar[i][j].nYear == m_WarDeclareDateView.Year
			   && m_Calendar[i][j].nMonth == m_WarDeclareDateView.Month
			   && m_Calendar[i][j].nDay == m_WarDeclareDateView.Day)
			{
				m_pCalendarCheck->Move(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_CAL_X + (j * MOTHERSHIP_WAR_DECLARE_CAL_GAP_W),
									   m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_CAL_Y + (i * MOTHERSHIP_WAR_DECLARE_CAL_GAP_H));
				m_pCalendarCheck->Render();
			}
		}
	}
}

void	CINFWarDeclare::Render(void)
{
	if(!m_bWarDeclareAct)
		return;

	// �� �׶���
	m_pImgWarDeclareBG->Move(m_nWarDeclarePosX, m_nWarDeclarePosY);
	m_pImgWarDeclareBG->Render();

	// ����̹��� ������
	// �⵵�� 0�̸� ����, �� ���º��� ����� �ٸ�
	CINFImage* pFlagImg;
	if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI ) )
	{
		if( m_WarDeclareDate.Year == 0 )
			pFlagImg = m_pANIWhiteFlag;
		else
			pFlagImg = m_pANICombatFlag;
	}
	else if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_VCN ) )
	{
		if( m_WarDeclareDate.Year == 0 )
			pFlagImg = m_pBCUWhiteFlag;
		else
			pFlagImg = m_pBCUCombatFlag;
	}

	// ��� ������
	pFlagImg->Move( m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_FLAG_X, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_FLAG_Y );
	pFlagImg->Render();

	// �޷� ������
	RenderCalendar();

	// ���� ���� ���� ǥ��
	if(!m_bWarDeclareNoon)
	{
		m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_NOON_X,
									m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_NOON_Y,
									GUI_FONT_COLOR,
									STRMSG_C_090113_0305);	// ����
	}
	else
	{
		m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_NOON_X,
									m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_NOON_Y,
									GUI_FONT_COLOR,
									STRMSG_C_090113_0306);	// ����
	}
	
	// ���� ���� ��ư
	m_pWarDeclareNoonSelBtnUp->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_TIMESELUP_X,
										 m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_TIMESELUP_Y);
	m_pWarDeclareNoonSelBtnUp->Render();

	m_pWarDeclareNoonSelBtnDown->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_TIMESELDOWN_X,
										m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_TIMESELDOWN_Y);
	m_pWarDeclareNoonSelBtnDown->Render();

	// �ð� �ڽ�
	m_pWarDeclareHourEditBox->SetPos(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_X, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_Y);
	m_pWarDeclareHourEditBox->Render();

	// : 
	m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_X + 40, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_Y, GUI_FONT_COLOR, ":");
	
	// �� �ڽ�
	m_pWarDeclareMinEditBox->SetPos(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_X + 35, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_Y);
	m_pWarDeclareMinEditBox->Render();

	// ���� ���� ���� Ƚ�� ����
	char szBuff[MAX_PATH];
	sprintf(szBuff, STRMSG_C_090113_0307, m_nWarDeclareLeftChance);
	m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_DESC_X,
								m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_DESC_Y,
								GUI_FONT_COLOR,
								szBuff);

	
	// 2009. 02. 10 by ckPark ���� ���� �ý��� �߰� ����

// 	// ���� �ð����κ��� ������ �Ⱓ���� ���� �ϼ� ���
// 	ATUM_DATE_TIME serverTime	= GetServerDateTime();
// 	int nTodayTotalDay			= CCalendarBuilder().CalTotalDay(serverTime.Year, serverTime.Month, serverTime.Day);
// 	int nEndTotalDay			= CCalendarBuilder().CalTotalDay(m_WarDeclareEnableStartPeriod.Year, m_WarDeclareEnableStartPeriod.Month, m_WarDeclareEnableStartPeriod.Day);
// 	sprintf(szBuff, STRMSG_C_090113_0308, nEndTotalDay - nTodayTotalDay);
// 	// ���� ���� �Ⱓ ǥ��
// 	m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_DESC_X,
// 								m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_DESC_Y + 17,
// 								GUI_FONT_COLOR,
// 								szBuff);

	// ���� �Ұ����� ��� ���� �ϼ� 0���� ����
	if(m_bDisableWarDeclare)
		sprintf(szBuff, STRMSG_C_090113_0308, 0);
	else
	{
		ATUM_DATE_TIME serverTime	= GetServerDateTime();
		int nTodayTotalDay			= CCalendarBuilder().CalTotalDay(serverTime.Year, serverTime.Month, serverTime.Day);
		int nEndTotalDay			= CCalendarBuilder().CalTotalDay(m_WarDeclareEnableStartPeriod.Year, m_WarDeclareEnableStartPeriod.Month, m_WarDeclareEnableStartPeriod.Day);
		sprintf(szBuff, STRMSG_C_090113_0308, nEndTotalDay - nTodayTotalDay);
	}

	// ���� ���� �Ⱓ ǥ��
	m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_DESC_X,
								m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_DESC_Y + 17,
								GUI_FONT_COLOR,
								szBuff);
	// end 2009. 02. 10 by ckPark ���� ���� �ý��� �߰� ����

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	// ���� ��ư
	POINT pPos = m_pImgWarDeclareControl->GetFindControlTargetofMinPos("wdgbtn0");
	m_pGiveupWarBtn->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_GIVEUP_X + pPos.x, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_GIVEUP_Y + pPos.y );
	m_pGiveupWarBtn->Render();

	// ���� ��ư
	pPos = m_pImgWarDeclareControl->GetFindControlTargetofMinPos("apps00");
	m_pSaveWarDeclareOptionBtn->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_GIVEUP_X + pPos.x, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_SAVE_Y + pPos.y );
	m_pSaveWarDeclareOptionBtn->Render();

	// �ݱ� ��ư
	pPos = m_pImgWarDeclareControl->GetFindControlTargetofMinPos("cans00");
	m_pCloseDeclareOption->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_GIVEUP_X + pPos.x, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_CLOSE_Y + pPos.y );
	m_pCloseDeclareOption->Render();
#else
	// ���� ��ư
	m_pGiveupWarBtn->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_GIVEUP_X, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_GIVEUP_Y);
	m_pGiveupWarBtn->Render();

	// ���� ��ư
	m_pSaveWarDeclareOptionBtn->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_SAVE_X, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_SAVE_Y);
	m_pSaveWarDeclareOptionBtn->Render();

	// �ݱ� ��ư
	m_pCloseDeclareOption->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_CLOSE_X, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_CLOSE_Y);
	m_pCloseDeclareOption->Render();
#endif
}

void	CINFWarDeclare::OnClickCalLeft(void)
{
	// 1���� �ƴϸ� ����
	if(m_nCurCalMonth != 1)
		--m_nCurCalMonth;
	else
	{// 1���� ��� ���⵵ 12��
		--m_nCurCalYear;
		m_nCurCalMonth = 12;
	}

	// �޷� ����
	CCalendarBuilder().BuildCalendar(m_nCurCalYear, m_nCurCalMonth, &m_Calendar);
}

void	CINFWarDeclare::OnClickCalRight(void)
{
	// 12���� �ƴ� ��� ������
	if(m_nCurCalMonth != 12)
		++m_nCurCalMonth;
	else
	{// 12���� ��� �����⵵ 1��
		++m_nCurCalYear;
		m_nCurCalMonth = 1;
	}

	// �޷� ����
	CCalendarBuilder().BuildCalendar(m_nCurCalYear, m_nCurCalMonth, &m_Calendar);
}

void	CINFWarDeclare::OnClickNoonSel(void)
{
	// ���� ���� ����
	if(m_bWarDeclareNoon)
		m_bWarDeclareNoon = FALSE;
	else
		m_bWarDeclareNoon = TRUE;
}

void	CINFWarDeclare::OnClickCalendar(const POINT pt)
{
	// �޷� �ε��� ���
	int jIndex = (pt.y - m_nWarDeclarePosY - MOTHERSHIP_WAR_DECLARE_DAY_RECT_Y) / MOTHERSHIP_WAR_DECLARE_CAL_GAP_H - 1;
	int iIndex = (pt.x - m_nWarDeclarePosX - MOTHERSHIP_WAR_DECLARE_DAY_RECT_X) / m_pCalendarRect->GetImgSize().x;

	// �ú��ʴ� ������ ���� ���� �Ⱓ���� ����Ѵ�
	ATUM_DATE_TIME selectDay, start(m_WarDeclareEnableStartPeriod), end(m_WarDeclareEnableEndPeriod);
	selectDay.SetDateTime(m_Calendar[jIndex][iIndex].nYear, m_Calendar[jIndex][iIndex].nMonth, m_Calendar[jIndex][iIndex].nDay, 0, 0, 0);
	start.Hour	=	end.Hour	= 0;
	start.Minute=	end.Minute	= 0;
	start.Second=	end.Second	= 0;

	// 2009. 02. 10 by ckPark ���� ���� �ý��� �߰� ����
// 	// ���� ���� �Ⱓ�̸� ��������¥ ������Ʈ
// 	if(selectDay >= start && selectDay <= end)
// 	{
// 		m_WarDeclareDateView.Year	= m_Calendar[jIndex][iIndex].nYear;
// 		m_WarDeclareDateView.Month	= m_Calendar[jIndex][iIndex].nMonth;
// 		m_WarDeclareDateView.Day	= m_Calendar[jIndex][iIndex].nDay;
// 	}

	// ���� ���� �Ⱓ�̸� ��������¥ ������Ʈ
	if(selectDay >= start && selectDay <= end)
	{
		// ���� �Ұ����� ��� ���þ��ϰ� ��� �޽���
		if(m_bDisableWarDeclare)
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_090210_0303, COLOR_ERROR);// "\\y�������� ������ �ٷ� ���ֿ��� ���� ���� �մϴ�.\\y"
		else
		{
			m_WarDeclareDateView.Year	= m_Calendar[jIndex][iIndex].nYear;
			m_WarDeclareDateView.Month	= m_Calendar[jIndex][iIndex].nMonth;
			m_WarDeclareDateView.Day	= m_Calendar[jIndex][iIndex].nDay;
		}
	}
	// end 2009. 02. 10 by ckPark ���� ���� �ý��� �߰� ����
}

void	CINFWarDeclare::OnClickGiveUpWar(void)
{
	// 2009. 02. 10 by ckPark ���� ���� �ý��� �߰� ����
	// ���� �Ұ����� ��� ���þ��ϰ� ��� �޽���
	if(m_bDisableWarDeclare)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_090210_0302, COLOR_ERROR);// ""\\y�������� ������ �Ұ��� �ϹǷ� ����/���Ⱑ �Ұ��� �մϴ�\\y""
		return;
	}
	// end 2009. 02. 10 by ckPark ���� ���� �ý��� �߰� ����

	char chBuff[256];
	wsprintf(chBuff, STRMSG_C_090113_0320);
	g_pGameMain->m_pInfWindow->AddMsgBox(chBuff, _Q_WAR_DECLARE_GIVE_UP);	//"������ ���� ������ �����Ͻðڽ��ϱ�?"
}

void	CINFWarDeclare::OnClickSaveBtn(void)
{
	// 2009. 02. 10 by ckPark ���� ���� �ý��� �߰� ����
	// ���� �Ұ����� ��� ���þ��ϰ� ��� �޽���
	if(m_bDisableWarDeclare)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_090210_0302, COLOR_ERROR);// ""\\y�������� ������ �Ұ��� �ϹǷ� ����/���Ⱑ �Ұ��� �մϴ�\\y""
		return;
	}
	// end 2009. 02. 10 by ckPark ���� ���� �ý��� �߰� ����
	
	// ��, �� ���ڸ� ���ڷ� ����
	char szTemp[MAX_PATH];
	m_pWarDeclareHourEditBox->GetString(szTemp, MAX_PATH);
	m_WarDeclareDateView.Hour	= atoi(szTemp);

	m_pWarDeclareMinEditBox->GetString(szTemp, MAX_PATH);
	m_WarDeclareDateView.Minute	= atoi(szTemp);

	// �ð��� 0~11 ������ �Ѿ��ų� ���� 0~59���� �Ѿ��� ��� ����
	if( (m_WarDeclareDateView.Hour < 0) || (m_WarDeclareDateView.Hour >= 12)
	   || (m_WarDeclareDateView.Minute < 0) || (m_WarDeclareDateView.Minute >= 60) )
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_090113_0325, COLOR_ERROR);// "\\y�������� �ð��� �߸� �ԷµǾ����ϴ�. �Է������� �߸��Ǿ��ų�, ���� �ð����κ��� 12�ð� �̳��� �������� �� �� �����ϴ�.\\y"
		return;
	}

	// ���� �� ��� �ð��� 12�ð� ���ϱ�
	if(m_bWarDeclareNoon)
		m_WarDeclareDateView.Hour += 12;

	// ������ ���� ������ ���
	if(m_nForbidDayOfWeek == CCalendarBuilder().CalWeekDay(m_WarDeclareDateView.Year, m_WarDeclareDateView.Month, m_WarDeclareDateView.Day) )
	{
		// �����Ⱓ, ���������� �ð��� �ʴ����� ����
		int startSec, endSec, declareSec;
		startSec	= m_WarDeclareForbidStartPeriod.Hour * 3600 + m_WarDeclareForbidStartPeriod.Minute * 60 + m_WarDeclareForbidStartPeriod.Second;
		endSec		= m_WarDeclareForbidEndPeriod.Hour * 3600 + m_WarDeclareForbidEndPeriod.Minute * 60 + m_WarDeclareForbidEndPeriod.Second;
		declareSec	= m_WarDeclareDateView.Hour * 3600 + m_WarDeclareDateView.Minute * 60 + m_WarDeclareDateView.Second;

		// �ʴ����� üũ
		if(declareSec >= startSec && declareSec <= endSec)
		{
			char*	pTemp[MAX_DAY] = {STRMSG_C_090113_0309,
									  STRMSG_C_090113_0310,
									  STRMSG_C_090113_0311,
									  STRMSG_C_090113_0312,
									  STRMSG_C_090113_0313,
									  STRMSG_C_090113_0314,
									  STRMSG_C_090113_0315};

			char szTemp[MAX_PATH];
			//"\\y%s���� %d:%d���� %d:%d���ͱ����� �������� �� �� �����ϴ�.\\y"
			sprintf(szTemp, STRMSG_C_090113_0328, pTemp[m_nForbidDayOfWeek],
												  m_WarDeclareForbidStartPeriod.Hour,
												  m_WarDeclareForbidStartPeriod.Second,
												  m_WarDeclareForbidEndPeriod.Hour,
												  m_WarDeclareForbidEndPeriod.Minute);

			g_pD3dApp->m_pChat->CreateChatChild(szTemp, COLOR_ERROR);
			return;
		}
	}

	// ����� �����������ڿ� �����Ϸ��� �������� ���� ��� ����
	if(m_WarDeclareDate == m_WarDeclareDateView)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_090113_0327, COLOR_ERROR);// "\\y����� ������ �����ϴ�.\\y"
		return;
	}
	
	char chBuff[256], *szEnemyInflName;

	if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI ) )
		szEnemyInflName	= STRMSG_C_060210_0000;
	else if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_VCN ) )
		szEnemyInflName	= STRMSG_C_060210_0001;
	else 
		return;

	std::string strYYYYMMDD;
	m_WarDeclareDateView.GetLocalString_YYYYMMDD(m_WarDeclareDateView.Year, m_WarDeclareDateView.Month, m_WarDeclareDateView.Day, strYYYYMMDD);
	// "%s %02d�� %02d�п� %s ���¿��� �������� �Ͻðڽ��ϱ�?"
	sprintf(chBuff, STRMSG_C_090113_0329, strYYYYMMDD.c_str(), m_WarDeclareDateView.Hour, m_WarDeclareDateView.Minute, szEnemyInflName);
	g_pGameMain->m_pInfWindow->AddMsgBox(chBuff, _Q_WAR_DECLARE);
}

void	CINFWarDeclare::OnMouseMoveCal(const POINT pt)
{
	int jIndex = (pt.y - m_nWarDeclarePosY - MOTHERSHIP_WAR_DECLARE_DAY_RECT_Y) / MOTHERSHIP_WAR_DECLARE_CAL_GAP_H - 1;
	int iIndex = (pt.x - m_nWarDeclarePosX - MOTHERSHIP_WAR_DECLARE_DAY_RECT_X) / m_pCalendarRect->GetImgSize().x;

	// �ú��ʴ� ������ ���� ���� �Ⱓ���� ����Ѵ�
	ATUM_DATE_TIME selectDay, start(m_WarDeclareEnableStartPeriod), end(m_WarDeclareEnableEndPeriod);
	selectDay.SetDateTime(m_Calendar[jIndex][iIndex].nYear, m_Calendar[jIndex][iIndex].nMonth, m_Calendar[jIndex][iIndex].nDay, 0, 0, 0);
	start.Hour	=	end.Hour	= 0;
	start.Minute=	end.Minute	= 0;
	start.Second=	end.Second	= 0;
	
	// ���� ���� �Ⱓ�̸� ����
	if(selectDay >= start && selectDay <= end)
		g_pGameMain->SetToolTip(pt.x, pt.y, STRMSG_C_090113_0317);//"\\y������ �� ���� ��¥�� ���� �մϴ�.\\y"
}

void	CINFWarDeclare::CalcCalendarRect(void)
{
	m_CalendarMouseRect.top		= m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_DAY_RECT_Y + (MOTHERSHIP_WAR_DECLARE_CAL_GAP_H);
	m_CalendarMouseRect.bottom	= m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_DAY_RECT_Y + ((MAX_CALENDAR_LINE + 1) * MOTHERSHIP_WAR_DECLARE_CAL_GAP_H);
	m_CalendarMouseRect.left	= m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_DAY_RECT_X;
	m_CalendarMouseRect.right	= m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_DAY_RECT_X + (MAX_DAY * m_pCalendarRect->GetImgSize().x);
}

int		CINFWarDeclare::WndProcWarDeclare(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!m_bWarDeclareAct)
		return INF_MSGPROC_NORMAL;

	switch(uMsg)
	{
		case WM_MOUSEMOVE:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				if(m_bLButtonClick)
				{
					m_nWarDeclarePosX	+= pt.x - m_ptOldPoint.x;
					m_nWarDeclarePosY	+= pt.y - m_ptOldPoint.y;

					POINT ptImg			= m_pImgWarDeclareBG->GetImgSize();

					if(m_nWarDeclarePosX < 0)
						m_nWarDeclarePosX = 0;

					if(m_nWarDeclarePosX > g_pD3dApp->GetWidth() - ptImg.x)
						m_nWarDeclarePosX = g_pD3dApp->GetWidth() - ptImg.x;

					if(m_nWarDeclarePosY < 0)
						m_nWarDeclarePosY = 0;

					if(m_nWarDeclarePosY > g_pD3dApp ->GetHeight() - ptImg.y)
						m_nWarDeclarePosY = g_pD3dApp ->GetHeight() - ptImg.y;

					CalcCalendarRect();

					m_ptOldPoint = pt;
				}

				m_pCalendarLeftBtn->OnMouseMove(pt);
				m_pCalendarRightBtn->OnMouseMove(pt);
				
				m_pWarDeclareNoonSelBtnUp->OnMouseMove(pt);
				m_pWarDeclareNoonSelBtnDown->OnMouseMove(pt);
				
				m_pGiveupWarBtn->OnMouseMove(pt);
				m_pSaveWarDeclareOptionBtn->OnMouseMove(pt);
				m_pCloseDeclareOption->OnMouseMove(pt);

				g_pGameMain->SetToolTip(0, 0, NULL);
				
				// �޷� ������ ���ԉ�ٸ?
				if(PtInRect(&m_CalendarMouseRect, pt))
				{
					OnMouseMoveCal(pt);
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

				vector<CINFMessageBox*>::iterator it = g_pGameMain->m_pInfWindow->m_vecMessageBox.begin();
				while(it != g_pGameMain->m_pInfWindow->m_vecMessageBox.end())
				{				
					CINFMessageBox* pMsgBox = *it;
					// �޼��� �ڽ� �����(��ȯ)
					if(_Q_WAR_DECLARE_GIVE_UP == pMsgBox->m_nMsgType)
						return INF_MSGPROC_BREAK;

					if(_Q_WAR_DECLARE == pMsgBox->m_nMsgType)
						return INF_MSGPROC_BREAK;
				}

				if( pt.x > m_nWarDeclarePosX
					&& pt.y > m_nWarDeclarePosY
					&& pt.x < m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_BAR_W
					&& pt.y < m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_BAR_H )
				{
					m_bLButtonClick = TRUE;
					m_ptOldPoint	= pt;
				}

				m_pCalendarLeftBtn->OnLButtonDown(pt);
				m_pCalendarRightBtn->OnLButtonDown(pt);
				
				m_pWarDeclareNoonSelBtnUp->OnLButtonDown(pt);
				m_pWarDeclareNoonSelBtnDown->OnLButtonDown(pt);

				if(m_pWarDeclareHourEditBox->OnLButtonDown(pt))
				{
					m_pWarDeclareHourEditBox->EnableEdit(TRUE);
					m_pWarDeclareMinEditBox->EnableEdit(FALSE);
				}
				
				if(m_pWarDeclareMinEditBox->OnLButtonDown(pt))
				{
					m_pWarDeclareMinEditBox->EnableEdit(TRUE);
					m_pWarDeclareHourEditBox->EnableEdit(FALSE);
				}

				m_pGiveupWarBtn->OnLButtonDown(pt);
				m_pSaveWarDeclareOptionBtn->OnLButtonDown(pt);
				m_pCloseDeclareOption->OnLButtonDown(pt);

				return INF_MSGPROC_BREAK;
			}
			break;

		case WM_LBUTTONUP:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);
				m_bLButtonClick = FALSE;
				
				// �޷� ���� ��ư
				if( m_pCalendarLeftBtn->OnLButtonUp(pt) )
					OnClickCalLeft();

				// �޷� ������ ��ư
				if( m_pCalendarRightBtn->OnLButtonUp(pt) )
					OnClickCalRight();

				// �������� ��ư
				if( m_pWarDeclareNoonSelBtnUp->OnLButtonUp(pt) )
					OnClickNoonSel();

				// �������� ��ư
				if( m_pWarDeclareNoonSelBtnDown->OnLButtonUp(pt) )
					OnClickNoonSel();

				// ���� ��ư
				if(m_pGiveupWarBtn->OnLButtonUp(pt))
					OnClickGiveUpWar();

				// ���� ��ư
				if(m_pSaveWarDeclareOptionBtn->OnLButtonUp(pt))
					OnClickSaveBtn();

				// �ݱ� ��ư
				if( m_pCloseDeclareOption->OnLButtonUp(pt) )
					HideWarDeclare();

				// �޷� ������ ���ԉ�ٸ?
				if(PtInRect(&m_CalendarMouseRect, pt))
					OnClickCalendar(pt);

				return INF_MSGPROC_BREAK;
			}
			break;

		case WM_KEYDOWN:
			{
				// �� ����Ʈ �ڽ�
				if(m_pWarDeclareHourEditBox->IsEditMode())
				{
					if(m_pWarDeclareHourEditBox->WndProc(uMsg, wParam, lParam))
						return INF_MSGPROC_BREAK;
				}

				// �� ����Ʈ �ڽ�
				if(m_pWarDeclareMinEditBox->IsEditMode())
				{
					if(m_pWarDeclareMinEditBox->WndProc(uMsg, wParam, lParam))
						return INF_MSGPROC_BREAK;
				}

				return INF_MSGPROC_BREAK;
			}
			break;
	}
	
	return INF_MSGPROC_NORMAL;
}

void	CINFWarDeclare::SetWarDeclareDate(ATUM_DATE_TIME* pWarDeclareData)
{
	// ���� ���� ��¥ ������Ʈ
	m_WarDeclareDate = *pWarDeclareData;

	// �ð��� 11�����̸� ����
	if(m_WarDeclareDate.Hour < 12)
		m_bWarDeclareNoon = FALSE;
	else
	{// 12�̻��̸� ����
		m_bWarDeclareNoon = TRUE;
		m_WarDeclareDate.Hour -= 12;
	}

	// ������ �����͵� ������Ʈ
	m_WarDeclareDateView = m_WarDeclareDate;
	
	// �� �� ������Ʈ
	char szTemp[MAX_PATH];
	sprintf(szTemp, "%d", m_WarDeclareDate.Hour);
	m_pWarDeclareHourEditBox->SetString(szTemp, MAX_PATH);
	
	sprintf(szTemp, "%d", m_WarDeclareDate.Minute);
	m_pWarDeclareMinEditBox->SetString(szTemp, MAX_PATH);
}
// end 2009. 01. 12 by ckPark ���� ���� �ý���