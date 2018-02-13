// CINFMissionInfo.cpp: implementation of the CINFMissionInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFMissionInfo.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "D3DHanFont.h"
#include "INFIcon.h"
#include "QuestData.h"
#include "ShuttleChild.h"
#include "INFGameMain.h"
#include "INFScrollBar.h"
#include "AtumDatabase.h"
#include "INFWindow.h"
#include "StoreData.h"
#include "ItemInfo.h"
#include "CharacterChild.h"
#include "Interface.h"
#include "SceneData.h"
#include "MonsterData.h"
#include "Background.h"
#include "dxutil.h"
#include "TutorialSystem.h"
#include "GlobalImage.h"
#include "Chat.h"
#include "ClientParty.h"
#include "AtumSound.h"
#include "INFImageEx.h"			// 2011. 10. 10 by jskim UI�ý��� ����

#define MISSION_WINDOW_SIZE_WIDTH			590							// �̼�â �ִ� ũ�� 
#define MISSION_WINDOW_SIZE_HEIGHT			396							// �̼�â �ִ� ũ��

#define	MISSION_COMPENSATIONITEM_MAX		4							// �ִ� ��������ۼ� 4

#define	MISSION_SCROLL_LENGTH				310
#define	MISSION_SCROLL_LENGTH_FLUENCE		392
#define MISSION_SCROLL_LIST_NUMBER			17

#define MISSION_LEFT_TEXT_GAB_X				53
#define MISSION_RIGHT_TEXT_GAB_X			324
#define MISSION_RIGHT_TEXT_GAB_Y			60
#define MISSION_FONT_HEIGHT_GAB				17

#define MISSION_SELECT_MISSION_BAR_WIDTH	243
#define MISSION_SELECT_MISSION_BAR_HEIGHT	17

#define MISSION_GIVEUP_X					500
#define MISSION_GIVEUP_Y					55

#define MISSION_VIEW_ALL					0
#define MISSION_VIEW_SOLVE					1
#define MISSION_VIEW_UNSOLVE				2
#define MISSION_VIEW_INFO_POSX				(g_pD3dApp->GetBackBufferDesc().Width-390)

// ����ϱ� �̼Ǹ� �̵� ��ǥ
#define MISSION_LAUNCH_SHUTTLE_X			(g_pD3dApp->GetBackBufferDesc().Width-127)
#define MISSION_LAUNCH_SHUTTLE_Y			(g_pD3dApp->GetBackBufferDesc().Height-72)
#define MISSION_MAP_MOVE_X					(g_pD3dApp->GetBackBufferDesc().Width-252)
#define MISSION_MAP_MOVE_Y					(g_pD3dApp->GetBackBufferDesc().Height-72)

// ���� ����
#define MISSION_SELECT_COLOR				(RGB(194, 155, 0))
#define MISSION_PROGRESS_COLOR				(RGB(59, 177, 255))
#define MISSION_COMPLETE_COLOR				(RGB(215, 215, 45))
#define MISSION_IMPOSIBLE_COLOR				(RGB(184, 36, 36))
#define MISSION_POSIBLE_COLOR				(RGB(255, 255, 255))

// ���� ���� ���� ����
#define MISSION_ERR_NONE					0
#define MISSION_ERR_LEVEL					100
#define MISSION_ERR_STAT_AT					101
#define MISSION_ERR_STAT_DE					102
#define MISSION_ERR_STAT_FU					103
#define MISSION_ERR_STAT_SO					104
#define MISSION_ERR_STAT_SH					105
#define MISSION_ERR_STAT_DO					106
#define MISSION_ERR_ITEM					107
#define MISSION_ERR_FLUENCE					108
#define MISSION_ERR_EXPERIENCE				109
#define MISSION_ERR_QUEST					110
#define MISSION_ERR_HIDEQUEST				111
#define MISSION_ERR_POSENDQUEST				112

// ���â 
#define MISSION_WARNING_X					((g_pD3dApp->GetBackBufferDesc().Width / 2) - 225)
#define MISSION_WARNING_Y					((g_pD3dApp->GetBackBufferDesc().Height / 2) - 103)
#define MISSION_WARNING_OK_X				MISSION_WARNING_X + 139
#define MISSION_WARNING_OK_Y				MISSION_WARNING_Y + 118
#define MISSION_WARNING_CAN_X				MISSION_WARNING_X + 226
#define MISSION_WARNING_CAN_Y				MISSION_WARNING_Y + 118
#define MISSION_WARNING_INFO_X				MISSION_WARNING_X + 47
#define MISSION_WARNING_INFO_Y				MISSION_WARNING_Y + 73

// 2006-03-07 by ispark, �� ���� ��ġ ����
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn �±� ���� �߰�
#define STRING_CULL ::StringCullingUserData_ToBlank
#define MISSION_LEFT_TEXT_GAB_Y				80//82
#define MISSION_RADIO_Y						4
#define MISSION_MAX_DESC_STRING				220//35
#define MISSION_FLUNCE_MAX_DESC_STRING		312//55
#define MISSION_TOOLTIB_Y					30
#else
#define STRING_CULL ::StringCullingUserDataEx
#define MISSION_LEFT_TEXT_GAB_Y				80//82
#define MISSION_RADIO_Y						4//2
#define MISSION_MAX_DESC_STRING				220//35
#define MISSION_FLUNCE_MAX_DESC_STRING		312//55
#define MISSION_TOOLTIB_Y					30
#endif // _DEBUG_endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFMissionInfo::CINFMissionInfo()
{
	FLOG( "CINFMissionInfo()" );
	
	int nCount = 0;
	// Image
	for(nCount=0; nCount<IMAGE_MAX_COUNT; nCount++)
		m_pMissionImg[nCount] = NULL;
	
	// Font
	for(nCount=0;nCount<LEFTFONT_MAX_LINE_COUNT;nCount++)
		m_pMissionFontLeft[nCount] = NULL;
	for(nCount=0;nCount<RIGHTFONT_MAX_LINE_COUNT;nCount++)
		m_pMissionFontRight[nCount] = NULL;
	for(nCount=0;nCount<MISSION_SCROLL_FLUENCE_MAX_LINE;nCount++)
	{
		m_pMissionFontfluenceL[nCount] = NULL;
		m_pMissionFontfluenceR[nCount] = NULL;
	}		
	
	m_pScrollLeftWindow  = NULL;
	m_pScrollRightWindow = NULL;
	m_pCompensation = NULL;
	for(nCount=0;nCount<MISSION_TARGET_LIST;nCount++)
		m_pMissionTarget[nCount] = NULL;

	m_PointMissionPos.x	= 0;
	m_PointMissionPos.y = 0;
	m_PointMissionPosOld.x = 0;
	m_PointMissionPosOld.y = 0;
	m_PointMissionMouse.x = 0;
	m_PointMissionMouse.y = 0;
	m_bMissionWindowStart = FALSE;
	m_nMissionGiveUp = 0;
	m_nSelectMissionTab = MISS_SCEN;
	m_nSelectMissionIndex = -1;
	m_nMissionViewCase = MISSION_VIEW_ALL;
	m_bProgressMission = FALSE;

	m_vecMissionList.clear();
	m_vecTabMissionNum.clear();

	m_vecCompensationItemName.clear();
	m_vecPossibleDesc.clear();
//	ZERO_MEMORY(m_strMissionName);
	m_bMoveMissionWindows = FALSE;
	m_nMissionButtonStateStart  = MISS_START_NOT;
	m_nMissionButtonStateEnd	= MISS_COPLETE_N;
	m_nMissionButtonMapMove		= MISS_MAP_GO_N;
	m_bButtonStateStart = FALSE;
	m_bButtonStateEnd = FALSE;

	m_nLaunchShuttleState = MISS_LAUNCH_SHUTTLE_N;
	m_nMissionMapMove = MISS_MAP_MOVE_N;
	m_bLaunchShuttleState = FALSE;
	m_bMissionMapMove = FALSE;
	m_nMissionMapIndex = 0;
	m_bFlagEnterShop = FALSE;
	m_bMissionComButtonFlag = FALSE;
	m_bMissionStartButtonUse = FALSE;
	m_bButtonStateMapMove = FALSE;
	m_nVCUButton = MISS_INFLUENCE_NR;
	m_bVCUButton = FALSE;
	m_nANIButton = MISS_INFLUENCE_NR;
	m_bANIButton = FALSE;
	m_pScrollLeftfluence = NULL;
	m_pScrollRightfluence = NULL;
	m_vecVCUfluenceDesc.clear();
	m_vecANIfluenceDesc.clear();
	m_nOldMissionIndex = 0;
	m_vecOldString.clear();
	ZERO_MEMORY(m_nCompensation);
	ZERO_MEMORY(m_bCompenToolTib);
	m_vecErrPart.clear();
	m_vecErrQuestName.clear();
	m_vecErritem.clear();
	m_bSearchPosQuest = FALSE;
	m_bPosMissionComplete = FALSE;
	m_bViewMissionSelectFirst = FALSE;
	m_nMaxNum = 0;

	m_fVCNInflDistributionPercent = 0.0f;
	m_fANIInflDistributionPercent = 0.0f;

	m_nInflChoiceOver = INFLUENCE_TYPE_NORMAL;
	m_nInflChoiceOKButton = -1;
	m_nInflChoiceCANButton = -1;

	int i;
	for(i = 0; i < 4; i++)
	{
		m_pImgOkButton[i] = NULL;
		m_pImgCancelButton[i] = NULL;
	}
	for(i = 0; i < 3; i++)
	{
		m_pInfluenceWarning[i] = NULL;
	}
	m_pImgWarning = NULL;
	m_vecMissionName.clear();

	
}

CINFMissionInfo::~CINFMissionInfo()
{
	FLOG( "~CINFMissionInfo()" );
	
	int nCount = 0;
	// Image
	for(nCount=0; nCount<IMAGE_MAX_COUNT; nCount++)
		util::del(m_pMissionImg[nCount]);	
	
	// Font
	for(nCount=0;nCount<LEFTFONT_MAX_LINE_COUNT;nCount++)
		util::del(m_pMissionFontLeft[nCount]);
	for(nCount=0;nCount<RIGHTFONT_MAX_LINE_COUNT;nCount++)
		util::del(m_pMissionFontRight[nCount]);
	for(nCount=0;nCount<MISSION_TARGET_LIST;nCount++)
		util::del(m_pMissionTarget[nCount]);
	for(nCount=0;nCount<MISSION_SCROLL_FLUENCE_MAX_LINE;nCount++)
	{
		util::del(m_pMissionFontfluenceL[nCount]);
		util::del(m_pMissionFontfluenceR[nCount]);
	}

	util::del(m_pCompensation);
	util::del(m_pScrollLeftWindow);
	util::del(m_pScrollRightWindow);
	util::del(m_pScrollLeftfluence);
	util::del(m_pScrollRightfluence);

	int i;
	for(i = 0; i < 4; i++)
	{
		util::del(m_pImgOkButton[i]);
		util::del(m_pImgCancelButton[i]);
	}
	for(i = 0; i < 3; i++)
	{	
		util::del(m_pInfluenceWarning[i]);
	}
	util::del(m_pImgWarning);
	m_vecMissionName.clear();
}

HRESULT CINFMissionInfo::InitDeviceObjects()
{
	FLOG( "CINFMissionInfo::InitDeviceObjects(char* pData, int nSize)" );

	DataHeader	* pDataHeader ;
	char buf[IMAGE_MAX_COUNT][32];
	ZERO_MEMORY(buf);
	int nCount = 0;
	
	InitMission();

	wsprintf( buf[MISS_BACK],			"misbak");
	wsprintf( buf[MISS_NOM],			"misnor");
	wsprintf( buf[MISS_SCEN],			"missce");
	wsprintf( buf[MISS_SCEN_D],			"misscd");
	wsprintf( buf[MISS_CANCEL],			"misgive");
	wsprintf( buf[MISS_SEL_BACK],		"missel");
	wsprintf( buf[MISS_SEL_RADIO],		"misradn");
	wsprintf( buf[MISS_RADIO],			"misradn");
	wsprintf( buf[MISS_RADIO_S],		"radio_a");	
	
	wsprintf( buf[MISS_START_N],		"miss_c");	
	wsprintf( buf[MISS_START_O],		"miss_o");	
	wsprintf( buf[MISS_START_S],		"miss_s");	
	wsprintf( buf[MISS_START_NOT],		"missnot");	
	wsprintf( buf[MISS_COPLETE_N],		"mistarn");	
	wsprintf( buf[MISS_COPLETE_O],		"mistaro");	
	wsprintf( buf[MISS_COPLETE_S],		"mistars");	
	wsprintf( buf[MISS_COPLETE_NOT],	"mistanot");
	
	wsprintf( buf[MISS_LAUNCH_SHUTTLE_N],	"goshn");
	wsprintf( buf[MISS_LAUNCH_SHUTTLE_O],	"gosho");
	wsprintf( buf[MISS_LAUNCH_SHUTTLE_S],	"goshs");
	wsprintf( buf[MISS_LAUNCH_SHUTTLE_NOT],	"goshnot");
	wsprintf( buf[MISS_MAP_MOVE_N],			"migon");
	wsprintf( buf[MISS_MAP_MOVE_O],			"migoo");
	wsprintf( buf[MISS_MAP_MOVE_S],			"migos");
	wsprintf( buf[MISS_MAP_MOVE_NOT],		"migonot");	
	wsprintf( buf[MISS_MAP_GO_N],			"mgdflt");
	wsprintf( buf[MISS_MAP_GO_O],			"mgov");
	wsprintf( buf[MISS_MAP_GO_S],			"mgpsh");
	wsprintf( buf[MISS_MAP_GO_NOT],			"mgdis");
	
	wsprintf( buf[MISS_INFLUENCE_BK],		"frc_bk");	// ���� �̹���
	wsprintf( buf[MISS_INFLUENCE_NR],		"frc_nr");
	wsprintf( buf[MISS_INFLUENCE_PS],		"frc_psh");
	wsprintf( buf[MISS_INFLUENCE_OV],		"frc_ov");


	for(nCount=0; nCount<IMAGE_MAX_COUNT; nCount++)
	{
		m_pMissionImg[nCount] = new CINFImageEx;
		pDataHeader = FindResource(buf[nCount]);
		m_pMissionImg[nCount]->InitDeviceObjects(pDataHeader) ;
	}

	// Font
	for(nCount=0;nCount<LEFTFONT_MAX_LINE_COUNT;nCount++){
		m_pMissionFontLeft[nCount] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9,
			D3DFONT_ZENABLE, TRUE,1024,32);
		m_pMissionFontLeft[nCount]->InitDeviceObjects(g_pD3dDev) ;
	}
	for(nCount=0;nCount<RIGHTFONT_MAX_LINE_COUNT;nCount++){
		if(nCount == 0)
		{
			m_pMissionFontRight[nCount] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9,
				D3DFONT_ZENABLE|D3DFONT_BOLD, TRUE,1024,32);
			m_pMissionFontRight[nCount]->InitDeviceObjects(g_pD3dDev) ;
			
		}
		else
		{
			m_pMissionFontRight[nCount] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9,
				D3DFONT_ZENABLE, TRUE,1024,32);
			m_pMissionFontRight[nCount]->InitDeviceObjects(g_pD3dDev) ;
		}
	}

	m_pCompensation = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
	m_pCompensation->InitDeviceObjects(g_pD3dDev) ;

	for(nCount=0;nCount<MISSION_TARGET_LIST;nCount++)
	{
		m_pMissionTarget[nCount] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
		m_pMissionTarget[nCount]->InitDeviceObjects(g_pD3dDev) ;
	}
	for(nCount=0;nCount<MISSION_SCROLL_FLUENCE_MAX_LINE;nCount++)
	{
		m_pMissionFontfluenceL[nCount] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
		m_pMissionFontfluenceL[nCount]->InitDeviceObjects(g_pD3dDev) ;

		m_pMissionFontfluenceR[nCount] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
		m_pMissionFontfluenceR[nCount]->InitDeviceObjects(g_pD3dDev) ;
	}

	// ��ũ�� ����
	m_pScrollLeftWindow = new CINFScrollBar(this,
								m_PointMissionPos.x+295, 
								m_PointMissionPos.y+50, 
								MISSION_SCROLL_LENGTH,
								LEFTFONT_MAX_LINE_COUNT);
	m_pScrollLeftWindow->SetGameData( m_pGameData );
	m_pScrollLeftWindow->InitDeviceObjects();
	m_pScrollLeftWindow->SetWheelRect(
		m_PointMissionPos.x, 
		m_PointMissionPos.y,
		m_PointMissionPos.x+297,
		m_PointMissionPos.y+398);

	m_pScrollRightWindow = new CINFScrollBar(this,
								m_PointMissionPos.x+565, 
								m_PointMissionPos.y+50, 
								MISSION_SCROLL_LENGTH,
								RIGHTFONT_MAX_LINE_COUNT);
	m_pScrollRightWindow->SetGameData( m_pGameData );
	m_pScrollRightWindow->InitDeviceObjects();
	m_pScrollRightWindow->SetWheelRect(
		m_PointMissionPos.x+298,
		m_PointMissionPos.y,
		m_PointMissionPos.x+592,
		m_PointMissionPos.y+398);
	
	// ���� ���ý�ũ�� ����
	POINT pSize;
	pSize.x = 768; 
	pSize.y = 554;

	int nPosX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(pSize.x/2);
	int nPosY = (g_pD3dApp->GetBackBufferDesc().Height/2)-(pSize.y/2);

	m_pScrollLeftfluence = new CINFScrollBar(this,
								nPosX+381, 
								nPosY+86, 
								MISSION_SCROLL_LENGTH_FLUENCE,
								MISSION_SCROLL_FLUENCE_MAX_LINE);
	m_pScrollLeftfluence->SetGameData( m_pGameData );
	m_pScrollLeftfluence->InitDeviceObjects();
	m_pScrollLeftfluence->SetScrollLinePos( nPosX+381, nPosY+86);
	m_pScrollLeftfluence->SetWheelRect(
		nPosX, 
		nPosY,
		nPosX+(pSize.x/2),
		nPosY+pSize.y);
	

	m_pScrollRightfluence = new CINFScrollBar(this,
								nPosX+747, 
								nPosY+86, 
								MISSION_SCROLL_LENGTH_FLUENCE,
								MISSION_SCROLL_FLUENCE_MAX_LINE);
	m_pScrollRightfluence->SetGameData( m_pGameData );
	m_pScrollRightfluence->InitDeviceObjects();
	m_pScrollRightfluence->SetScrollLinePos( nPosX+747, nPosY+86);
	m_pScrollRightfluence->SetWheelRect(
		nPosX+(pSize.x/2), 
		nPosY,
		nPosX+pSize.x,
		nPosY+pSize.y);	
	
	char sbuf[30];
	int i;
	for(i = 0; i < 4; i++)
	{

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	wsprintf(sbuf, "okb0%d", i);
#else
		wsprintf(sbuf, "shlaok0%d", i);
#endif
	
		m_pImgOkButton[i] = new CINFImageEx;
		pDataHeader = FindResource(sbuf);
		m_pImgOkButton[i]->InitDeviceObjects(pDataHeader);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		wsprintf(sbuf, "canb0%d", i);
#else
		wsprintf(sbuf, "shmcan0%d", i);
#endif


		m_pImgCancelButton[i] = new CINFImageEx;
		pDataHeader = FindResource(sbuf);
		m_pImgCancelButton[i]->InitDeviceObjects(pDataHeader);
	}

	wsprintf(sbuf, "inftex");
	m_pImgWarning = new CINFImageEx;
	pDataHeader = FindResource(sbuf);
	m_pImgWarning->InitDeviceObjects(pDataHeader);

	for(i = 0; i < 3; i++)
	{
		m_pInfluenceWarning[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
		m_pInfluenceWarning[i]->InitDeviceObjects(g_pD3dDev) ;
	}
	return S_OK;
}

HRESULT CINFMissionInfo::RestoreDeviceObjects()
{
	FLOG( "CINFMissionInfo::RestoreDeviceObjects()" );

	int nCount = 0;
	// Image
	for(nCount=0; nCount<IMAGE_MAX_COUNT; nCount++)
		m_pMissionImg[nCount]->RestoreDeviceObjects();
	
	// Font
	for(nCount=0;nCount<LEFTFONT_MAX_LINE_COUNT;nCount++)
		m_pMissionFontLeft[nCount]->RestoreDeviceObjects();
	for(nCount=0;nCount<RIGHTFONT_MAX_LINE_COUNT;nCount++)
		m_pMissionFontRight[nCount]->RestoreDeviceObjects();
	for(nCount=0;nCount<MISSION_TARGET_LIST;nCount++)
		m_pMissionTarget[nCount]->RestoreDeviceObjects();
	for(nCount=0;nCount<MISSION_SCROLL_FLUENCE_MAX_LINE;nCount++)
	{
		m_pMissionFontfluenceL[nCount]->RestoreDeviceObjects();
		m_pMissionFontfluenceR[nCount]->RestoreDeviceObjects();
	}

	m_pCompensation->RestoreDeviceObjects();	

	// ��ũ�� ����
	m_pScrollLeftWindow->RestoreDeviceObjects();
	m_pScrollRightWindow->RestoreDeviceObjects();
	m_pScrollLeftfluence->RestoreDeviceObjects();
	m_pScrollRightfluence->RestoreDeviceObjects();

	// ��ũ�� ����
	POINT pSize = m_pMissionImg[MISS_INFLUENCE_BK]->GetImgSize();
	m_nInfluencePosX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(pSize.x/2);
	m_nInfluencePosY = (g_pD3dApp->GetBackBufferDesc().Height/2)-(pSize.y/2);	

	m_pScrollLeftfluence->SetScrollLinePos( m_nInfluencePosX+381, m_nInfluencePosY+86);
	m_pScrollLeftfluence->SetWheelRect(
		m_nInfluencePosX, 
		m_nInfluencePosY,
		m_nInfluencePosX+(pSize.x/2),
		m_nInfluencePosY+pSize.y);	
	
	m_pScrollRightfluence->SetScrollLinePos( m_nInfluencePosX+747, m_nInfluencePosY+86);
	m_pScrollRightfluence->SetWheelRect(
		m_nInfluencePosX+(pSize.x/2),
		m_nInfluencePosY,
		m_nInfluencePosX+pSize.x,
		m_nInfluencePosY+pSize.y);	
	
	int i;
	for(i = 0; i < 4; i++)
	{
		m_pImgOkButton[i]->RestoreDeviceObjects();
		m_pImgCancelButton[i]->RestoreDeviceObjects();
	}

	m_pImgWarning->RestoreDeviceObjects();
	for(i = 0; i < 3; i++)
	{
		m_pInfluenceWarning[i]->RestoreDeviceObjects();
	}
	return S_OK;
}

HRESULT CINFMissionInfo::InvalidateDeviceObjects()
{
	FLOG( "CINFMissionInfo::InvalidateDeviceObjects()" );

	int nCount = 0;
	// Image
	for(nCount=0; nCount<IMAGE_MAX_COUNT; nCount++)
		m_pMissionImg[nCount]->InvalidateDeviceObjects();
	
	// Font
	for(nCount=0;nCount<LEFTFONT_MAX_LINE_COUNT;nCount++)
		m_pMissionFontLeft[nCount]->InvalidateDeviceObjects();
	for(nCount=0;nCount<RIGHTFONT_MAX_LINE_COUNT;nCount++)
		m_pMissionFontRight[nCount]->InvalidateDeviceObjects();
	for(nCount=0;nCount<MISSION_TARGET_LIST;nCount++)
		m_pMissionTarget[nCount]->InvalidateDeviceObjects();
	for(nCount=0;nCount<MISSION_SCROLL_FLUENCE_MAX_LINE;nCount++)
	{
		m_pMissionFontfluenceL[nCount]->InvalidateDeviceObjects();
		m_pMissionFontfluenceR[nCount]->InvalidateDeviceObjects();
	}

	m_pCompensation->InvalidateDeviceObjects();

	// ��ũ�� ����
	m_pScrollLeftWindow->InvalidateDeviceObjects();
	m_pScrollRightWindow->InvalidateDeviceObjects();
	m_pScrollLeftfluence->InvalidateDeviceObjects();
	m_pScrollRightfluence->InvalidateDeviceObjects();	
	
	int i;
	for(i = 0; i < 4; i++)
	{
		m_pImgOkButton[i]->InvalidateDeviceObjects();
		m_pImgCancelButton[i]->InvalidateDeviceObjects();
	}

	m_pImgWarning->InvalidateDeviceObjects();
	for(i = 0; i < 3; i++)
	{
		m_pInfluenceWarning[i]->InvalidateDeviceObjects();
	}
	return S_OK;
}

HRESULT CINFMissionInfo::DeleteDeviceObjects()
{
	FLOG( "CINFMissionInfo::DeleteDeviceObjects()" );
	
	int nCount = 0;
	// Image
	for(nCount=0; nCount<IMAGE_MAX_COUNT; nCount++){
		m_pMissionImg[nCount]->DeleteDeviceObjects();	
		util::del(m_pMissionImg[nCount]);
	}	

	// Font
	for(nCount=0;nCount<LEFTFONT_MAX_LINE_COUNT;nCount++){
		m_pMissionFontLeft[nCount]->DeleteDeviceObjects();
		util::del(m_pMissionFontLeft[nCount]);
	}
	for(nCount=0;nCount<RIGHTFONT_MAX_LINE_COUNT;nCount++){
		m_pMissionFontRight[nCount]->DeleteDeviceObjects();
		util::del(m_pMissionFontRight[nCount]);
	}
	for(nCount=0;nCount<MISSION_TARGET_LIST;nCount++){
		m_pMissionTarget[nCount]->DeleteDeviceObjects();
		util::del(m_pMissionTarget[nCount]);
	}
	for(nCount=0;nCount<MISSION_SCROLL_FLUENCE_MAX_LINE;nCount++){
		m_pMissionFontfluenceL[nCount]->DeleteDeviceObjects();
		util::del(m_pMissionFontfluenceL[nCount]);
		m_pMissionFontfluenceR[nCount]->DeleteDeviceObjects();
		util::del(m_pMissionFontfluenceR[nCount]);
	}
	m_pCompensation->DeleteDeviceObjects();
	util::del(m_pCompensation);

	// ��ũ�� ����
	m_pScrollLeftWindow->DeleteDeviceObjects();
	util::del(m_pScrollLeftWindow);
	m_pScrollRightWindow->DeleteDeviceObjects();
	util::del(m_pScrollRightWindow);
	
	m_pScrollLeftfluence->DeleteDeviceObjects();
	util::del(m_pScrollLeftfluence);
	m_pScrollRightfluence->DeleteDeviceObjects();
	util::del(m_pScrollRightfluence);
	
	int i;
	for(i = 0; i < 4; i++)
	{
		m_pImgOkButton[i]->DeleteDeviceObjects();
		util::del(m_pImgOkButton[i]);
		m_pImgCancelButton[i]->DeleteDeviceObjects();
		util::del(m_pImgCancelButton[i]);
	}

	m_pImgWarning->DeleteDeviceObjects();
	util::del(m_pImgWarning);
	for(i = 0; i < 3; i++)
	{
		m_pInfluenceWarning[i]->DeleteDeviceObjects();
		util::del(m_pInfluenceWarning[i]);
	}
	return S_OK;
}

void CINFMissionInfo::Render()
{
	FLOG( "CINFMissionInfo::Render()" );

	if(TRUE == m_bInfluenceFlag)
	{	//////////////////////////////////////////////////////////////////////////
		// ���¹̼� ������ ���̸� ���� �̼����� ������ ����
		// ����ϱ� �̼Ǹ��̵� �̼� ������
		// 2005-08-16 by ydkim
		//////////////////////////////////////////////////////////////////////////
		RenderInfluenceMission();
		return;
	}

	if(m_bMissionWindowStart == TRUE)
	{
		// ó�� �̼ǽ��۽� ù���̼� ���� ���̱�
		if(m_bViewMissionSelectFirst == TRUE)
		{
			if(m_nSelectMissionIndex == -1)
				m_nSelectMissionIndex = 0;
			
			// �� �� �ִ� �̼� ����(����)
			StartPossibleMission();

			m_bViewMissionSelectFirst = FALSE;
		}

		// ��� ������
		m_pMissionImg[MISS_BACK]->Move( m_PointMissionPos.x, m_PointMissionPos.y);
		m_pMissionImg[MISS_BACK]->Render();

		// �̼� �Ƿ����� 
		m_pMissionImg[m_nSelectMissionTab]->Move( m_PointMissionPos.x+15, m_PointMissionPos.y+24);
		m_pMissionImg[m_nSelectMissionTab]->Render();

		// �̼� ���� ��ư
		m_pMissionImg[MISS_SEL_RADIO]->Move( m_PointMissionPos.x+38,  m_PointMissionPos.y+347);
		m_pMissionImg[MISS_SEL_RADIO]->Render();
		m_pMissionImg[MISS_SEL_RADIO]->Move( m_PointMissionPos.x+98, m_PointMissionPos.y+347);
		m_pMissionImg[MISS_SEL_RADIO]->Render();
		m_pMissionImg[MISS_SEL_RADIO]->Move( m_PointMissionPos.x+188, m_PointMissionPos.y+347);
		m_pMissionImg[MISS_SEL_RADIO]->Render();
		
		switch(m_nMissionViewCase)
		{
		case MISSION_VIEW_ALL:
			m_pMissionImg[MISS_RADIO_S]->Move( m_PointMissionPos.x+36,  m_PointMissionPos.y+345);
			break;
		case MISSION_VIEW_SOLVE:
			m_pMissionImg[MISS_RADIO_S]->Move( m_PointMissionPos.x+96,  m_PointMissionPos.y+345);
			break;
		case MISSION_VIEW_UNSOLVE:
			m_pMissionImg[MISS_RADIO_S]->Move( m_PointMissionPos.x+186,  m_PointMissionPos.y+345);
			break;
		}
		m_pMissionImg[MISS_RADIO_S]->Render();

		// �̼� ����Ʈ ������
		RenderMissionList();

		// �Ҽ� �ִ� �̼� ������
		// ���� ���� ���� ���� ����
		RenderPossibleMission();

		// ��ũ�� ������
		m_pScrollLeftWindow->Render();
		m_pScrollRightWindow->Render();
	}

	// �̼� ��ǥ ������
//	RenderMissionTarget();
	
	// ����ϱ� �̼Ǹ��̵�
	// 2006-03-24 by ispark, ��Ƽ�̼��ϴ� ��Ƽ�常 ��ư ���̱� �߰�
	// 2006-07-21 by ispark, ����ʿ����� �̼�â ���� ���Ѵ�.
	// 2007-03-27 by bhsohn ������ ���� �������̽� ����
	//if(g_pD3dApp->m_bCharacter == TRUE && g_pInterface->m_bShowInterface && m_bFlagEnterShop == FALSE && !g_pD3dApp->m_bBazaar)
	if(g_pD3dApp->m_bCharacter == TRUE && !g_pShuttleChild->IsObserverMode() && g_pInterface->m_bShowInterface && m_bFlagEnterShop == FALSE && !g_pD3dApp->m_bBazaar)
	{	
		// �̼� �Ƿ����� 
		m_pMissionImg[m_nLaunchShuttleState]->Move( MISSION_LAUNCH_SHUTTLE_X, MISSION_LAUNCH_SHUTTLE_Y);
		m_pMissionImg[m_nLaunchShuttleState]->Render();		
		
		// �̼Ǹ� �̵� (OutDoor)
		if(m_pQuestInfoProgOutDoor && m_pQuestInfoProgOutDoor->StartMapIndex>0 && 
			(!m_pQuestInfoProgOutDoor->IsPartyQuest() || (m_pQuestInfoProgOutDoor->IsPartyQuest() && g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)))
		{
			m_pMissionImg[m_nMissionMapMove]->Move( MISSION_MAP_MOVE_X, MISSION_MAP_MOVE_Y);
			m_pMissionImg[m_nMissionMapMove]->Render();	
		}
		// 2006-03-24 by ispark, ��Ƽ�̼��ϴ� ��Ƽ�常 ��ư ���̱� �߰�
		if(m_pQuestInfoProgOutDoor && m_pQuestInfoProg && m_pQuestInfoProg->StartMapIndex>0 && m_bMissionWindowStart == TRUE && 
			(!m_pQuestInfoProgOutDoor->IsPartyQuest() || (m_pQuestInfoProgOutDoor->IsPartyQuest() && g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)))
		{			
			// �̼� â�� �̼Ǹ��̵� ������
			m_pMissionImg[m_nMissionButtonMapMove]->Move( m_PointMissionPos.x+430,	m_PointMissionPos.y+290);
			m_pMissionImg[m_nMissionButtonMapMove]->Render();
		}
	}
}

void CINFMissionInfo::RenderMissionList()
{
	FLOG( "CINFMissionInfo::RenderMissionList()" );

	// ���� �̼Ǹ���Ʈ
	if(m_nSelectMissionIndex >= m_pScrollLeftWindow->GetCurrentScrollIndex() && 
		m_nSelectMissionIndex < m_pScrollLeftWindow->GetCurrentScrollIndex()+LEFTFONT_MAX_LINE_COUNT)
	{
		m_pMissionImg[MISS_SEL_BACK]->Move( m_PointMissionPos.x+35, 
			m_PointMissionPos.y+80+((m_nSelectMissionIndex-m_pScrollLeftWindow->GetCurrentScrollIndex())*MISSION_SELECT_MISSION_BAR_HEIGHT));
		m_pMissionImg[MISS_SEL_BACK]->Render();
	}

	// �̼� �̸� ���
	for(int i=0; i<LEFTFONT_MAX_LINE_COUNT; i++)
	{
		if(m_vecMissionList.size() <= i+m_pScrollLeftWindow->GetCurrentScrollIndex() || m_vecMissionList.size() == 0)
			break;
		
		m_pMissionImg[MISS_RADIO]->Move( m_PointMissionPos.x+MISSION_LEFT_TEXT_GAB_X-12,
				m_PointMissionPos.y+MISSION_LEFT_TEXT_GAB_Y+(i*MISSION_FONT_HEIGHT_GAB)+MISSION_RADIO_Y);
		m_pMissionImg[MISS_RADIO]->Render();

		if( m_nSelectMissionIndex == i+m_pScrollLeftWindow->GetCurrentScrollIndex() )
		{
			m_pMissionFontLeft[i]->DrawText(m_PointMissionPos.x+MISSION_LEFT_TEXT_GAB_X,
				m_PointMissionPos.y+MISSION_LEFT_TEXT_GAB_Y+(i*MISSION_FONT_HEIGHT_GAB),
				MISSION_SELECT_COLOR, (char*)m_vecMissionList[i+m_pScrollLeftWindow->GetCurrentScrollIndex()].c_str());
		}
		else
		{
			int nindex = m_vecTabMissionNum[i+m_pScrollLeftWindow->GetCurrentScrollIndex()];
			DWORD nColor = MISSION_IMPOSIBLE_COLOR;
			if(g_pQuestData->IsQuestCompleted(nindex))
			{
				nColor = MISSION_COMPLETE_COLOR;
			}
			else if(g_pQuestData->IsQuestProgress(nindex))
			{
				nColor = MISSION_PROGRESS_COLOR;
			}
			else
			{
				 CQuest* pQuest = g_pQuestData->FindQuest(nindex);
				 if(pQuest)
				 {
					 if( CheckProgressMission(pQuest) == TRUE)
						 nColor = MISSION_POSIBLE_COLOR;
				 }
			}

			m_pMissionFontLeft[i]->DrawText(m_PointMissionPos.x+MISSION_LEFT_TEXT_GAB_X,
				m_PointMissionPos.y+MISSION_LEFT_TEXT_GAB_Y+(i*MISSION_FONT_HEIGHT_GAB),
				nColor, (char*)m_vecMissionList[i+m_pScrollLeftWindow->GetCurrentScrollIndex()].c_str());
		}
	}
//	m_pMissionFontLeft[LEFTFONT_MAX_LINE_COUNT]
//	m_vecMissionList
}

void CINFMissionInfo::Tick()
{
	FLOG( "CINFMissionInfo::Tick()" );
	
	if(m_bSearchPosQuest == TRUE && m_pHidePosQuest)
	{	// Ư�� �����ǿ� ����Ʈ�� �ִ°�� �˻� �� �����Ѵ�.
		CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
		if(pQuestInfo == NULL)
		{
			int X = (int)(g_pD3dApp->m_pShuttleChild->m_vPos.x);
			int Z = (int)(g_pD3dApp->m_pShuttleChild->m_vPos.z);

			if(CheckProgressMission(m_pHidePosQuest))
			{
				char buff[512];
				ZERO_MEMORY(buff);
				wsprintf(buff, STRMSG_C_050825_0001, m_pHidePosQuest->QuestName); //"�̼� %s��(��) �����Ͻðڽ��ϱ�?"
				g_pGameMain->m_pInfWindow->AddMsgBox(buff,_Q_MISSION_START, (DWORD)m_pHidePosQuest->QuestIndex);
				m_bSearchPosQuest = FALSE;
			}
		}
	}
	// Ư������ �̼� �Ϸ�
	if(m_bPosMissionComplete == TRUE)
	{
		int nMissionIndex = g_pD3dApp->SerchMissionCondition();
		if(nMissionIndex)
		{
			MissionEnd(nMissionIndex);
			m_bPosMissionComplete = FALSE;
		}
	}

	// �̼� �Ϸ� �ð� ǥ��
	// ����Ʈ �����ð� ǥ��
	if(g_pGameMain->m_bQuestLimitTimeView)
	{	
		float fElapsedTime = g_pD3dApp->GetElapsedTime();
		// 2008-04-07 by bhsohn ����Ʈ �ð� �����Ǵ� ���� ó��
		float fGetQuestTimeElapse = g_pGameMain->GetQuestTimeElapse();
		fGetQuestTimeElapse -= fElapsedTime;				
		g_pGameMain->SetQuestTimeElapse(fGetQuestTimeElapse);
		if(fGetQuestTimeElapse < -5)
		{
			g_pGameMain->m_bQuestLimitTimeView = FALSE;
			// �� ���� �̼���� ������
			CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
			if(pQuestInfo)
			{
				// 2005-08-02 ispark
				if(QUEST_END_TYPE_TIME_OVER == pQuestInfo->QuestEndType)
				{
					// �̼� �Ϸ� ������
					// 2006-03-27 by ispark, ��Ƽ�̼� �߰�
					if(pQuestInfo->IsPartyQuest())
					{
						g_pQuestData->SendFieldSocketQuestRequestSuccessCheck(pQuestInfo->QuestIndex);
					}
					else
					{
						g_pGImage->SetImageInfo(MISSION_SUC, TRUE);
						//g_pD3dApp->SendMissionComplete();
					}
				}
				else
				{
					// �̼� ��� ������
					g_pGImage->SetImageInfo(MISSION_FAI, TRUE);
				}
			}
		}
//		g_pGameMain->m_fTimeElapse -= fElapsedTime;				
//		if(g_pGameMain->m_fTimeElapse < 0)
//		{
//			g_pGameMain->m_fTimeElapse = 1;
//			g_pGameMain->m_nTimeSecond--;
//			
//			if(g_pGameMain->m_nTimeSecond<-5)
//			{
//				g_pGameMain->m_bQuestLimitTimeView = FALSE;
//				// �� ���� �̼���� ������
//				CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
//				if(pQuestInfo)
//				{
//					// 2005-08-02 ispark
//					if(QUEST_END_TYPE_TIME_OVER == pQuestInfo->QuestEndType)
//					{
//						// �̼� �Ϸ� ������
//						// 2006-03-27 by ispark, ��Ƽ�̼� �߰�
//						if(pQuestInfo->IsPartyQuest())
//						{
//							g_pQuestData->SendFieldSocketQuestRequestSuccessCheck(pQuestInfo->QuestIndex);
//						}
//						else
//						{
//							g_pGImage->SetImageInfo(MISSION_SUC, TRUE);
//							//g_pD3dApp->SendMissionComplete();
//						}
//					}
//					else
//					{
//						// �̼� ��� ������
//						g_pGImage->SetImageInfo(MISSION_FAI, TRUE);
//					}
//				}
//			}
//		}				
	}
}

int CINFMissionInfo::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFMissionInfo::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );

	// ���¼��� �̼� ó��
	if(TRUE == m_bInfluenceFlag){
		return WndProcInfluence(uMsg, wParam, lParam);
	}

	// ��ũ�� ó��
	if(m_pScrollLeftWindow && m_pScrollRightWindow && m_bMissionWindowStart == TRUE){
		if(m_pScrollLeftWindow->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK){
			return INF_MSGPROC_BREAK;
		}
		else if(m_pScrollRightWindow->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK){
			return INF_MSGPROC_BREAK;
		}
	}

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			if(m_bMissionWindowStart == TRUE)
			{
				if(m_bMoveMissionWindows == TRUE)
				{
					m_PointMissionPos.x = m_PointMissionPosOld.x + pt.x - m_PointMissionMouse.x;
					m_PointMissionPos.y = m_PointMissionPosOld.y + pt.y - m_PointMissionMouse.y;
					
					// ��ũ�� ����
					m_pScrollLeftWindow->SetScrollLinePos( m_PointMissionPos.x+295, m_PointMissionPos.y+50);
					m_pScrollLeftWindow->SetWheelRect(
						m_PointMissionPos.x, 
						m_PointMissionPos.y,
						m_PointMissionPos.x+297,
						m_PointMissionPos.y+398);
					
					m_pScrollRightWindow->SetScrollLinePos( m_PointMissionPos.x+565, m_PointMissionPos.y+50);
					m_pScrollRightWindow->SetWheelRect(
						m_PointMissionPos.x+298,
						m_PointMissionPos.y,
						m_PointMissionPos.x+592,
						m_PointMissionPos.y+398);
					
					return INF_MSGPROC_BREAK;				
				}
				else if (pt.y>m_PointMissionPos.y+290 && pt.y<m_PointMissionPos.y+307 &&
					pt.x>m_PointMissionPos.x+312 && pt.x<m_PointMissionPos.x+376 &&
					m_bMissionComButtonFlag)
				{
					// �̼ǿϷ�
					if(m_vecTabMissionNum.size() <= 0)
						break;
					if(m_bButtonStateStart == TRUE)
						m_nMissionButtonStateStart = MISS_START_S;
					else
						m_nMissionButtonStateStart = MISS_START_O;
				}
				else if (pt.y>m_PointMissionPos.y+290 && pt.y<m_PointMissionPos.y+307 &&
					pt.x>m_PointMissionPos.x+485 && pt.x<m_PointMissionPos.x+549 &&
					m_bMissionStartButtonUse == FALSE)
				{	// �̼ǽ���
					if(m_vecTabMissionNum.size() <= 0)
						break;
					if(m_bButtonStateEnd == TRUE)
						m_nMissionButtonStateEnd = MISS_COPLETE_S;
					else
						m_nMissionButtonStateEnd = MISS_COPLETE_O;			
				}
				else if(pt.y>m_PointMissionPos.y+290 && pt.y<m_PointMissionPos.y+307 &&
					pt.x>m_PointMissionPos.x+430 && pt.x<m_PointMissionPos.x+550 &&
					m_pQuestInfoProg)
				{	// �̼Ǹ� �̵� ��ư(�̼� â����)
					if(m_bButtonStateMapMove == TRUE)
						m_nMissionButtonMapMove = MISS_MAP_GO_S;
					else
						m_nMissionButtonMapMove = MISS_MAP_GO_O;
				}
				else
				{				
					if(m_vecTabMissionNum.size() <= 0)
					{
						m_nMissionButtonStateStart = MISS_START_NOT;
						m_nMissionButtonStateEnd = MISS_COPLETE_NOT;	
					}
					else
					{
						m_nMissionButtonStateStart = MISS_START_N;
						m_nMissionButtonStateEnd = MISS_COPLETE_N;	
					}
					m_nMissionButtonMapMove = MISS_MAP_GO_N;					
				}
				
				// ��������� ��������.
				g_pGameMain->SetItemInfo( 0, 0, 0, 0);	// 2005-11-30 by ispark
				ZERO_MEMORY(m_bCompenToolTib);
				if(pt.y>m_PointMissionPos.y+330 && pt.y<m_PointMissionPos.y+357)
				{
					for(int i=0;i<COMPENS_MAX;i++)
					{
						if(pt.x>m_PointMissionPos.x+310+(i*52) && pt.x<m_PointMissionPos.x+337+(i*52))
						{
							m_bCompenToolTib[i] = TRUE;
						}
					}
				}
				// 2006-08-25 by dgwoo �̼� â�ȿ��� �κ��� ������������ ��ȿ�� �Ѵ�.
				if (pt.y>m_PointMissionPos.y && pt.y<m_PointMissionPos.y+MISSION_WINDOW_SIZE_HEIGHT &&
					pt.x>m_PointMissionPos.x && pt.x<m_PointMissionPos.x+MISSION_WINDOW_SIZE_WIDTH)
				{
					return INF_MSGPROC_BREAK;
				}
			}
			
			// ��� �� �̼Ǹ��̵�
			if(pt.y>MISSION_LAUNCH_SHUTTLE_Y && pt.y<MISSION_LAUNCH_SHUTTLE_Y+30 &&
				pt.x>MISSION_LAUNCH_SHUTTLE_X && pt.x<MISSION_LAUNCH_SHUTTLE_X+124 &&
				m_bFlagEnterShop == FALSE && g_pD3dApp->m_bCharacter == TRUE)
			{
				if(m_bLaunchShuttleState == TRUE)
					m_nLaunchShuttleState = MISS_LAUNCH_SHUTTLE_S;
				else
					m_nLaunchShuttleState = MISS_LAUNCH_SHUTTLE_O;
			}
			else
			{
				m_nLaunchShuttleState = MISS_LAUNCH_SHUTTLE_N;
			}
			
			if(m_pQuestInfoProgOutDoor &&
				pt.y>MISSION_MAP_MOVE_Y && pt.y<MISSION_MAP_MOVE_Y+30 &&
				pt.x>MISSION_MAP_MOVE_X && pt.x<MISSION_MAP_MOVE_X+124 &&
				m_bFlagEnterShop == FALSE && g_pD3dApp->m_bCharacter == TRUE)
			{
				if(m_bMissionMapMove == TRUE)
					m_nMissionMapMove = MISS_MAP_MOVE_S;
				else
					m_nMissionMapMove = MISS_MAP_MOVE_O;				
			}
			else
			{
				m_nMissionMapMove = MISS_MAP_MOVE_N;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if(m_bMissionWindowStart == TRUE)
			{
				m_bButtonStateEnd = FALSE;
				m_bButtonStateStart = FALSE;

				if(pt.x>m_PointMissionPos.x	&& pt.x<m_PointMissionPos.x+565
					&& pt.y>m_PointMissionPos.y && pt.y<m_PointMissionPos.y+20)
				{
					m_PointMissionMouse = pt;
					m_PointMissionPosOld = m_PointMissionPos;
					m_bMoveMissionWindows = TRUE;				
					g_pInterface->SetWindowOrder(WNDMissionInfo);
				}
				
				if( pt.x>m_PointMissionPos.x+MISSION_LEFT_TEXT_GAB_X
					&& pt.x<m_PointMissionPos.x+MISSION_LEFT_TEXT_GAB_X+MISSION_SELECT_MISSION_BAR_WIDTH)
				{
					// �̼� ����
					for(int i=0; i<LEFTFONT_MAX_LINE_COUNT; i++)
					{
						if(m_vecMissionList.size() <= i+m_pScrollLeftWindow->GetCurrentScrollIndex())
							break;
						
						if( pt.y>m_PointMissionPos.y+MISSION_LEFT_TEXT_GAB_Y+(i*MISSION_FONT_HEIGHT_GAB)
							&& pt.y<m_PointMissionPos.y+MISSION_LEFT_TEXT_GAB_Y+MISSION_SELECT_MISSION_BAR_HEIGHT+(i*MISSION_FONT_HEIGHT_GAB))
						{
							m_nSelectMissionIndex = i+m_pScrollLeftWindow->GetCurrentScrollIndex();
							
							// �� �� �ִ� �̼� ����(����)
							g_pInterface->SetWindowOrder(WNDMissionInfo);
							StartPossibleMission();
							return INF_MSGPROC_BREAK;
						}
					}
				}			
				else if(pt.x>m_PointMissionPos.x+MISSION_GIVEUP_X && pt.x<m_PointMissionPos.x+MISSION_GIVEUP_X+54 &&
					pt.y>m_PointMissionPos.y+MISSION_GIVEUP_Y && pt.y<m_PointMissionPos.y+MISSION_GIVEUP_Y+16)
				{
					// �̼� ����
					if(m_nMissionGiveUp != 0 && m_bProgressMission == TRUE)
					{
						g_pInterface->SetWindowOrder(WNDMissionInfo);
						g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_QUEST_0015,_Q_QUEST_DISCARD,
							m_nMissionGiveUp);//"�ش� �̼��� �����Ͻðڽ��ϱ�?."
						m_nMissionGiveUp = 0;
						m_pQuestInfoProg = NULL;
						m_pQuestInfoProgOutDoor = NULL;
					}
					// �̼� �� �����϶�
					else if(m_nSelectMissionIndex == -1)
					{
						g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_060720_0002, _MESSAGE);
					}
					// �̼� ���� �ȵ�
					else
					{
						g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_060720_0001, _MESSAGE);
					}

					return INF_MSGPROC_BREAK;
				}
				
				if( pt.y>m_PointMissionPos.y+342 && pt.y<m_PointMissionPos.y+362 )
				{
					g_pInterface->SetWindowOrder(WNDMissionInfo);
					// ��ü����, �ذ�̼� ����, ���ذ� �̼Ǻ���
					if(pt.x>m_PointMissionPos.x+40 && pt.x<m_PointMissionPos.x+100){
						InitMission();
						m_nMissionViewCase = MISSION_VIEW_ALL;
						InitMissionTitle();
						m_bMissionStartButtonUse = FALSE;
						return INF_MSGPROC_BREAK;
					}
					else if (pt.x>m_PointMissionPos.x+101 && pt.x<m_PointMissionPos.x+190){
						InitMission();
						m_nMissionViewCase = MISSION_VIEW_SOLVE;
						InitMissionTitle();
						m_bMissionStartButtonUse = TRUE;
						return INF_MSGPROC_BREAK;
					}
					else if(pt.x>m_PointMissionPos.x+191 && pt.x<m_PointMissionPos.x+285){
						InitMission();
						m_nMissionViewCase = MISSION_VIEW_UNSOLVE;
						InitMissionTitle();
						m_bMissionStartButtonUse = TRUE;
						return INF_MSGPROC_BREAK;
					}
					
				}
				else if(pt.y>m_PointMissionPos.y+24 && pt.y<m_PointMissionPos.y+38 )
				{
					g_pInterface->SetWindowOrder(WNDMissionInfo);
					// �Ǽ���
					if(pt.x>m_PointMissionPos.x+15 && pt.x<m_PointMissionPos.x+120){
						m_nSelectMissionTab = MISS_SCEN;
						InitMission();
						InitMissionTitle();
						return INF_MSGPROC_BREAK;
					}
					else if(pt.x>m_PointMissionPos.x+121 && pt.x<m_PointMissionPos.x+192){
						m_nSelectMissionTab = MISS_NOM;
						InitMission();
						InitMissionTitle();
						return INF_MSGPROC_BREAK;
					}
					else if(pt.x>m_PointMissionPos.x+193 && pt.x<m_PointMissionPos.x+300){
						m_nSelectMissionTab = MISS_SCEN_D;
						InitMission();
						InitMissionTitle();
						return INF_MSGPROC_BREAK;
					}
					
				}
				else if (pt.y>m_PointMissionPos.y+290 && pt.y<m_PointMissionPos.y+307 &&
					pt.x>m_PointMissionPos.x+312 && pt.x<m_PointMissionPos.x+376 &&
					m_bMissionComButtonFlag)
				{
					// �̼ǿϷ�
					g_pInterface->SetWindowOrder(WNDMissionInfo);
					if(m_vecTabMissionNum.size() <= 0)
					{
						m_nMissionButtonStateStart = MISS_START_NOT;
						break;
					}					
					m_nMissionButtonStateStart = MISS_START_S;
					m_bButtonStateStart = TRUE;
					return INF_MSGPROC_BREAK;
				}
				else if (pt.y>m_PointMissionPos.y+290 && pt.y<m_PointMissionPos.y+307 &&
					pt.x>m_PointMissionPos.x+485 && pt.x<m_PointMissionPos.x+549 &&
					m_bMissionStartButtonUse == FALSE)
				{	// �̼ǽ���
					g_pInterface->SetWindowOrder(WNDMissionInfo);
					if(m_vecTabMissionNum.size() <= 0)
					{
						m_nMissionButtonStateEnd = MISS_COPLETE_NOT;
						break;
					}
					m_nMissionButtonStateEnd = MISS_COPLETE_S;
					m_bButtonStateEnd = TRUE;
					return INF_MSGPROC_BREAK;
				}
				else if(pt.y>m_PointMissionPos.y+290 && pt.y<m_PointMissionPos.y+307 &&
					pt.x>m_PointMissionPos.x+430 && pt.x<m_PointMissionPos.x+550 &&
					m_pQuestInfoProg)
				{	// �̼Ǹ� �̵� ��ư(�̼� â����)
					g_pInterface->SetWindowOrder(WNDMissionInfo);
					m_nMissionButtonMapMove = MISS_MAP_GO_S;
					m_bButtonStateMapMove = TRUE;
					return INF_MSGPROC_BREAK;
				}
				else if (pt.y>m_PointMissionPos.y && pt.y<m_PointMissionPos.y+20 &&
					pt.x>m_PointMissionPos.x+570 && pt.x<m_PointMissionPos.x+590)
				{	// â�ݱ�
					g_pInterface->SetWindowOrder(WNDMissionInfo);
					MissionProcFlag(FALSE);					
					return INF_MSGPROC_BREAK;
				}
				
				// 2005-08-02 by ispark
				// �̼� â�ȿ��� Ŭ���� ��ȿ
				if (pt.y>m_PointMissionPos.y && pt.y<m_PointMissionPos.y+MISSION_WINDOW_SIZE_HEIGHT &&
					pt.x>m_PointMissionPos.x && pt.x<m_PointMissionPos.x+MISSION_WINDOW_SIZE_WIDTH)
				{
					g_pInterface->SetWindowOrder(WNDMissionInfo);
					return INF_MSGPROC_BREAK;
				}
			}
			
			// 2005-12-14 by ispark, �������̽��� ������ �ִٸ� �ؿ� Ŭ���� ��ȿ
			if(!g_pInterface->m_bShowInterface || m_bFlagEnterShop == TRUE)
				break;

			// 2007-03-27 by bhsohn ������ ���� �������̽� ����
			if(g_pShuttleChild->IsObserverMode())
			{
				break;
			}
			// end 2007-03-27 by bhsohn ������ ���� �������̽� ����
			
			// ��� �� �̼Ǹ��̵�
			m_bLaunchShuttleState = FALSE;
			m_bMissionMapMove = FALSE;
			if (pt.y>MISSION_LAUNCH_SHUTTLE_Y && pt.y<MISSION_LAUNCH_SHUTTLE_Y+30 &&
				pt.x>MISSION_LAUNCH_SHUTTLE_X && pt.x<MISSION_LAUNCH_SHUTTLE_X+124 &&
				m_bFlagEnterShop == FALSE && g_pD3dApp->m_bCharacter == TRUE &&
				!g_pGround->m_bBazaar)
			{
				g_pInterface->SetWindowOrder(WNDMissionInfo);
				m_nLaunchShuttleState = MISS_LAUNCH_SHUTTLE_S;
				m_bLaunchShuttleState = TRUE;
				return INF_MSGPROC_BREAK;
			}
			// 2006-03-24 by ispark, ��Ƽ�̼��ϴ� ��Ƽ�常 ��ư ���̱� �߰�
			if(m_pQuestInfoProgOutDoor &&
				pt.y>MISSION_MAP_MOVE_Y && pt.y<MISSION_MAP_MOVE_Y+30 &&
				pt.x>MISSION_MAP_MOVE_X && pt.x<MISSION_MAP_MOVE_X+124 &&
				m_bFlagEnterShop == FALSE && g_pD3dApp->m_bCharacter == TRUE &&
				(!m_pQuestInfoProgOutDoor->IsPartyQuest() || (m_pQuestInfoProgOutDoor->IsPartyQuest() && g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)))
			{
				g_pInterface->SetWindowOrder(WNDMissionInfo);
				m_bMissionMapMove = TRUE;
				m_nMissionMapMove = MISS_MAP_MOVE_S;
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_MBUTTONDOWN:
		{
			// �׽�Ʈ �ڵ�
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			if(m_bMissionWindowStart == TRUE)
			{
				// 2007-07-09 by bhsohn �̼� â ����� ��, �޸� ���� ���� ����ó��
				//CQuest* pQuest = g_pQuestData->FindQuest(m_vecTabMissionNum[m_nSelectMissionIndex]);
				CQuest* pQuest = NULL;
				if(m_nSelectMissionIndex >= 0)
				{
					pQuest = g_pQuestData->FindQuest(m_vecTabMissionNum[m_nSelectMissionIndex]);
				}				

				if (pt.y>m_PointMissionPos.y+290 && pt.y<m_PointMissionPos.y+307 &&
					pt.x>m_PointMissionPos.x+312 && pt.x<m_PointMissionPos.x+376 &&
					m_bButtonStateStart == TRUE &&  m_bMissionComButtonFlag)
				{
					if(m_vecTabMissionNum.size() <= 0 || m_vecTabMissionNum.size()<m_nSelectMissionIndex)
						break;
					if(m_nMissionViewCase == MISSION_VIEW_SOLVE || m_nMissionViewCase == MISSION_VIEW_UNSOLVE)
						break;
					// �̼ǿϷ�
					MissionEnd(m_vecTabMissionNum[m_nSelectMissionIndex]);
					m_nMissionButtonStateStart = MISS_START_O;
				}
				else if (pt.y>m_PointMissionPos.y+290 && pt.y<m_PointMissionPos.y+307 &&
					pt.x>m_PointMissionPos.x+485 && pt.x<m_PointMissionPos.x+549 &&
					m_bButtonStateEnd == TRUE &&  m_bMissionComButtonFlag == FALSE &&
					m_bMissionStartButtonUse == FALSE &&
					pQuest &&(!pQuest->IsPartyQuest() || (pQuest->IsPartyQuest() && g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)))
				{	// �̼ǽ���
					if(m_vecTabMissionNum.size() <= 0 || m_vecTabMissionNum.size()<m_nSelectMissionIndex)
						break;
					// 2006-03-28 by ispark, �� �ذ��ǿ��� ��밡��
//					if(m_nMissionViewCase == MISSION_VIEW_SOLVE || m_nMissionViewCase == MISSION_VIEW_UNSOLVE)
					if(m_nMissionViewCase == MISSION_VIEW_SOLVE)
						break;
					MissionStart(m_vecTabMissionNum[m_nSelectMissionIndex]);
					m_nMissionButtonStateEnd = MISS_COPLETE_O;
				}
				else if(m_pQuestInfoProgOutDoor && m_pQuestInfoProgOutDoor->StartMapIndex>0 &&
					pt.y>m_PointMissionPos.y+290 && pt.y<m_PointMissionPos.y+307 &&
					pt.x>m_PointMissionPos.x+430 && pt.x<m_PointMissionPos.x+550 &&
					m_pQuestInfoProg && g_pD3dApp->m_bCharacter == TRUE &&
					pQuest && (!pQuest->IsPartyQuest() || (pQuest->IsPartyQuest() && g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)))
				{	// �̼Ǹ� �̵� ��ư(�̼� â����)
					// 2005-11-16 by ispark
					// �̼Ǹ� �̵� ���� �˻�
					if(!CheckMissionStart())
					{
						break;
					}
					m_nMissionButtonMapMove = MISS_MAP_GO_O;
					m_bMissionWindowStart = FALSE;
					g_pD3dApp->StartFadeEffect(TRUE,3,D3DCOLOR_ARGB(0,0,0,0));
					g_pCharacterChild->FineObjectTakeOff();
					m_nMissionMapIndex = m_pQuestInfoProg->StartMapIndex;
				}
			}
			
			// ��� �� �̼Ǹ��̵�
			if (pt.y>MISSION_LAUNCH_SHUTTLE_Y && pt.y<MISSION_LAUNCH_SHUTTLE_Y+30 &&
				pt.x>MISSION_LAUNCH_SHUTTLE_X && pt.x<MISSION_LAUNCH_SHUTTLE_X+124 &&
				m_bLaunchShuttleState == TRUE && m_bFlagEnterShop == FALSE &&
				g_pD3dApp->m_bCharacter == TRUE)
			{
				// 2005-11-16 by ispark
				// ��� �̵� ���� �˻�
				if(!CheckMissionStart())
				{
					break;
				}

				MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(3002);	
				MEX_QUEST_INFO* pTemp = g_pQuestData->FindCharacterQuest(pMapInfo->QuestIndexForWarp);
				if(pTemp && pTemp->QuestState == QUEST_STATE_COMPLETED)
				{
					g_pD3dApp->StartFadeEffect(TRUE,3,D3DCOLOR_ARGB(0,0,0,0));
					m_nLaunchShuttleState = MISS_LAUNCH_SHUTTLE_O;
					g_pCharacterChild->FineObjectTakeOff();
					m_nMissionMapIndex = 0;
				}
				else
				{
					CQuest* pQuest = g_pQuestData->FindQuest(pMapInfo->QuestIndexForWarp);
					if(pQuest)
					{
						char buf[512];
						ZERO_MEMORY(buf);
						wsprintf(buf, STRMSG_C_050818_0010, pQuest->QuestName); //"'%s' �̼� �Ϸ� �� ��ݰ���!"
						g_pGameMain->m_pInfWindow->AddMsgBox( buf, _MESSAGE);
					}
					else
					{
						g_pD3dApp->StartFadeEffect(TRUE,3,D3DCOLOR_ARGB(0,0,0,0));
						m_nLaunchShuttleState = MISS_LAUNCH_SHUTTLE_O;
						g_pCharacterChild->FineObjectTakeOff();
						m_nMissionMapIndex = 0;						
					}
				}
			}
			// 2006-03-24 by ispark, ��Ƽ�̼��ϴ� ��Ƽ�常 ��ư ���̱� �߰�
			if(m_pQuestInfoProgOutDoor && m_pQuestInfoProgOutDoor->StartMapIndex>0 &&
				pt.y>MISSION_MAP_MOVE_Y && pt.y<MISSION_MAP_MOVE_Y+30 &&
				pt.x>MISSION_MAP_MOVE_X && pt.x<MISSION_MAP_MOVE_X+124 &&
				m_bFlagEnterShop == FALSE && g_pD3dApp->m_bCharacter == TRUE &&
				(!m_pQuestInfoProgOutDoor->IsPartyQuest() || (m_pQuestInfoProgOutDoor->IsPartyQuest() && g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)))
			{
				// 2005-11-16 by ispark
				// �̼Ǹ� �̵� ���� �˻�
				if(!CheckMissionStart())
				{
					break;
				}
				
				g_pD3dApp->StartFadeEffect(TRUE,3,D3DCOLOR_ARGB(0,0,0,0));
				m_nMissionMapMove = MISS_MAP_MOVE_O;
				g_pCharacterChild->FineObjectTakeOff();
				m_nMissionMapIndex = m_pQuestInfoProgOutDoor->StartMapIndex;
			}
			
			m_bMoveMissionWindows = FALSE;
			m_bButtonStateEnd = FALSE;
			m_bButtonStateStart = FALSE;
			m_bLaunchShuttleState = FALSE;
			m_bMissionMapMove = FALSE;
			m_bButtonStateMapMove = FALSE;
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

void CINFMissionInfo::StartPossibleMission()
{
	FLOG( "CINFMissionInfo::StartPossibleMission()" );

	CQuest* pQuest = NULL;
	CQuest* pQuestErr = NULL;
	m_bMissionStartButtonUse = TRUE;
	m_vecPossibleDesc.clear();
	m_vecMissionName.clear();
	m_vecCompensationItemName.clear();
//	ZERO_MEMORY(m_strMissionName);
	m_pQuestInfoProgOutDoor = NULL;
	m_pQuestInfoProg = NULL;
	m_nMissionButtonStateEnd = MISS_COPLETE_N;
	// ��������� ���� �߰� ����
	ZERO_MEMORY(m_nCompensation);	

	if(m_vecTabMissionNum.size()<=m_nSelectMissionIndex)
		return;
	pQuest = g_pQuestData->FindQuest(m_vecTabMissionNum[m_nSelectMissionIndex]);
	if(pQuest == NULL)
		return;
	
	pQuestErr = g_pQuestData->GetProgressMission();
	if(CheckProgressMission(pQuest) == FALSE)
	{
		if(pQuestErr && pQuestErr->QuestIndex == pQuest->QuestIndex)
		{
		}
		else
		{
			// ���� ���� �κ� (�����Ҽ� ���� �̼�)
			vector<int>::iterator itErr = m_vecErrPart.begin();
			while(itErr != m_vecErrPart.end())
			{		
				switch(*itErr)
				{
				case MISSION_ERR_NONE:
					break;
				case MISSION_ERR_LEVEL:
					{					
						// 2007-01-22 by bhsohn �̼� ���� ���̽� ���� ������
						//m_vecPossibleDesc.push_back(STRMSG_C_050825_0002); //"������ �����մϴ�."
						char buf[512];
						ZERO_MEMORY(buf);						
						wsprintf(buf, STRMSG_C_070122_0201, pQuest->ReqLevel.Min);		//"%d���� ���� ������ �̼� �Դϴ�."
						m_vecPossibleDesc.push_back(buf); 
					}
					break;
				case MISSION_ERR_STAT_AT:
					m_vecPossibleDesc.push_back(STRMSG_C_050825_0003); //"������ ������Ʈ�� �����մϴ�."
					break;
				case MISSION_ERR_STAT_DE:
					m_vecPossibleDesc.push_back(STRMSG_C_050825_0004); //"������ �����Ʈ�� �����մϴ�."
					break;
				case MISSION_ERR_STAT_FU:
					m_vecPossibleDesc.push_back(STRMSG_C_050825_0005); //"������ ������Ʈ�� �����մϴ�."
					break;
				case MISSION_ERR_STAT_SO:
					m_vecPossibleDesc.push_back(STRMSG_C_050825_0006); //"������ ������Ʈ�� �����մϴ�."
					break;
				case MISSION_ERR_STAT_SH:
					m_vecPossibleDesc.push_back(STRMSG_C_050825_0007); //"������ ������Ʈ�� �����մϴ�."
					break;
				case MISSION_ERR_STAT_DO:
					m_vecPossibleDesc.push_back(STRMSG_C_050825_0008); // "������ ȸ����Ʈ�� �����մϴ�."
					break;
				case MISSION_ERR_ITEM:
					{
						char buf[512];
						ZERO_MEMORY(buf);
						strcpy(buf, STRMSG_C_050825_0009); //"������ \\r"
						vector<string>::iterator itItem = m_vecErritem.begin();
						while(itItem != m_vecErritem.end())
						{
							strcat(buf, (char*)itItem->c_str());
							strcat(buf, " ");
							itItem++;						
						}
						strcat(buf, STRMSG_C_050825_0010); //"\\r��(��) �ʿ��մϴ�."
						STRING_CULL(buf, MISSION_MAX_DESC_STRING, &m_vecPossibleDesc, m_pMissionFontRight[1]);
					}
					break;
				case MISSION_ERR_FLUENCE:
					m_vecPossibleDesc.push_back(STRMSG_C_050825_0011); //"������ �����ʽ��ϴ�."
					break;
				case MISSION_ERR_EXPERIENCE:
					m_vecPossibleDesc.push_back(STRMSG_C_050825_0012); //"����ġ�� �����մϴ�"
					break;
				case MISSION_ERR_QUEST:
					{
						char buf[512];
						ZERO_MEMORY(buf);
						strcpy(buf, STRMSG_C_050825_0013); //"�̼� \\r"
						vector<string>::iterator itItem = m_vecErrQuestName.begin();
						while(itItem != m_vecErrQuestName.end())
						{
							strcat(buf, (char*)itItem->c_str());
							strcat(buf, " ");
							itItem++;						
						}
						strcat(buf, STRMSG_C_050825_0014); //"\\r��(��) �Ϸ��ؾ� ���� �� �� �ֽ��ϴ�."
						STRING_CULL(buf, MISSION_MAX_DESC_STRING, &m_vecPossibleDesc, m_pMissionFontRight[1]);
					}
					break;
				case MISSION_ERR_HIDEQUEST:
					{
						m_vecPossibleDesc.push_back(STRMSG_C_050825_0015); //"������ �̼�"
						m_vecPossibleDesc.push_back(STRMSG_C_050825_0016); //"\\rƯ�� ����\\r�� �Ϸ� �ϼ���"
					}
					break;
				}
				itErr++;
			}
			return;
		}
	}

	char buf[64];
	ZERO_MEMORY(buf);
	m_bProgressMission = FALSE;	
	
	// �̸� ����
	// 2006-03-27 by ispark, ���ڿ� ó�� ����	
	char strMissionNameTemp[50] = {0,};
	int nMaxLevel = pQuest->ReqLevel.Max;
	if(pQuest->ReqLevel.Max>100)
		nMaxLevel = 100;
	wsprintf(buf, " [Lv.%d]", pQuest->ReqLevel.Min);
	int nLevelLen = strlen(buf);
//	strncpy( m_strMissionName, pQuest->QuestName, SIZE_MAX_QUEST_NAME-nLevelLen);
//	strcat( m_strMissionName, buf);
	strncpy( strMissionNameTemp, pQuest->QuestName, 50-nLevelLen);
	strcat( strMissionNameTemp, buf);
//	STRING_CULL(strMissionNameTemp, SIZE_MAX_QUEST_NAME, &m_vecMissionName, m_pMissionFontRight[0]);
	STRING_CULL(strMissionNameTemp, MISSION_MAX_DESC_STRING, &m_vecMissionName, m_pMissionFontRight[0]);

	// ��������� �̸�����	
	OverlapSortPayItem(pQuest);
//	m_vecCompensationItemName = pQuest->EndPayItemVector;
	if(pQuest->EndPayPropensity != 0)
	{ // ��
		m_nCompensation[COMPENS_PRO] = pQuest->EndPayPropensity;
	}
	if(pQuest->EndPayLevel != 0)
	{ // ����
		m_nCompensation[COMPENS_LEV] = pQuest->EndPayLevel;
	}
	if(pQuest->EndPayExperience != 0)
	{ // ����ġ
		m_nCompensation[COMPENS_EXP] = pQuest->EndPayExperience;
	}
	if(pQuest->EndPayBonusStat != 0)
	{ // ���ʽ� ����
		m_nCompensation[COMPENS_BUN] = pQuest->EndPayBonusStat;
	}
	
	// �̼� ���
	MEX_QUEST_INFO* pCharacterQuest = g_pQuestData->FindCharacterQuest(pQuest->QuestIndex);
	if(pCharacterQuest)
	{
		if(g_pQuestData->IsQuestProgress(pQuest->QuestIndex))
		{
			m_vecPossibleDesc.push_back(STRMSG_C_050726_0001); //"\\y�����Ȳ\\y"
			m_bProgressMission = TRUE;
			STRING_CULL(pQuest->QuestNPCInfo.MidTalk, 
				MISSION_MAX_DESC_STRING, &m_vecPossibleDesc, m_pMissionFontRight[1]);
			m_vecPossibleDesc.push_back("          ");
			m_pQuestInfoProg = g_pQuestData->GetProgressMission();
		}
		else if(g_pQuestData->IsQuestCompleted(pQuest->QuestIndex))
		{
			m_vecPossibleDesc.push_back(STRMSG_C_050726_0001); //"\\y�����Ȳ\\y"
			STRING_CULL(pQuest->QuestNPCInfo.SuccessTalk, 
				MISSION_MAX_DESC_STRING, &m_vecPossibleDesc, m_pMissionFontRight[1]);			
			m_vecPossibleDesc.push_back("          ");
		}
//		���� ���� ��Ȳ�� ����
//		else if()
//		{
//			pQuest->TargetMeetNPCInfo.FailTalk;
//		}
		else
		{
			m_vecPossibleDesc.push_back(STRMSG_C_050726_0001); //"\\y�����Ȳ\\y"
			STRING_CULL(pQuest->QuestNPCInfo.PreTalk, 
				MISSION_MAX_DESC_STRING, &m_vecPossibleDesc, m_pMissionFontRight[1]);
			m_vecPossibleDesc.push_back("          ");
//			m_bMissionStartButtonUse = FALSE;
		}
	}
	else
	{		
		if(strlen(pQuest->QuestNPCInfo.PreTalk) > 1)
		{
			m_vecPossibleDesc.push_back(STRMSG_C_050726_0001); //"\\y�����Ȳ\\y"
		STRING_CULL(pQuest->QuestNPCInfo.PreTalk, 
				MISSION_MAX_DESC_STRING, &m_vecPossibleDesc, m_pMissionFontRight[1]);
			m_vecPossibleDesc.push_back("          ");
		}
		m_bMissionStartButtonUse = FALSE;
	}
	
	// 
	if(pCharacterQuest && pCharacterQuest->QuestState == QUEST_STATE_COMPLETED)
	{
		// �̼� ����
		m_vecPossibleDesc.push_back(STRMSG_C_051209_0001); //"\\y��漳��\\y"
		STRING_CULL(pQuest->QuestDescription, 
			MISSION_MAX_DESC_STRING, &m_vecPossibleDesc, m_pMissionFontRight[1]);

		// ��ư���� ���� ���� �߰�
		m_vecPossibleDesc.push_back("          ");
	}

	// �̼� NUMBER (�ó����� �̼� �̼����� ����)
	// 2006-07-20 by ispark, �ó����� �̼� ���� ���� �Ѱ� �߰�
	if((pQuest->QuestKind == QUEST_KIND_SCENARIO) && (pQuest->IsDiscardable == 0))
		m_nMissionGiveUp = 0;
	else
		m_nMissionGiveUp = pQuest->QuestIndex;
	
	// �������� �̼��� �ִ°�� �ٸ��̼� ����Ұ�
	if(g_pQuestData->GetProgressMission())
		m_bMissionStartButtonUse = TRUE;

	// ��ũ�� ����
	m_pScrollRightWindow->Reset();
	m_pScrollRightWindow->SetNumberOfData((int)m_vecPossibleDesc.size());
}

void CINFMissionInfo::InitMission()
{
	FLOG( "CINFMissionInfo::initMission()" );	

	m_vecMissionList.clear();
	m_vecTabMissionNum.clear();

	m_vecCompensationItemName.clear();
	m_vecPossibleDesc.clear();
	m_vecMissionName.clear();
//	ZERO_MEMORY(m_strMissionName);
	ZERO_MEMORY(m_nCompensation);
	ZERO_MEMORY(m_bCompenToolTib);
}
void CINFMissionInfo::InitMissionTitle()
{
	FLOG( "CINFMissionInfo::InitMissionTitle()" );	
	
	//////////////////////////////////////////////////////////////////////////
	// ��Ÿ �ʱ�ȭ
	CQuest* pQuest = NULL;
	m_vecMissionList.clear();
	m_vecTabMissionNum.clear();
	ZERO_MEMORY(m_nCompensation);
	ZERO_MEMORY(m_bCompenToolTib);

	//////////////////////////////////////////////////////////////////////////
	// ���ݱ��� ����, �Ϸ��� �̼ǵ�� 
	// ���� ���� �������� �̼ǵ��� �Է� �ϴ� �κ�
	SetCompleteMissionInput();
	
	//////////////////////////////////////////////////////////////////////////
	// �Ǻ� �з�
	// �ذ�, ���ذ�, ���� ���� �з�
	SetMissionClassiFy();		

	//////////////////////////////////////////////////////////////////////////	
	// �̼� ���� �� �����̼� ���̱�
	pQuest = SetMissionSorting();

	//////////////////////////////////////////////////////////////////////////
	// �̼� ���� ����( ���డ�ɹ̼� �����̼�)
	SetMissionInfo();
	
	// ���� �̼� ȿ��
	m_nSelectMissionIndex = -1;
	m_pScrollRightWindow->Reset();
	m_pScrollLeftWindow->Reset();	

	// ��ũ�� ����
	m_pScrollLeftWindow->SetScrollLinePos( m_PointMissionPos.x+295, m_PointMissionPos.y+50);
	m_pScrollLeftWindow->SetWheelRect(
		m_PointMissionPos.x, 
		m_PointMissionPos.y,
		m_PointMissionPos.x+297,
		m_PointMissionPos.y+398);
	
	m_pScrollRightWindow->SetScrollLinePos( m_PointMissionPos.x+565, m_PointMissionPos.y+50);
	m_pScrollRightWindow->SetWheelRect(
		m_PointMissionPos.x+298,
		m_PointMissionPos.y,
		m_PointMissionPos.x+592,
		m_PointMissionPos.y+398);
	m_pScrollLeftWindow->SetNumberOfData((int)m_vecMissionList.size());
}

BOOL CINFMissionInfo::MissionWindowRunState()
{
	FLOG( "CINFMissionInfo::MissionWindowRunState()" );
	
	return TRUE;
}

DWORD CINFMissionInfo::MissionProcFlag(BOOL bFlag)
{
	FLOG( "CINFMissionInfo::MissionProcFlag(BOOL bFlag)" );
	
	if(m_bInfluenceFlag == TRUE)
		return MISSION_RUN_RENDER_ERR;	// ���¼��� �̼��� �ٸ� �̼� ����ȵ�
	// 2006-07-21 by ispark, ����ʿ����� �̼�â ���� ���Ѵ�.
	if(g_pGround->m_bBazaar)
	{
		return MISSION_RUN_RENDER_ERR;
	}

	m_PointMissionPos.x = ((g_pD3dApp->GetBackBufferDesc().Width/2)-(MISSION_WINDOW_SIZE_WIDTH/2));
	m_PointMissionPos.y = ((g_pD3dApp->GetBackBufferDesc().Height/2)-(MISSION_WINDOW_SIZE_HEIGHT/2));

	InitMission();
	
	// �̼�â �ݱ� ��ư 
	if(FALSE == bFlag) 
	{		
		m_bMissionWindowStart = FALSE;
		if(g_pTutorial)
		{
			g_pTutorial->SetTutorialEd(FALSE);
		}
		
		return MISSION_RUN_SUCCESS;
	}

	// �̼�â ���� ��ư
	if(TRUE == bFlag){
		if(FALSE == MissionWindowRunState())
			return MISSION_RUN_RENDER_ERR;
		else
		{
			InitMissionTitle();
			m_bMissionWindowStart = TRUE;
		}		
	}
	return MISSION_RUN_SUCCESS;
}
void CINFMissionInfo::RenderPossibleMission()
{
	FLOG( "CINFMissionInfo::RenderPossibleMission()" );

	// �̼� ����	
	int nMissionNameLineCount = m_vecMissionName.size();
	for(int i = 0; i < nMissionNameLineCount; i++)
	{
		m_pMissionFontRight[0]->DrawText(
			m_PointMissionPos.x+MISSION_RIGHT_TEXT_GAB_X,       
			m_PointMissionPos.y+MISSION_RIGHT_TEXT_GAB_Y+(MISSION_FONT_HEIGHT_GAB*(i+1)),
			GUI_FONT_COLOR_YM, (char*)m_vecMissionName[i].c_str());
	}
	
	// �̼� ��ũ����
	int nLineCount = 0;
	vector<string>::iterator it = m_vecPossibleDesc.begin();
	while(it != m_vecPossibleDesc.end())
	{
		if(nLineCount+nMissionNameLineCount>RIGHTFONT_MAX_LINE_COUNT)
			break;
		if(nLineCount+m_pScrollRightWindow->GetCurrentScrollIndex()>m_vecPossibleDesc.size())
			break;
		if(nLineCount+nMissionNameLineCount>=RIGHTFONT_MAX_LINE_COUNT)
		{
			m_pMissionFontRight[RIGHTFONT_MAX_LINE_COUNT-1]->DrawText(m_PointMissionPos.x+MISSION_RIGHT_TEXT_GAB_X,
			m_PointMissionPos.y+MISSION_RIGHT_TEXT_GAB_Y+33+((nLineCount+nMissionNameLineCount)*MISSION_FONT_HEIGHT_GAB),
			GUI_FONT_COLOR_W, " ");
		}
		else
		{
			m_pMissionFontRight[nLineCount+1]->DrawText(m_PointMissionPos.x+MISSION_RIGHT_TEXT_GAB_X,
				m_PointMissionPos.y+MISSION_RIGHT_TEXT_GAB_Y+33+((nLineCount+nMissionNameLineCount)*MISSION_FONT_HEIGHT_GAB),
				GUI_FONT_COLOR_W, (char*)m_vecPossibleDesc[nLineCount+m_pScrollRightWindow->GetCurrentScrollIndex()].c_str());
		}
		
		nLineCount++;
		it++;
	}
	// ��������� ������
	RenderCompensationItem();

	// �̼� ����, ���� ��ư
	if(m_bMissionComButtonFlag == TRUE )
	{
		if(m_nMissionViewCase == MISSION_VIEW_SOLVE || m_nMissionViewCase == MISSION_VIEW_UNSOLVE)
		{
			m_nMissionButtonStateStart = MISS_START_N;
			m_nMissionButtonStateEnd = MISS_COPLETE_N;
		}
		
		m_pMissionImg[m_nMissionButtonStateStart]->Move( m_PointMissionPos.x+312, m_PointMissionPos.y+290);
		m_pMissionImg[m_nMissionButtonStateStart]->Render();
	}
	
	// 2007-07-09 by bhsohn �̼� â ����� ��, �޸� ���� ���� ����ó��
	CQuest*	pQuest = NULL;	
	if(m_nSelectMissionIndex >= 0)
	{
		pQuest = g_pQuestData->FindQuest(m_vecTabMissionNum[m_nSelectMissionIndex]);		
	}
	// �̼� ���� ��ư Ư�� ��Ȳ�� Disable������
	// 2006-03-24 by ispark, ��Ƽ�̼��ϴ� ��Ƽ�常 ��ư ���̱� �߰�
	//CQuest*	pQuest = g_pQuestData->FindQuest(m_vecTabMissionNum[m_nSelectMissionIndex]);
	if(m_bMissionStartButtonUse == TRUE 
		|| m_nSelectMissionIndex == -1 
		|| (pQuest && pQuest->IsPartyQuest() && g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType != _PARTYMASTER))

	{
		m_nMissionButtonStateEnd = MISS_COPLETE_NOT;

//		Disable��ư ������ ����
//		if(!m_pQuestInfoProg)
//		{
//			m_pMissionImg[m_nMissionButtonStateEnd]->Move( m_PointMissionPos.x+485,	m_PointMissionPos.y+290);
//			m_pMissionImg[m_nMissionButtonStateEnd]->Render();
//		}
	}
	else
	{
		m_pMissionImg[m_nMissionButtonStateEnd]->Move( m_PointMissionPos.x+485,	m_PointMissionPos.y+290);
		m_pMissionImg[m_nMissionButtonStateEnd]->Render();
	}	
}

void CINFMissionInfo::RenderCompensationItem()
{
	FLOG( "CINFMissionInfo::RenderCompensationItem()" );

	int nCount = 0;
	int nItemGab = m_PointMissionPos.x+310;
	int nItemGabY = m_PointMissionPos.y+330;
	char buf[64];
	ZERO_MEMORY(buf);
	CINFImageEx* pSelectIcon;
	USHORT nUnitKind;
	vector<QUEST_PAY_ITEM>::iterator it = m_vecCompensationItemName.begin();
	while(it != m_vecCompensationItemName.end())
	{
		if(nCount>=MISSION_COMPENSATIONITEM_MAX)
			break;		
		if(it->ItemNum)
		{			
			ITEM * p2 = g_pDatabase->GetServerItemInfo(it->ItemNum);
			if(p2 == NULL)
				break;
			nUnitKind = it->ToUnitKind;						// 2006-11-14 by ispark
			if(nUnitKind == UNITKIND_ALL_MASK || 0 == nUnitKind || CompareBitFlag( nUnitKind, g_pShuttleChild->m_myShuttleInfo.UnitKind ))
			{				
				/*--------------------------------------------------------------------------*/
//				wsprintf(buf, "%08d", p2->ItemNum);			// 2005-08-23 by ispark
				// 2005-11-29 by ispark, SourceIndex�� ����, ��ų�� SKILL_BASE_NUM���� �Ѵ�.
				if(IS_SKILL_ITEM(p2->Kind))
				{
					// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
//					wsprintf(buf, "%08d", SKILL_BASE_NUM(p2->SourceIndex));
					wsprintf(buf, "%08d", SKILL_BASE_SOURCEINDEX(p2->SourceIndex));
				}
				else
				{
					wsprintf(buf, "%08d", p2->SourceIndex);
				}
				pSelectIcon = g_pGameMain->m_pIcon->FindIcon(buf);
				pSelectIcon->Move(nItemGab, nItemGabY);
				pSelectIcon->Render();

				if(m_bCompenToolTib[nCount] == TRUE && p2->ItemNum == 7000022)
				{	// �����ϰ�츸 ǥ��
					ZERO_MEMORY(buf);				
					wsprintf(buf, "SPI:%d", (int)it->Count);
					g_pGameMain->RenderPopUpWindowImage(nItemGab, nItemGabY+32,strlen(buf)*6.5, 1 );
					m_pCompensation->DrawText(nItemGab, nItemGabY+MISSION_TOOLTIB_Y, GUI_FONT_COLOR_YM, buf);
				}
				else if(m_bCompenToolTib[nCount] == TRUE)
				{	// �׿��� ��� ������ ���� ǥ��
					if(p2) 
					{
						g_pGameMain->SetItemInfo(0, p2->ItemNum, nItemGab, nItemGabY+32, it->Count); 
					}
					else 
					{
						g_pGameMain->SetItemInfo( 0, 0, 0, 0); 
					}
				}

//				nItemGab = nItemGab + strlen(buf)*7.5;
				ZERO_MEMORY(buf);
				nItemGab += 52;			// ������ ����
				nCount++;
			}
		}		
		it++;
	}

	for(int i=0; i<COMPENS_MAX; i++)
	{
		if(m_nCompensation[i] == 0)
			continue;
		
		char bufName[COMPENS_MAX][256];
		ZERO_MEMORY(bufName);

		switch(i)
		{
		case COMPENS_EXP:
			wsprintf(buf, "%08d", ICON_EXP_ITEMNUM);
			strcpy(bufName[COMPENS_EXP], STRMSG_C_050819_0001);	//"����ġ:"
			break;
		case COMPENS_LEV:
			wsprintf(buf, "%08d", ICON_LEV_ITEMNUM);
			strcpy(bufName[COMPENS_LEV], STRMSG_C_050819_0002);	//"����:"
			break;
		case COMPENS_PRO:
			wsprintf(buf, "%08d", ICON_PRO_ITEMNUM);
			strcpy(bufName[COMPENS_PRO], STRMSG_C_050819_0003);	//"��ġ:"
			break;
		case COMPENS_BUN:
			wsprintf(buf, "%08d", ICON_BUN_ITEMNUM);
			strcpy(bufName[COMPENS_BUN], STRMSG_C_050819_0004); //"���ʽ�����:"
			break;
		}
		pSelectIcon = g_pGameMain->m_pIcon->FindIcon(buf);
		pSelectIcon->Move(nItemGab, nItemGabY);
		pSelectIcon->Render();		
		
		if(m_bCompenToolTib[nCount] == TRUE)
		{
			ZERO_MEMORY(buf);
			wsprintf(buf, "%s%d", bufName[i], m_nCompensation[i]);
			g_pGameMain->RenderPopUpWindowImage(nItemGab, nItemGabY+32,strlen(buf)*6.5, 1 );
			m_pCompensation->DrawText(nItemGab, nItemGabY+MISSION_TOOLTIB_Y, GUI_FONT_COLOR_YM, buf);
		}

		nItemGab += 52;
		nCount++;
	}
}

void CINFMissionInfo::MissionStart(DWORD dMissionIndex)
{
	FLOG( "CINFMissionInfo::MissionStart()" );
	
	if(dMissionIndex == NULL)
		return;

	// 2005-11-15 by ispark
	// �̼� ���� ���� �˻�
	if(!CheckMissionStart())
	{
		return;
	}

	CQuest* pQuest = g_pQuestData->FindQuest(dMissionIndex);
	if(pQuest && pQuest->QuestEndType == QUEST_END_TYPE_IMMEDIATE)
	{
		// ���۽� �ٷ� ������ �̼� �̼�â ���� ����.
	}	
	else
	{
		m_bMissionWindowStart = FALSE;
	}

	// Ư�� ������ �Ϸ� �̼� �˻�
	CQuest* pQuest2 = g_pQuestData->FindPosCompleteQuest( dMissionIndex );
	if(pQuest2)
	{
		SetPosMissionComplete(TRUE);
	}

	// ���� ���� �̼�
	if(pQuest->QuestEndType == QUEST_END_TYPE_SELECT_INFLUENCE)
	{
		STAGE_EFFECT_DATA stStageData;
		stStageData.nMissionIndex = dMissionIndex;
		// 2008-04-02 by bhsohn ���¼��� ���ѷα� ��ŵ�����ϰ� ����
		//g_pInterface->InitSoundScript("Influence.tex", &stStageData, MISSION_PRE, FALSE);
		g_pInterface->InitSoundScript("Influence.tex", &stStageData, MISSION_PRE, TRUE);
		
		return;
	}
	
	g_pQuestData->SendFieldSocketQuestRequestStart(dMissionIndex,0);	

	// 2008-06-17 by bhsohn ��� ���� ó��
	// ������ ���� ��������
	if(g_pShuttleChild->m_pClientParty 
		&& g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER 
		&& g_pShuttleChild->m_pClientParty->IsFormationFlight())
	{
		g_pShuttleChild->m_pClientParty->TempPartyFormation(FLIGHT_FORM_NONE);
	}
	// end 2008-06-17 by bhsohn ��� ���� ó��
}

void CINFMissionInfo::MissionEnd(DWORD dMissionIndex)
{
	FLOG( "CINFMissionInfo::MissionEnd()" );
	
	if(dMissionIndex == NULL)
		return;

	g_pQuestData->SendFieldSocketQuestRequestSuccess(dMissionIndex);
}

void CINFMissionInfo::RenderMissionTarget()
{
	FLOG( "CINFMissionInfo::RenderMissionTarget()" );
	
	CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
	m_pQuestInfoProgOutDoor = pQuestInfo;	
	
	if(NULL != pQuestInfo)
	{
		int nLineCount = 0;		// ���� ����
		int nbreak = 0;		// ���� �극��ũ
		char buf[128];
		int nCount = 0;
		vector<int> nVecItemNum;
		int nItemNum = 0;
		nVecItemNum.clear();
		
		// �̼� �̸� ǥ��
		m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, pQuestInfo->QuestName);
		nLineCount++;
		
		// ���� �ʿ� ������
		vector<ITEM_W_COUNT>::iterator it = pQuestInfo->TargetItemVector.begin();
		while(it != pQuestInfo->TargetItemVector.end())
		{
			if(nbreak>3)
				break;
			
			if(it->Count < 0)
			{// 2007-03-15 by dgwoo ��ġ������ ���̴� ����Ÿ
				it++;
				continue;
			}
			//////////////////////////////////////////////////////////////////////////	
			// 2005-08-02 ispark		
			ITEM* pItem = g_pDatabase->GetServerItemInfo(it->ItemNum);
			if(pItem)
			{				
				util::zero(buf);
				
				wsprintf(buf, STRMSG_C_090112_0203, pItem->ItemName, (*it).Count); //"��ǥ:%s %d��"

				m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
				nCount = it->Count;
				nVecItemNum.push_back(it->ItemNum);
				nLineCount++;
				nbreak++;
			}			
#ifdef _DEBUG
			else if(NULL == pItem)
			{
				ZERO_MEMORY(buf);
				wsprintf(buf, "ItemNum : %d Check",it->ItemNum);
				m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
				nLineCount++;
				nbreak++;
			}
#endif
			//////////////////////////////////////////////////////////////////////////
			it++;
		}
		
		// ���� ��ǥ ����
		vector<MONSTER_W_COUNT>::iterator itMonC = pQuestInfo->TargetMonsterVector.begin();
		while(itMonC != pQuestInfo->TargetMonsterVector.end())
		{
			if(0 < itMonC->Count )
			{
				MEX_MONSTER_INFO * pTempIn = g_pDatabase->CheckMonsterInfo((*itMonC).MonsterUniqueNumber);
				
				util::zero(buf);

				wsprintf( buf, STRMSG_C_090112_0203,pTempIn->MonsterName, (*itMonC).Count ); //"\\e%s\\e \\w%d��/%d��\\w"

				m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
				nLineCount++;
			}
			itMonC++;
		}
		
		nbreak = 0;
		vector<ITEM_W_COUNT_MAP_AREA>::iterator it2 = pQuestInfo->TargetMonsterItemVector.begin();
		while(it2 != pQuestInfo->TargetMonsterItemVector.end())
		{
			if(nbreak>3)
				break;

			if(it2->ItemWithCount.Count < 0)
			{// 2007-03-15 by dgwoo ��ġ������ ������ ����Ÿ 
				it2++;
				continue;
			}
			//////////////////////////////////////////////////////////////////////////			
			ITEM* pItem = g_pDatabase->GetServerItemInfo(it2->ItemWithCount.ItemNum);
			if(pItem)
			{
				m_vecOldString.clear();
				if(pQuestInfo->vecQuestDropItem.size()<0)
				{
					util::zero(buf);

					wsprintf(buf, STRMSG_C_090112_0203, pItem->ItemName, it2->ItemWithCount.Count); //"��ǥ:%s %d��"

					m_vecOldString.push_back(buf);
					
					m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 
						20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W,
						buf);
					
					nCount = it2->ItemWithCount.Count;
					nVecItemNum.push_back(it2->ItemWithCount.ItemNum);
					nbreak++;
					nLineCount++;
				}
				
				vector<QUEST_DROP_ITEM>::iterator itItem = pQuestInfo->vecQuestDropItem.begin();
				while(itItem != pQuestInfo->vecQuestDropItem.end())
				{
					QUEST_DROP_ITEM *pTemp = &*itItem;
					if(pItem->ItemNum == pTemp->ItemNumber)
					{
						MEX_MONSTER_INFO * pMonster = g_pDatabase->CheckMonsterInfo(pTemp->MonsterUniqueNumber);							
						if(pMonster)
						{
							ZERO_MEMORY(buf);
							// 2009-01-12 by bhsohn �Ϻ� �߰� ��������
// 							wsprintf(buf, STRMSG_C_050818_0011, pItem->ItemName,pMonster->MonsterName,
// 								it2->ItemWithCount.Count); //"��ǥ:%s %d��"
							wsprintf(buf, STRMSG_C_090112_0205, pItem->ItemName,pMonster->MonsterName,
								it2->ItemWithCount.Count); //"��ǥ:%s %d��"
							// end 2009-01-12 by bhsohn �Ϻ� �߰� ��������

							m_vecOldString.push_back(buf);
							
							m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 
								20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W,
								buf);
							
							nCount = it2->ItemWithCount.Count;
							nVecItemNum.push_back(it2->ItemWithCount.ItemNum);
							nbreak++;
							nLineCount++;						
						}
					}
					
					itItem++;
				}				
			}
#ifdef _DEBUG
			else if(NULL == pItem)
			{
				ZERO_MEMORY(buf);
				wsprintf(buf, "ItemNum : %d Check",it2->ItemWithCount.ItemNum);
				m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
				nbreak++;
				nLineCount++;
			}
#endif
			//////////////////////////////////////////////////////////////////////////
			it2++;
		}
		// ���� NPC �̸�
		if(strlen(pQuestInfo->TargetMeetNPCInfo.NPCName)>0)
		{
			ZERO_MEMORY(buf);			
			wsprintf(buf, STRMSG_C_050726_0003, pQuestInfo->TargetMeetNPCInfo.NPCName); // "[%s]�� ã�ư���!"
			m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
			nLineCount++;
		}
		
		// ���� �������ϴ� ������Ʈ
		// ���� ��� ���� ��������Ʈ ���
		if(pQuestInfo->TargetMeetObjectMapArea.MapIndex)
		{
			MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(pQuestInfo->TargetMeetObjectMapArea.MapIndex);
			if(pMapInfo)
			{
				ZERO_MEMORY(buf);			
				wsprintf(buf, STRMSG_C_050726_0004, pMapInfo->MapName); //"[%s]�� ����϶�!"
				m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
				nLineCount++;
			}
		}
		
		// ���� ��ǥ ������
		if(pQuestInfo->QuestEndType == QUEST_END_TYPE_LEVEL_UP)
		{
			ZERO_MEMORY(buf);			
			wsprintf(buf, STRMSG_C_050805_0001); // "������ �Ѵܰ� �ø�����"
			m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
			nLineCount++;
		}
		
		// ����Ʈ �����ð� ǥ��
		if(g_pGameMain->m_bQuestLimitTimeView)
		{
			ZERO_MEMORY(buf);			
			// 2008-04-07 by bhsohn ����Ʈ �ð� �����Ǵ� ���� ó��
			//if(g_pGameMain->m_nTimeSecond >= 0)					
//			{
//				wsprintf(buf, STRMSG_C_INTERFACE_0020, g_pGameMain->m_nTimeSecond/60, g_pGameMain->m_nTimeSecond%60);//"[�����ð�] %2.d�� %2.d��"
//				m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
//				nLineCount++;
//			}
			if(g_pGameMain->GetQuestTimeElapse() >= 0)
			{
				int nTimeSecond = (int)g_pGameMain->GetQuestTimeElapse();
				wsprintf(buf, STRMSG_C_INTERFACE_0020, nTimeSecond/60, nTimeSecond%60);//"[�����ð�] %2.d�� %2.d��"
				m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
				nLineCount++;
			}
			// end 2008-04-07 by bhsohn ����Ʈ �ð� �����Ǵ� ���� ó��
		}
		// 2007-04-10 by dgwoo �ͽ� �������� ������.
		if(pQuestInfo->TargetMixItemTarget.Count > 0)
		{
			ZERO_MEMORY(buf);	
			ITEM* pItem = g_pDatabase->GetServerItemInfo(pQuestInfo->TargetMixItemTarget.ItemNum);
			if(pItem != NULL)
			{
				wsprintf(buf, STRMSG_C_070410_0100,pItem->ItemName);	//"[���丮]���� %s(��)�� �����϶�"
				m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
				nLineCount++;
			}
		}
		
		// ����Ʈ ������ � ����
		vector<int>::iterator vecInt = nVecItemNum.begin();
		while(vecInt != nVecItemNum.end())
		{
			ITEM_GENERAL *pItem = g_pStoreData->FindItemInInventoryByItemNum(*vecInt);
			if( pItem )
			{
				ZERO_MEMORY(buf);
				wsprintf( buf, STRMSG_C_050726_0005, pItem->ItemInfo->ItemName, pItem->CurrentCount );//"\\e%s\\e \\w%d��/%d\\w"
				m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
				nLineCount++;
			}
			vecInt++;
		}
		
		// ����Ʈ ���͸� ����� ��
		vector<MONSTER_W_COUNT>::iterator itMon = pQuestInfo->TargetMonsterVector.begin();
		while(itMon != pQuestInfo->TargetMonsterVector.end())
		{
			vector<Quest_MonsterInfo>::iterator it = g_pQuestData->m_vecQuestMonsterCount.begin();
			while(it != g_pQuestData->m_vecQuestMonsterCount.end())
			{
				MEX_MONSTER_INFO * pTempInfo = g_pDatabase->CheckMonsterInfo((*it).nMonsterUnitkind);
				if(pTempInfo)
				{
					if((*itMon).MonsterUniqueNumber == (*it).nMonsterUnitkind)
					{
						if(0 < itMon->Count)
						{
						ZERO_MEMORY(buf);
						wsprintf( buf, STRMSG_C_051026_0001,pTempInfo->MonsterName, (*it).nMonsterCount, (*itMon).Count ); //"\\e%s\\e \\w%d��/%d��\\w"
						m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
						nLineCount++;
						}
					}					
				}
				it++;
			}
			itMon++;
		}		
	}
	
	if(pQuestInfo)
	{
		m_nOldMissionIndex = pQuestInfo->QuestIndex;
	}
}

int CINFMissionInfo::CheckProgressMission(CQuest *pQuestInfo)
{
	BOOL err = TRUE;
	// ���� �ʱ�ȭ
	m_vecErrPart.clear();
	m_vecErrQuestName.clear();
	m_vecErritem.clear();

	if (FALSE == COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pQuestInfo->ReqUnitKind))
	{// ���� ���� üũ
		m_vecErrPart.push_back(MISSION_ERR_FLUENCE);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqAttackPart.Min, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart, pQuestInfo->ReqAttackPart.Max))
	{
		m_vecErrPart.push_back(MISSION_ERR_STAT_AT);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqDefensePart.Min, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DefensePart, pQuestInfo->ReqDefensePart.Max))
	{
		m_vecErrPart.push_back(MISSION_ERR_STAT_DE);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqFuelPart.Min, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.FuelPart, pQuestInfo->ReqFuelPart.Max))
	{
		m_vecErrPart.push_back(MISSION_ERR_STAT_FU);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqSoulPart.Min, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.SoulPart, pQuestInfo->ReqSoulPart.Max))
	{
		m_vecErrPart.push_back(MISSION_ERR_STAT_SO);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqShieldPart.Min, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.ShieldPart, pQuestInfo->ReqShieldPart.Max))
	{
		m_vecErrPart.push_back(MISSION_ERR_STAT_SH);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqDodgePart.Min, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DodgePart, pQuestInfo->ReqDodgePart.Max))
	{
		m_vecErrPart.push_back(MISSION_ERR_STAT_DO);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqLevel.Min, g_pShuttleChild->m_myShuttleInfo.Level, pQuestInfo->ReqLevel.Max))	// �ʿ䷹��
	{
		m_vecErrPart.push_back(MISSION_ERR_LEVEL);
		err = FALSE;
	}

	if (pQuestInfo->ReqExperience > g_pShuttleChild->m_myShuttleInfo.Experience)	// �ʿ� ����ġ
	{
		m_vecErrPart.push_back(MISSION_ERR_EXPERIENCE);
		err = FALSE;
	}

	int X = (int)(g_pD3dApp->m_pShuttleChild->m_vPos.x);
	int Z = (int)(g_pD3dApp->m_pShuttleChild->m_vPos.z);
	if (pQuestInfo->ReqStartArea.MapIndex && FALSE == pQuestInfo->ReqStartArea.IsPositionInArea(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex, X,Z))	// ���� ����Ʈ
	{
		m_vecErrPart.push_back(MISSION_ERR_HIDEQUEST);
		err = FALSE;
	}
	
//	// �̼� �Ϸ� ���� üũ
//	if (pQuestInfo->TargetMeetMapArea.MapIndex &&
//		FALSE == pQuestInfo->TargetMeetMapArea.IsPositionInArea(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex, X,Z))	// ���� ����Ʈ
//	{
//		m_vecErrPart.push_back(MISSION_ERR_POSENDQUEST);
//		err = FALSE;
//	}

	int i = 0;
	BOOL bReqItem = FALSE;
	// �ʿ� ������ Ȯ��
	for (i = 0; i < pQuestInfo->ReqItemVector.size(); i++)
	{
		ITEM_W_COUNT tmpItemWCount = pQuestInfo->ReqItemVector[i];

		CItemInfo *pItem = g_pStoreData->FindItemInInventoryByItemNum(tmpItemWCount.ItemNum);
		if (pItem == NULL || (IS_GENERAL_ITEM(pItem->Kind) && pItem->CurrentCount < tmpItemWCount.Count))
		{
			m_vecErritem.push_back(pItem->ItemInfo->ItemName);
			bReqItem = TRUE;			
			err = FALSE;
		}
	}
	if(bReqItem == TRUE)	{
		m_vecErrPart.push_back(MISSION_ERR_ITEM);
	}

//	// �ʿ� ���� ������ Ȯ��
//	bReqItem = FALSE;
//	for (i = 0; i < pQuestInfo->TargetMonsterItemVector.size(); i++)
//	{
//		if (pQuestInfo->TargetMonsterItemVector[i].ItemWithCount.ItemNum != 0)
//		{
//			CItemInfo *pTargetMonsterItem = g_pStoreData->FindItemInInventoryByItemNum(pQuestInfo->TargetMonsterItemVector[i].ItemWithCount.ItemNum);
//			
//			// Count�� 0�̸� �� ���� �ʾƵ� �Ǵ� ��������
//			if (pQuestInfo->TargetMonsterItemVector[i].ItemWithCount.Count == 0)
//			{
//				continue;
//			}
//			
//			// �ش� �������� ������ ���� ���
//			if (pTargetMonsterItem == NULL)
//			{
//				err = FALSE;
//				bReqItem = TRUE;
//				break;
//			}
//			
//			// �ش� �������� ������ ������ ���
//			if (IS_COUNTABLE_ITEM(pTargetMonsterItem->ItemInfo->Kind)
//				&& pTargetMonsterItem->CurrentCount < pQuestInfo->TargetMonsterItemVector[i].ItemWithCount.Count)
//			{
//				err = FALSE;
//				bReqItem = TRUE;
//				break;
//			}
//		}
//	}
//	if(bReqItem == TRUE)	{
//		m_vecErrPart.push_back(MISSION_ERR_MONSTER_ITEM);
//	}

	// �ʿ� quest Ȯ��
	BOOL	bReqQuest = FALSE;
	for (i = 0; i < pQuestInfo->ReqQuestIndexVector.size(); i++)
	{
		MEX_QUEST_INFO* pCharacterQuest = g_pQuestData->FindCharacterQuest(pQuestInfo->ReqQuestIndexVector[i]);
		if(pCharacterQuest == NULL || pCharacterQuest->QuestState != QUEST_STATE_COMPLETED)
		{
			if(pQuestInfo)
			{
				CQuest* pQuest = NULL;
				vector<INT>::iterator itIndex = pQuestInfo->ReqQuestIndexVector.begin();
				while(itIndex != pQuestInfo->ReqQuestIndexVector.end())
				{
					pQuest = g_pQuestData->FindQuest(*itIndex);
					if(pQuest){
						m_vecErrQuestName.push_back( pQuest->QuestName );
					}
					itIndex++;
				}
				
			}
			bReqQuest = TRUE;
			err = FALSE;
		}
	}
	if(bReqQuest == TRUE) {
		m_vecErrPart.push_back(MISSION_ERR_QUEST);
	}

	return err;
}

void CINFMissionInfo::RefreshMission()
{
	m_nSelectMissionIndex = -1;
	m_nMissionViewCase = MISSION_VIEW_ALL;
	m_pScrollRightWindow->Reset();
	m_pScrollLeftWindow->Reset();
	
	InitMission();
	InitMissionTitle();
}

void CINFMissionInfo::SetInfluenceMission(BOOL nFlag, float fANIPercent, float fVANPercent)
{
	// ���¼��� �̼� �����κ� ( TRUE ������ �ٸ��̼� ���� �Ұ�, ����ϱ� �Ұ�)
	m_fANIInflDistributionPercent = fANIPercent;
	m_fVCNInflDistributionPercent = fVANPercent;
	if(nFlag)
	{
		MissionProcFlag(FALSE);
		InitInfluenceMission();	
	}
	else
	{
		m_bInfluenceFlag = FALSE;
	}
}

void CINFMissionInfo::InitInfluenceMission()
{
	m_bInfluenceFlag = TRUE;

	m_nInfluencePosX = (g_pD3dApp->GetBackBufferDesc().Width/2);
	m_nInfluencePosY = (g_pD3dApp->GetBackBufferDesc().Height/2);
	
	POINT pSize = m_pMissionImg[MISS_INFLUENCE_BK]->GetImgSize();

	m_nInfluencePosX = m_nInfluencePosX-(pSize.x/2);
	m_nInfluencePosY = m_nInfluencePosY-(pSize.y/2);

	m_vecVCUfluenceDesc.push_back(STRMSG_C_050818_0007);	// "[����]"
	STRING_CULL(STRMSG_C_050818_0001, MISSION_FLUNCE_MAX_DESC_STRING, &m_vecVCUfluenceDesc, m_pMissionFontfluenceL[0]);	
	m_vecVCUfluenceDesc.push_back(STRMSG_C_050818_0008);	// "[���]"
	STRING_CULL(STRMSG_C_050818_0002, MISSION_FLUNCE_MAX_DESC_STRING, &m_vecVCUfluenceDesc, m_pMissionFontfluenceL[0]);	
	m_vecVCUfluenceDesc.push_back(STRMSG_C_050818_0009);	//"[Aerial Cry]"
	STRING_CULL(STRMSG_C_050818_0003, MISSION_FLUNCE_MAX_DESC_STRING, &m_vecVCUfluenceDesc, m_pMissionFontfluenceL[0]);	

	m_vecANIfluenceDesc.push_back(STRMSG_C_050818_0007);	// "[����]"
	STRING_CULL(STRMSG_C_050818_0004, MISSION_FLUNCE_MAX_DESC_STRING, &m_vecANIfluenceDesc, m_pMissionFontfluenceR[0]);	
	m_vecANIfluenceDesc.push_back(STRMSG_C_050818_0008);	// "[���]"
	STRING_CULL(STRMSG_C_050818_0005, MISSION_FLUNCE_MAX_DESC_STRING, &m_vecANIfluenceDesc, m_pMissionFontfluenceR[0]);		
	m_vecANIfluenceDesc.push_back(STRMSG_C_050818_0009);	//"[Aerial Cry]"
	STRING_CULL(STRMSG_C_050818_0006, MISSION_FLUNCE_MAX_DESC_STRING, &m_vecANIfluenceDesc, m_pMissionFontfluenceR[0]);
	
	m_pScrollLeftfluence->SetNumberOfData(m_vecVCUfluenceDesc.size());
	m_pScrollRightfluence->SetNumberOfData(m_vecANIfluenceDesc.size());
	
}

void CINFMissionInfo::RenderInfluenceMission()
{
	m_pMissionImg[MISS_INFLUENCE_BK]->Move(m_nInfluencePosX, m_nInfluencePosY);
	m_pMissionImg[MISS_INFLUENCE_BK]->Render();
	
	// ��ư ������
	m_pMissionImg[m_nVCUButton]->Move(m_nInfluencePosX+133, m_nInfluencePosY+489);
	m_pMissionImg[m_nVCUButton]->Render();

	m_pMissionImg[m_nANIButton]->Move(m_nInfluencePosX+509, m_nInfluencePosY+489);
	m_pMissionImg[m_nANIButton]->Render();

	for(int i=0; i<MISSION_SCROLL_FLUENCE_MAX_LINE; i++)
	{
		if(m_vecVCUfluenceDesc.size()>i+m_pScrollLeftfluence->GetCurrentScrollIndex())
		{	// ���α� ����
			m_pMissionFontfluenceL[i]->DrawText(m_nInfluencePosX+40,m_nInfluencePosY+130+(i*MISSION_FONT_HEIGHT_GAB),GUI_FONT_COLOR_W,
				(char*)m_vecVCUfluenceDesc[i+m_pScrollLeftfluence->GetCurrentScrollIndex()].c_str());
		}

		if(m_vecANIfluenceDesc.size()>i+m_pScrollRightfluence->GetCurrentScrollIndex())
		{	// �ݶ��� ����
			m_pMissionFontfluenceR[i]->DrawText(m_nInfluencePosX+410,m_nInfluencePosY+130+(i*MISSION_FONT_HEIGHT_GAB),GUI_FONT_COLOR_W, 
				(char*)m_vecANIfluenceDesc[i+m_pScrollRightfluence->GetCurrentScrollIndex()].c_str());
		}
	}

	m_pScrollLeftfluence->Render();
	m_pScrollRightfluence->Render();

	// 2006-02-09 by ispark, ����
	if(m_nInflChoiceOver == INFLUENCE_TYPE_VCN || m_nInflChoiceOver == INFLUENCE_TYPE_ANI)
	{
		int nDistributionPercent = 0;
		m_pImgWarning->Move(MISSION_WARNING_X, MISSION_WARNING_Y);
		m_pImgWarning->Render();

		char chInfluenceName[20];
		if(m_nInflChoiceOver == INFLUENCE_TYPE_VCN)
		{
			sprintf(chInfluenceName, STRMSG_C_060210_0000);
			nDistributionPercent = m_fVCNInflDistributionPercent;
		}
		if(m_nInflChoiceOver == INFLUENCE_TYPE_ANI)
		{
			sprintf(chInfluenceName, STRMSG_C_060210_0001);
			nDistributionPercent = m_fANIInflDistributionPercent;
		}

		char MessageInfluenceWarning[1024];
		sprintf(MessageInfluenceWarning, STRMSG_C_060208_0001, (int)m_fVCNInflDistributionPercent, (int)m_fANIInflDistributionPercent); //"������ ���º��� ��Ȳ�� ������������ \\e%d\\e%%, �˸����� \\e%d\\e%%�Դϴ�."
		m_pInfluenceWarning[0]->DrawText(MISSION_WARNING_INFO_X, MISSION_WARNING_INFO_Y, GUI_FONT_COLOR_W, MessageInfluenceWarning);
		sprintf(MessageInfluenceWarning, STRMSG_C_060209_0000, chInfluenceName, (int)nDistributionPercent - 50); //"\\y%s\\y ������ �����̿��ݿ� \\r%d\\r%%�� �� ��� ��Ȳ�Դϴ�."
		m_pInfluenceWarning[1]->DrawText(MISSION_WARNING_INFO_X, MISSION_WARNING_INFO_Y + 15, GUI_FONT_COLOR_W, MessageInfluenceWarning);
		sprintf(MessageInfluenceWarning, STRMSG_C_060209_0001, chInfluenceName); //""�׷��� \\y%s\\y ������ �����Ͻðڽ��ϱ�?"
		m_pInfluenceWarning[2]->DrawText(MISSION_WARNING_INFO_X, MISSION_WARNING_INFO_Y + 30, GUI_FONT_COLOR_W, MessageInfluenceWarning);

		if(m_nInflChoiceOKButton >= 0)
		{
			m_pImgOkButton[m_nInflChoiceOKButton]->Move(MISSION_WARNING_OK_X, MISSION_WARNING_OK_Y);
			m_pImgOkButton[m_nInflChoiceOKButton]->Render();
		}
		else if(m_nInflChoiceCANButton >= 0)
		{
			m_pImgCancelButton[m_nInflChoiceCANButton]->Move(MISSION_WARNING_CAN_X, MISSION_WARNING_CAN_Y);
			m_pImgCancelButton[m_nInflChoiceCANButton]->Render();
		}
	}

}

int	CINFMissionInfo::WndProcInfluence(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// ��ũ�� ó��
	if(m_pScrollLeftfluence && m_pScrollRightfluence){
		if(m_pScrollLeftfluence->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK){
			return INF_MSGPROC_BREAK;
		}
		else if(m_pScrollRightfluence->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK){
			return INF_MSGPROC_BREAK;
		}
	}

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);			
			
			// ��ư �ʱ�ȭ
			m_nVCUButton = MISS_INFLUENCE_NR;
			m_nANIButton = MISS_INFLUENCE_NR;

			// 2006-02-09 by ispark, ����
			m_nInflChoiceOKButton = -1;
			m_nInflChoiceCANButton = -1;
			if(m_nInflChoiceOver == INFLUENCE_TYPE_VCN || m_nInflChoiceOver == INFLUENCE_TYPE_ANI)
			{
				if(pt.x > MISSION_WARNING_OK_X && pt.x < MISSION_WARNING_OK_X + 38 &&
					pt.y > MISSION_WARNING_OK_Y && pt.y < MISSION_WARNING_OK_Y + 17)
				{
					m_nInflChoiceOKButton = 0;
				}
				else if(pt.x > MISSION_WARNING_CAN_X && pt.x < MISSION_WARNING_CAN_X + 38 &&
					pt.y > MISSION_WARNING_CAN_Y && pt.y < MISSION_WARNING_CAN_Y + 17)
				{
					m_nInflChoiceCANButton = 0;
				}
				break;
			}
			
			if(pt.y>m_nInfluencePosY+489 && pt.y<m_nInfluencePosY+525 &&
				pt.x>m_nInfluencePosX+133 && pt.x<m_nInfluencePosX+253)
			{
				if(m_bVCUButton)
					m_nVCUButton = MISS_INFLUENCE_PS;
				else
					m_nVCUButton = MISS_INFLUENCE_OV;
			}
			else if(pt.y>m_nInfluencePosY+489 && pt.y<m_nInfluencePosY+525 &&
				pt.x>m_nInfluencePosX+509 && pt.x<m_nInfluencePosX+629)
			{
				if(m_bANIButton)
					m_nANIButton = MISS_INFLUENCE_PS;
				else
					m_nANIButton = MISS_INFLUENCE_OV;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			POINT pSize = m_pMissionImg[MISS_INFLUENCE_BK]->GetImgSize();
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			// 2006-02-09 by ispark, ����
			if(m_nInflChoiceOver == INFLUENCE_TYPE_VCN || m_nInflChoiceOver == INFLUENCE_TYPE_ANI)
			{
				if(pt.x > MISSION_WARNING_OK_X && pt.x < MISSION_WARNING_OK_X + 38 &&
					pt.y > MISSION_WARNING_OK_Y && pt.y < MISSION_WARNING_OK_Y + 17)
				{
					m_nInflChoiceOKButton = 1;
				}
				else if(pt.x > MISSION_WARNING_CAN_X && pt.x < MISSION_WARNING_CAN_X + 38 &&
					pt.y > MISSION_WARNING_CAN_Y && pt.y < MISSION_WARNING_CAN_Y + 17)
				{
					m_nInflChoiceCANButton = 1;
				}
				return INF_MSGPROC_BREAK;
			}

			m_bVCUButton = FALSE;
			m_bANIButton = FALSE;
			if(pt.y>m_nInfluencePosY && pt.y<m_nInfluencePosY+pSize.y &&
				pt.x>m_nInfluencePosX && pt.x<m_nInfluencePosX+pSize.x)
			{
				if(pt.y>m_nInfluencePosY+489 && pt.y<m_nInfluencePosY+525 &&
					pt.x>m_nInfluencePosX+133 && pt.x<m_nInfluencePosX+253)
				{
					m_bVCUButton = TRUE;
					m_nVCUButton = MISS_INFLUENCE_PS;					
				}
				else if(pt.y>m_nInfluencePosY+489 && pt.y<m_nInfluencePosY+525 &&
					pt.x>m_nInfluencePosX+509 && pt.x<m_nInfluencePosX+629)
				{
					m_bANIButton = TRUE;
					m_nANIButton = MISS_INFLUENCE_PS;					
				}
				
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			POINT pSize = m_pMissionImg[MISS_INFLUENCE_BK]->GetImgSize();
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			// 2006-02-09 by ispark, ����
			if(m_nInflChoiceOver == INFLUENCE_TYPE_VCN || m_nInflChoiceOver == INFLUENCE_TYPE_ANI)
			{
				m_nInflChoiceOKButton = -1;
				m_nInflChoiceCANButton = -1;
				if(pt.x > MISSION_WARNING_OK_X && pt.x < MISSION_WARNING_OK_X + 38 &&
					pt.y > MISSION_WARNING_OK_Y && pt.y < MISSION_WARNING_OK_Y + 17)
				{
					if(m_nInflChoiceOver == INFLUENCE_TYPE_VCN)
					{
						CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
						g_pQuestData->SendFieldSocketQuestRequestSuccess(pQuestInfo->QuestIndex,INFLUENCE_TYPE_VCN);
					}
					else if(m_nInflChoiceOver == INFLUENCE_TYPE_ANI)
					{
						CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
						g_pQuestData->SendFieldSocketQuestRequestSuccess(pQuestInfo->QuestIndex,INFLUENCE_TYPE_ANI);
					}
				}
				else if(pt.x > MISSION_WARNING_CAN_X && pt.x < MISSION_WARNING_CAN_X + 38 &&
					pt.y > MISSION_WARNING_CAN_Y && pt.y < MISSION_WARNING_CAN_Y + 17)
				{
					m_nInflChoiceOver = INFLUENCE_TYPE_NORMAL;
				}
				
				return INF_MSGPROC_BREAK;
			}
				
			if(pt.y>m_nInfluencePosY && pt.y<m_nInfluencePosY+pSize.y &&
				pt.x>m_nInfluencePosX && pt.x<m_nInfluencePosX+pSize.x)
			{
				if(pt.y>m_nInfluencePosY+489 && pt.y<m_nInfluencePosY+525 &&
					pt.x>m_nInfluencePosX+133 && pt.x<m_nInfluencePosX+253 &&
					m_bVCUButton == TRUE)
				{
					// ���� ����
					// 2006-02-08 by ispark, ���� �ۼ�Ʈ �߰�
					if(m_fVCNInflDistributionPercent > 50.0f)
					{
						if(g_pD3dApp->m_pSound)
						{
							g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OPEN_MESSAGE_BOX, g_pShuttleChild->m_vPos, FALSE);
						}						
						m_nInflChoiceOver = INFLUENCE_TYPE_VCN;
					}
					else
					{
						CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
						g_pQuestData->SendFieldSocketQuestRequestSuccess(pQuestInfo->QuestIndex,INFLUENCE_TYPE_VCN);
					}
					m_nVCUButton = MISS_INFLUENCE_OV;
				}
				else if(pt.y>m_nInfluencePosY+489 && pt.y<m_nInfluencePosY+525 &&
					pt.x>m_nInfluencePosX+509 && pt.x<m_nInfluencePosX+629 &&
					m_bANIButton == TRUE)
				{
					// ���� ����
					// 2006-02-08 by ispark, ���� �ۼ�Ʈ �߰�
					if(m_fANIInflDistributionPercent > 50.0f)
					{
						if(g_pD3dApp->m_pSound)
						{
							g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OPEN_MESSAGE_BOX, g_pShuttleChild->m_vPos, FALSE);
						}
						m_nInflChoiceOver = INFLUENCE_TYPE_ANI;
					}
					else
					{
						CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
						g_pQuestData->SendFieldSocketQuestRequestSuccess(pQuestInfo->QuestIndex,INFLUENCE_TYPE_ANI);
					}
					m_nANIButton = MISS_INFLUENCE_OV;					
				}

				m_bVCUButton = FALSE;
				m_bANIButton = FALSE;
				return INF_MSGPROC_BREAK;
			}

			m_bVCUButton = FALSE;
			m_bANIButton = FALSE;
		}
		break;
//	case WM_MBUTTONDOWN:
//		{
//			// �׽�Ʈ �ڵ�
//			SetInfluenceMission(!m_bInfluenceFlag);
//		}
//		break;
	}
	return INF_MSGPROC_NORMAL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFMissionInfo::CheckMissionStart()
/// \brief		�̼� ������ ���� ���� �˻�
/// \author		ispark
/// \date		2005-11-16 ~ 2005-11-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFMissionInfo::CheckMissionStart()
{
	CMapItemWindowInventoryIterator it;

	// 2006-11-07 by ispark, ���̴��� ���θ� ���
	it = g_pStoreData->m_mapItemWindowPosition.find(POS_PROW);
	if(it == g_pStoreData->m_mapItemWindowPosition.end())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051229_0101,COLOR_SYSTEM); // "���̴��� �����Ǿ� ���� �ʽ��ϴ�."
	}

	//*--------------------------------------------------------------------------*//
	// ������ ���ٸ� �̼� ��� ����
	it = g_pStoreData->m_mapItemWindowPosition.find(POS_REAR);
	if(it == g_pStoreData->m_mapItemWindowPosition.end())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051115_0002,COLOR_SYSTEM); // "���� ���������� ������ �� �����ϴ�."
		return FALSE;
	}

	it = g_pStoreData->m_mapItemWindowPosition.find(POS_CENTER);
	if(it == g_pStoreData->m_mapItemWindowPosition.end())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051206_0001,COLOR_SYSTEM); // "�Ƹ� ���������� ������ �� �����ϴ�."
		return FALSE;
	}

	// 2005-12-07 by ispark, ���� ���� �̼��� ��� ��Ƽ���°� �ƴҶ� Ŭ�� ����
	if(m_nSelectMissionIndex != -1 &&											// �̼� ������ ���ٸ� �Ѿ
		m_vecTabMissionNum[m_nSelectMissionIndex] == 112 && 
		g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType != _NOPARTY)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051207_0001,COLOR_SYSTEM); // "�����¿����� ������ ������ �� �����ϴ�."
		return FALSE;
	}

	// 2006-10-17 by ispark, �׾� ���� ��� ��� ���ϰ�
	if(g_pShuttleChild->CheckUnitState() == BREAKDOWN)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_061017_0001,COLOR_SYSTEM); // "��� �� �� �����ϴ�."
		return FALSE;
	}

	// 2007-07-09 by bhsohn ��ݰ� ���ڻ��� ���� ���� ������ ó��
	if(g_pInterface->IsBazarOpen())
	{		
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_061017_0001,COLOR_SYSTEM); // "��� �� �� �����ϴ�."
		return FALSE;
	}	
	// end 2007-07-09 by bhsohn ��ݰ� ���ڻ��� ���� ���� ������ ó��

	return TRUE;
}


void CINFMissionInfo::SetCompleteMissionInput()
{
	//////////////////////////////////////////////////////////////////////////	
	// ���º� ����Ʈ �з��� �Ϸ� ����Ʈ�� �����´�
	// ������ Ʋ���� �Ϸ�� ����Ʈ�� ȭ�鿡 ǥ���Ѵ�	
	// ���� ������ �̼��� ã�� �κ�
	vector<int> vecQuestInflue;
	vecQuestInflue = g_pQuestData->GetRaceQuestIndex(g_pShuttleChild->m_myShuttleInfo.InfluenceType);
	
	// ��ü �̼��� �������� �κ�
	vector<int> vecQuestTemp;
	vecQuestTemp = g_pQuestData->GetRaceQuestIndex(INFLUENCE_TYPE_ALL_MASK);	
	m_vecQuestIndex.clear();	// ���� â�� �����ִ� �̼� �ε���

	vector<int>::iterator itQuest = vecQuestTemp.begin();
	while(itQuest != vecQuestTemp.end())
	{
		if(g_pQuestData->IsQuestCompleted(*itQuest))
		{
			// ��ȿ�� ���� ��������� ������ ����� �ٲ��� �ʰ� ������ Ʋ������ 
			// �Ϸ� �̼��� �����ֱ� ���� ���
			// �Ŀ� ��ü ���
			BOOL nQFlag = TRUE;
			vector<int>::iterator itQuestSolve = vecQuestInflue.begin();
			while(itQuestSolve != vecQuestInflue.end())
			{
				if(*itQuest == *itQuestSolve)
				{
					nQFlag = FALSE;
				}

				itQuestSolve++;
			}
			if(nQFlag) 
				m_vecQuestIndex.push_back(*itQuest);
		}
		itQuest++;
	}
	// �������� ��ü ���
	vector<int>::iterator itQuestSolve2 = vecQuestInflue.begin();
	while(itQuestSolve2 != vecQuestInflue.end())
	{
		m_vecQuestIndex.push_back(*itQuestSolve2);
		itQuestSolve2++;
	}
	//
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-16 by dgwoo ���� �̼� �����ؾ� �ϴ� �κ�.
	//////////////////////////////////////////////////////////////////////////
}

void CINFMissionInfo::SetMissionClassiFy()
{
	int nQuestKind = 0;
	CQuest* pQuest = NULL;
	char strTemp[512];
	ZERO_MEMORY(strTemp);
	m_bMissionStartButtonUse = FALSE;
	m_pQuestInfoProg = NULL;
	vector<int>::iterator it = m_vecQuestIndex.begin();
	while(it != m_vecQuestIndex.end())
	{
		pQuest = g_pQuestData->FindQuest(*it);

		//////////////////////////////////////////////////////////////////////////
		// �Ǽ��� �κ� (�Ǽ������� �̼� ���� ��Ī�� ���Ѵ�)
		if(m_nSelectMissionTab == MISS_NOM)
		{
			nQuestKind = QUEST_KIND_NORMAL; // �Ϲ� Quest
			if(FALSE == COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pQuest->ReqUnitKind))
			{
				it++;
				continue;
			}
		}
		else if(m_nSelectMissionTab == MISS_SCEN)
		{
			nQuestKind = QUEST_KIND_SCENARIO; // �ó����� Quest
			if(CheckProgressMission(pQuest) == FALSE)
			{
				it++;
				continue;
			}
		}
		else if(m_nSelectMissionTab == MISS_SCEN_D)
		{
//			nQuestKind = QUEST_KIND_COLLECTION; // �÷��� Quest
			nQuestKind = QUEST_KIND_SIDESTORY;  // �÷��� Quest
		}
	

		//////////////////////////////////////////////////////////////////////////		
		// �̼� �Է� �κ�
		if(nQuestKind == pQuest->QuestKind && m_nMissionViewCase == MISSION_VIEW_SOLVE && g_pQuestData->IsQuestCompleted(pQuest->QuestIndex))
		{
		// �ذ� �̼�
			m_vecMissionList.push_back(pQuest->QuestName);
			m_vecTabMissionNum.push_back(pQuest->QuestIndex);
		}
		// 2006-03-28 by ispark, �� �ذ� �̼� ����(�� �� �ִ� �̼Ǳ���)
//		else if(nQuestKind == pQuest->QuestKind && m_nMissionViewCase == MISSION_VIEW_UNSOLVE && g_pQuestData->IsQuestProgress(pQuest->QuestIndex)){
		else if(nQuestKind == pQuest->QuestKind && m_nMissionViewCase == MISSION_VIEW_UNSOLVE && !g_pQuestData->IsQuestCompleted(pQuest->QuestIndex))
		{
		// �� �ذ� �̼�
			m_vecMissionList.push_back(pQuest->QuestName);
			m_vecTabMissionNum.push_back(pQuest->QuestIndex);
		}
		else if(nQuestKind == pQuest->QuestKind && m_nMissionViewCase == MISSION_VIEW_ALL)	{
		// �̼� ���κ���
			ZERO_MEMORY(strTemp);
			
			if(g_pQuestData->IsQuestProgress(pQuest->QuestIndex))
				wsprintf(strTemp, STRMSG_C_050805_0002,pQuest->QuestName); // "%s [���ذ�]"
			else if(g_pQuestData->IsQuestCompleted(pQuest->QuestIndex))
				wsprintf(strTemp, STRMSG_C_050805_0003,pQuest->QuestName);
			else
				wsprintf(strTemp, "%s",pQuest->QuestName);

			if( MISS_SCEN == m_nSelectMissionTab  
				|| MISS_SCEN_D == m_nSelectMissionTab
				|| MISS_NOM == m_nSelectMissionTab)// && !g_pQuestData->IsQuestCompleted(pQuest->QuestIndex))
			{
				m_vecMissionList.push_back(strTemp);
				m_vecTabMissionNum.push_back(pQuest->QuestIndex);
			}
// 2007-02-23 by dgwoo
//			else
//			{
//				m_vecMissionList.push_back(strTemp);
//				m_vecTabMissionNum.push_back(pQuest->QuestIndex);
//			}
		}
		it++;
	}	
}
// 2007-02-23 by dgwoo ���� �ҽ���.
//CQuest* CINFMissionInfo::SetMissionSorting()
//{
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// �ó����� �̼� ���� �ó����� 1�� �������ֱ� ( ��Ÿ �������� )
//	// 2005-08-22 by ydkim
//	// ���� ���� �̼����� -�˻�-
//	// ?�� �κ����� GOTO�� ��� �Ŀ� �������
//	CQuest* pQuest = NULL;
//	m_bProgressMission = FALSE;
//	if(g_pQuestData->GetProgressMission())
//		m_bMissionStartButtonUse = TRUE;
//
//	m_nMaxNum = m_vecQuestIndex[0];							// ���� �ʹ� �̼��ε���.
//QUESTSERCH:
//	pQuest = g_pQuestData->FindQuest(m_nMaxNum);					// ����Ʈ ���� �ε�
//	if(pQuest)
//	{
//		// �˻� ����
//		int nNextIndex = pQuest->NextQuestIndex;
//		vector<int>::iterator itSer = m_vecTabMissionNum.begin();
//		while(itSer != m_vecTabMissionNum.end())
//		{
//			if(nNextIndex == *itSer)
//			{
//				m_nMaxNum = nNextIndex;
//				goto QUESTSERCH;
//			}
//			else if(nNextIndex == 0)
//			{
//				pQuest = g_pQuestData->FindFirstScenarioQuest(g_pShuttleChild->m_myShuttleInfo.InfluenceType);
//				if(pQuest)
//				{
//					m_nMaxNum = pQuest->QuestIndex;
//					goto QUESTSERCH;
//				}
//			}
//			else if(nNextIndex == -1)
//			{
//				return NULL;
//			}
//
//			itSer++;
//		}		
//	}
//	
//	return pQuest;
//	// �˻� ����
//	//////////////////////////////////////////////////////////////////////////
//}

CQuest* CINFMissionInfo::SetMissionSorting()
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// �ó����� �̼� ���� �ó����� 1�� �������ֱ� ( ��Ÿ �������� )
	// 2005-08-22 by ydkim
	// ���� ���� �̼����� -�˻�-
	// ?�� �κ����� GOTO�� ��� �Ŀ� �������
	CQuest* pQuest = NULL;
	int nNextIndex = 0;
	m_bProgressMission = FALSE;
	if(g_pQuestData->GetProgressMission())
		m_bMissionStartButtonUse = TRUE;

	m_nMaxNum = m_vecQuestIndex[0];							// ���� �ʹ� �̼��ε���.


// 2007-02-23 by dgwoo		
	vector<int>::iterator itSer = m_vecQuestIndex.begin();
	while(itSer != m_vecQuestIndex.end())
	{
		pQuest = g_pQuestData->FindQuest(*itSer);
		if(CheckProgressMission(pQuest))
		{
			nNextIndex = pQuest->NextQuestIndex;
			}
		else
		{
			if(nNextIndex == 0)
			{
				pQuest = g_pQuestData->FindFirstScenarioQuest(g_pShuttleChild->m_myShuttleInfo.InfluenceType);
				if(pQuest)
				{
					m_nMaxNum = 0;
				
				}
			}
			else if(nNextIndex == -1)
			{
				return NULL;
			}
			else
			{
				m_nMaxNum = nNextIndex;
			}
			return pQuest;
		}
			itSer++;
		}		
	
	return pQuest;
	// �˻� ����
	//////////////////////////////////////////////////////////////////////////
}
void CINFMissionInfo::SetMissionInfo()
{
	char strTemp[512];
	ZERO_MEMORY(strTemp);
	CQuest* pQuest = NULL;
	CQuest* pQuestFirstInflu = NULL;
	if(COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_NORMAL))
		pQuestFirstInflu = NULL;
	else 
		pQuestFirstInflu = g_pQuestData->FindFirstScenarioQuest(g_pShuttleChild->m_myShuttleInfo.InfluenceType);
	
	int nIndexMax = 0;

//	if(m_vecTabMissionNum.size()>0)
//		nIndexMax = m_nMaxNum;
		nIndexMax = m_nMaxNum;
	if(nIndexMax <= 0)
	{
	}
	else if(m_nMissionViewCase == MISSION_VIEW_ALL && m_nSelectMissionTab == MISS_SCEN)
	{
		// ù��° �̼�ó��
		pQuest = g_pQuestData->FindQuest(nIndexMax);
		wsprintf(strTemp, "%s",pQuest->QuestName);
		
		m_vecMissionList.push_back(strTemp);
		m_vecTabMissionNum.push_back(nIndexMax);
		pQuest = g_pQuestData->FindQuest(pQuest->NextQuestIndex);

		if(pQuest)
		{
			m_vecMissionList.push_back(pQuest->QuestName);
			m_vecTabMissionNum.push_back(pQuest->QuestIndex);
			nIndexMax = 0;
		}		

	}

	if(nIndexMax != 0 && m_nSelectMissionTab == MISS_SCEN && m_nMissionViewCase == MISSION_VIEW_ALL)
	{
		if(g_pQuestData->GetProgressMission() && g_pQuestData->GetProgressMission()->QuestIndex == nIndexMax
			&& pQuestFirstInflu && g_pQuestData->GetProgressMission()->QuestIndex != pQuestFirstInflu->QuestIndex )
		{			
		}
		else
		{
			pQuest = g_pQuestData->FindQuest(nIndexMax);
			if(pQuest)
			{
				int nProM = 0;
				if(g_pQuestData->GetProgressMission())
					nProM = g_pQuestData->GetProgressMission()->QuestIndex;

				if(pQuestFirstInflu && (pQuestFirstInflu->QuestIndex == pQuest->QuestIndex || 
					pQuestFirstInflu->QuestIndex == nProM) && !g_pQuestData->IsQuestCompleted(pQuest->QuestIndex))
					pQuest = g_pQuestData->FindQuest(pQuest->QuestIndex);
				else
					pQuest = g_pQuestData->FindQuest(pQuest->NextQuestIndex);
				if(pQuest)
				{
					// �����Ҽ� �ִ� �̼�
					if(COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pQuest->ReqInfluenceType)){
						m_vecMissionList.push_back(pQuest->QuestName);
						m_vecTabMissionNum.push_back(pQuest->QuestIndex);
					}
					else{						
						if(pQuestFirstInflu){
							m_vecMissionList.push_back(pQuestFirstInflu->QuestName);
							m_vecTabMissionNum.push_back(pQuestFirstInflu->QuestIndex);
						}						
					}
					
					pQuest = g_pQuestData->FindQuest(pQuest->NextQuestIndex);
					if(pQuest
						&& COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pQuest->ReqInfluenceType))
					{
						// ���� �̼�
						m_vecMissionList.push_back(pQuest->QuestName);
						m_vecTabMissionNum.push_back(pQuest->QuestIndex);
					}
				}
			}
		}
	}
	// 2006-03-28 by ispark, �� �ذ� �̼��ǿ� ������ �� �̼� ���� �߰�
	else if(nIndexMax != 0 && m_nSelectMissionTab == MISS_SCEN && m_nMissionViewCase == MISSION_VIEW_UNSOLVE)
	{
		// �� �ذ� �̼��ǿ� �� �����߿� ������ ��ȣ�� �����ͼ� �� ���� �̼� ������ �����´�.
		pQuest = g_pQuestData->FindQuest(nIndexMax);
		if(pQuest && COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pQuest->ReqInfluenceType))
		{
			// ���� �̼�
			m_vecMissionList.push_back(pQuest->QuestName);
			m_vecTabMissionNum.push_back(pQuest->QuestIndex);
		}
	}
	// �ó����� 1���������ֱ�.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	
}

//void CINFMissionInfo::SetMissionInfo()
//{
//	char strTemp[512];
//	ZERO_MEMORY(strTemp);
//	CQuest* pQuest = NULL;
//	CQuest* pQuestFirstInflu = NULL;
//	if(COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_NORMAL))
//		pQuestFirstInflu = NULL;
//	else 
//		pQuestFirstInflu = g_pQuestData->FindFirstScenarioQuest(g_pShuttleChild->m_myShuttleInfo.InfluenceType);
//	
//	int nIndexMax = 0;
//
//	if(m_vecTabMissionNum.size()>0)
//		nIndexMax = m_nMaxNum;
//	else if(m_nMissionViewCase == MISSION_VIEW_ALL && m_nSelectMissionTab == MISS_SCEN)
//	{
//		// ù��° �̼�ó��
//		pQuest = g_pQuestData->FindQuest(m_vecQuestIndex[0]);
//		wsprintf(strTemp, "%s",pQuest->QuestName);
//		
//		m_vecMissionList.push_back(strTemp);
//		m_vecTabMissionNum.push_back(pQuest->QuestIndex);
//		pQuest = g_pQuestData->FindQuest(pQuest->NextQuestIndex);
//
//		if(pQuest)
//		{
//			m_vecMissionList.push_back(pQuest->QuestName);
//			m_vecTabMissionNum.push_back(pQuest->QuestIndex);
//			nIndexMax = 0;
//		}
//
//	}
//
//	if(nIndexMax != 0 && m_nSelectMissionTab == MISS_SCEN)// && m_nMissionViewCase == MISSION_VIEW_ALL)
//	{
//		if(g_pQuestData->GetProgressMission() && g_pQuestData->GetProgressMission()->QuestIndex == nIndexMax
//			&& pQuestFirstInflu && g_pQuestData->GetProgressMission()->QuestIndex != pQuestFirstInflu->QuestIndex )
//		{			
//		}
//		else
//		{
//			pQuest = g_pQuestData->FindQuest(nIndexMax);
//			if(pQuest)
//			{
//				int nProM = 0;
//				if(g_pQuestData->GetProgressMission())
//					nProM = g_pQuestData->GetProgressMission()->QuestIndex;
//
//				if(pQuestFirstInflu && (pQuestFirstInflu->QuestIndex == pQuest->QuestIndex || 
//					pQuestFirstInflu->QuestIndex == nProM) && !g_pQuestData->IsQuestCompleted(pQuest->QuestIndex))
//					pQuest = g_pQuestData->FindQuest(pQuest->QuestIndex);
//				else
//					pQuest = g_pQuestData->FindQuest(pQuest->NextQuestIndex);
//				if(pQuest)
//				{
//					// �����Ҽ� �ִ� �̼�
//					if(COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pQuest->ReqInfluenceType)){
//						m_vecMissionList.push_back(pQuest->QuestName);
//						m_vecTabMissionNum.push_back(pQuest->QuestIndex);
//					}
//					else{						
//						if(pQuestFirstInflu){
//							m_vecMissionList.push_back(pQuestFirstInflu->QuestName);
//							m_vecTabMissionNum.push_back(pQuestFirstInflu->QuestIndex);
//						}						
//					}
//					
//					pQuest = g_pQuestData->FindQuest(pQuest->NextQuestIndex);
//					if(pQuest
//						&& COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pQuest->ReqInfluenceType))
//					{
//						// ���� �̼�
//						m_vecMissionList.push_back(pQuest->QuestName);
//						m_vecTabMissionNum.push_back(pQuest->QuestIndex);
//					}
//				}
//			}
//		}
//	}
//	// 2006-03-28 by ispark, �� �ذ� �̼��ǿ� ������ �� �̼� ���� �߰�
//	else if(nIndexMax != 0 && m_nSelectMissionTab == MISS_SCEN && m_nMissionViewCase == MISSION_VIEW_UNSOLVE)
//	{
//		// �� �ذ� �̼��ǿ� �� �����߿� ������ ��ȣ�� �����ͼ� �� ���� �̼� ������ �����´�.
//		int nindex = m_vecTabMissionNum.size() - 1;
//		pQuest = g_pQuestData->FindQuest(m_vecTabMissionNum[nindex]);
//		pQuest = g_pQuestData->FindQuest(pQuest->NextQuestIndex);
//		if(pQuest && COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pQuest->ReqInfluenceType))
//		{
//			// ���� �̼�
//			m_vecMissionList.push_back(pQuest->QuestName);
//			m_vecTabMissionNum.push_back(pQuest->QuestIndex);
//		}
//	}
//	// �ó����� 1���������ֱ�.
//	//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFMissionInfo::SortPayItem(CQuest *pQuest)
/// \brief		�̼� ���� ������ �ߺ� ����
/// \author		ispark
/// \date		2005-12-07 ~ 2005-12-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFMissionInfo::OverlapSortPayItem(CQuest *pQuest)
{
	m_vecCompensationItemName.clear();

	vector<QUEST_PAY_ITEM>::iterator it = pQuest->EndPayItemVector.begin();
	for(; it != pQuest->EndPayItemVector.end(); it++)
	{
		int nCount = 0;
		QUEST_PAY_ITEM* pPayItem = &*it;
		vector<QUEST_PAY_ITEM>::iterator it2 = m_vecCompensationItemName.begin();

// 2006-11-14 by ispark, ������ �ߺ��� �����ϰ� �״�� ���� ���������� ����
//		for(; it2 != m_vecCompensationItemName.end(); it2++)
//		{
//			QUEST_PAY_ITEM* pPayItem2 = it2;
//			if(pPayItem->ItemNum != 7000022 &&				// SPI�� �ߺ� ���� ���ؾ� �Ѵ�.
//				pPayItem->ItemNum != pPayItem2->ItemNum)	// ItemNum�� �޶�� �Ѵ�.
//			{
//				nCount++;
//			}
//		}
//
//		// �Ȱ��� ���� ����.
//		if(m_vecCompensationItemName.size() == nCount)
		{
			m_vecCompensationItemName.push_back(*pPayItem);
		}
	}
}



