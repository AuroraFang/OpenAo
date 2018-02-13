// INFSelect.cpp: implementation of the CINFSelect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFUnitCreateInfo.h"
#include "INFSelect.h"
#include "ChatMoveData.h"
#include "AtumApplication.h"
#include "GameDataLast.h"
#include "INFSelectBack.h"
#include "INFSelectMenu.h"
#include "INFPilotFace.h"
#include "INFCreateMenu.h"
#include "INFWindow.h"
#include "Interface.h"
#include "ShuttleChild.h"

// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
#include "IMSocketManager.h"
//#include "IMSocket.h"

#include "INFImage.h"
#include "AtumDatabase.h"
#include "INFGameMain.h"
// 2008-09-22 by bhsohn EP3 ĳ���� â
//#include "INFCharacterInfo.h"
#include "TutorialSystem.h"
#include "AtumSound.h"
#include "D3DHanFont.h"

// 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
#include "INFSecuSelectMenu.h"
#include "INFSelectOption.h"
#include "INFImageEx.h"		// 2011. 10. 10 by jskim UI�ý��� ����
#include "INFGroupImage.h"
#include "INFGroupManager.h"

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
#define TUTORIAL_SELECT_SIZE_X	496
#define TUTORIAL_SELECT_SIZE_Y	119
#define TUTORIAL_BUTTON_SIZE_X	30
#define TUTORIAL_BUTTON_SIZE_Y	30

// 2009. 10. 14 by jskim ������ī ���� 
#define INFLUENCE_SELECT_SIZE_X	450
#define INFLUENCE_SELECT_SIZE_Y	130

#define INFLUENCE_BUTTON_SIZE_X	30
#define INFLUENCE_BUTTON_SIZE_Y	30
//end 2009. 10. 14 by jskim ������ī ���� 

// 2007-07-04 by bhsohn ���۷����� �߰�
// NPC�̹��� ��ġ
#define SEL_NPC_POS_X	(g_pD3dApp->GetBackBufferDesc().Width-255)
#define SEL_NPC_POS_Y	(g_pD3dApp->GetBackBufferDesc().Height-481)

// 2007-07-23 by bhsohn ���۷����� �߰��۾�
#define SEL_NPC_BUBBLE_POS_X	(g_pD3dApp->GetBackBufferDesc().Width-387)
#define SEL_NPC_BUBBLE_POS_Y	(g_pD3dApp->GetBackBufferDesc().Height-647)
#define SEL_NPC_BUBBLE_WIDTH	(350)
#define SEL_NPC_BUBBLE_HEIGHT	(150)
#define SEL_NPC_BUBBLE_ARROW_X	(112)
#define	SEL_NPC_STRING_FONT_WIDTH	(SEL_NPC_BUBBLE_WIDTH+20)// �����Ǵ� ��Ʈ ����
#define	STRING_CAP_ONE_STR_HEIGHT	20	// �ѱ����� ����

// ���� ���ݹ�� 
#define UNIT_STATBK_POS_X		(0)
#define UNIT_STATBK_POS_Y		(g_pD3dApp->GetBackBufferDesc().Height/2-40)

// ���̴� ����
#define UNIT_SHOW_STEP			30
#define UNIT_INFO_HIDE_TIME		(0.023f)		// (0.7/UNIT_SHOW_STEP)

// NPC���̴� �ð�
#define	NPC_ALPHA_TIME			(2.0f)
#define NPC_ALPHA_DELAY			(0.0f)

// 2007-07-24 by bhsohn �����̼� mp3�߰�
#define	NARRATION_SELECT_NPC		"op_sel_start"
#define	NARRATION_CREATE_B			"op_sel_b"
#define	NARRATION_CREATE_M			"op_sel_m"
#define	NARRATION_CREATE_A			"op_sel_a"
#define	NARRATION_CREATE_I			"op_sel_i"

// ���� �׵θ�
#define ID_HELP_BALON_TLH				6
#define ID_HELP_BALON_TLW				8
#define ID_HELP_BALON_TMH				6
#define ID_HELP_BALON_TMW				1
#define ID_HELP_BALON_TRH				6
#define ID_HELP_BALON_TRW				6

#define ID_HELP_BALON_MLH				1
#define ID_HELP_BALON_MLW				8
#define ID_HELP_BALON_MMH				1
#define ID_HELP_BALON_MMW				1
#define ID_HELP_BALON_MRH				1
#define ID_HELP_BALON_MRW				6

#define ID_HELP_BALON_BLH				6
#define ID_HELP_BALON_BLW				8
#define ID_HELP_BALON_BMH				6
#define ID_HELP_BALON_BMW				1
#define ID_HELP_BALON_BRH				6
#define ID_HELP_BALON_BRW				6
#else
#define TUTORIAL_SELECT_SIZE_X	496
#define TUTORIAL_SELECT_SIZE_Y	119
#define TUTORIAL_BUTTON_SIZE_X	62
#define TUTORIAL_BUTTON_SIZE_Y	36

// 2009. 10. 14 by jskim ������ī ���� 
#define INFLUENCE_SELECT_SIZE_X	450
#define INFLUENCE_SELECT_SIZE_Y	130

#define INFLUENCE_BUTTON_SIZE_X	123
#define INFLUENCE_BUTTON_SIZE_Y	22
//end 2009. 10. 14 by jskim ������ī ���� 

// 2007-07-04 by bhsohn ���۷����� �߰�
// NPC�̹��� ��ġ
#define SEL_NPC_POS_X	(g_pD3dApp->GetBackBufferDesc().Width-255)
#define SEL_NPC_POS_Y	(g_pD3dApp->GetBackBufferDesc().Height-481)

// 2007-07-23 by bhsohn ���۷����� �߰��۾�
#define SEL_NPC_BUBBLE_POS_X	(g_pD3dApp->GetBackBufferDesc().Width-387)
#define SEL_NPC_BUBBLE_POS_Y	(g_pD3dApp->GetBackBufferDesc().Height-647)
#define SEL_NPC_BUBBLE_WIDTH	(350)
#define SEL_NPC_BUBBLE_HEIGHT	(150)
#define SEL_NPC_BUBBLE_ARROW_X	(112)
#define	SEL_NPC_STRING_FONT_WIDTH	(SEL_NPC_BUBBLE_WIDTH+20)// �����Ǵ� ��Ʈ ����
#define	STRING_CAP_ONE_STR_HEIGHT	20	// �ѱ����� ����

// ���� ���ݹ�� 
#define UNIT_STATBK_POS_X		(0)
#define UNIT_STATBK_POS_Y		(g_pD3dApp->GetBackBufferDesc().Height/2-40)

// ���̴� ����
#define UNIT_SHOW_STEP			30
#define UNIT_INFO_HIDE_TIME		(0.023f)		// (0.7/UNIT_SHOW_STEP)

// NPC���̴� �ð�
#define	NPC_ALPHA_TIME			(2.0f)
#define NPC_ALPHA_DELAY			(0.0f)

// 2007-07-24 by bhsohn �����̼� mp3�߰�
#define	NARRATION_SELECT_NPC		"op_sel_start"
#define	NARRATION_CREATE_B			"op_sel_b"
#define	NARRATION_CREATE_M			"op_sel_m"
#define	NARRATION_CREATE_A			"op_sel_a"
#define	NARRATION_CREATE_I			"op_sel_i"

// ���� �׵θ�
#define ID_HELP_BALON_TLH				6
#define ID_HELP_BALON_TLW				8
#define ID_HELP_BALON_TMH				6
#define ID_HELP_BALON_TMW				1
#define ID_HELP_BALON_TRH				6
#define ID_HELP_BALON_TRW				6

#define ID_HELP_BALON_MLH				1
#define ID_HELP_BALON_MLW				8
#define ID_HELP_BALON_MMH				1
#define ID_HELP_BALON_MMW				1
#define ID_HELP_BALON_MRH				1
#define ID_HELP_BALON_MRW				6

#define ID_HELP_BALON_BLH				6
#define ID_HELP_BALON_BLW				8
#define ID_HELP_BALON_BMH				6
#define ID_HELP_BALON_BMW				1
#define ID_HELP_BALON_BRH				6
#define ID_HELP_BALON_BRW				6
#endif

#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn �±� ���� �߰�
	#define STRING_CULL ::StringCullingUserData_ToBlank
#else
	#define STRING_CULL ::StringCullingUserDataEx	
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFSelect::CINFSelect(CAtumNode* pParent)
{
	FLOG( "CINFSelect(CAtumNode* pParent)" );
	g_pSelect = this;
	m_pParent = pParent;
	m_bRestored = FALSE;
	m_pGameData = NULL;
	m_pSelectBack = NULL;
	m_pSelectMenu = NULL;
	m_pCreateMenu = NULL;
	m_pPilotFace = NULL;
	m_pInfWindow = NULL;
	m_pNormalMouse[0] = NULL;
	m_pNormalMouse[1] = NULL;
	m_nMouseState = 0;
	memset((void*)m_guiUnitInfo, 0x00, sizeof(FC_CONNECT_LOGIN_INFO)*SELECT_MODE);
	memset((void*)m_character, 0x00, sizeof(CHARACTER)*SELECT_MODE);
	m_ptMouse.x = g_pD3dApp->GetBackBufferDesc().Width / 1024.0f;
	m_ptMouse.y = g_pD3dApp->GetBackBufferDesc().Height / 768.0f;
	m_bDelete = FALSE;			// ���� ���� ������ 

	// 2005-03-03 by jschoi
	m_pRequestEnableBack		= NULL;
	m_pRequestEnableTriangle	= NULL;
	ZERO_MEMORY(m_strAutoStat);

	// 2005-04-19 by jschoi - Tutorial
	m_bTutorialMenu = FALSE;
	m_bStartButtonClick = FALSE;
	m_bSkipButtonClick = FALSE;
	m_nStartButtonState = 0;
	m_nSkipButtonState = 0;
	m_pTutorialSelectImage = NULL;
	int i;
	for(i = 0; i < 3;i++)
	{
		m_pStartButton[i] = NULL;
		m_pSkipButton[i] = NULL;	
	}
	// 2012-02-28 by mspark, Ʃ�丮�� ���� ��(Ʃ�丮���� �����Ͻðڽ��ϱ�?) ȭ�鿡�� ��� ��Ӱ� ó��
	m_pImgFadeBG = NULL;
	// end 2012-02-28 by mspark, Ʃ�丮�� ���� ��(Ʃ�丮���� �����Ͻðڽ��ϱ�?) ȭ�鿡�� ��� ��Ӱ� ó��
	// 2009. 10. 14 by jskim ������ī ����
	m_pInfluenceSelectImage = NULL;
	for(i = 0; i < 3; i++)
	{
		m_pInfluenceSelectANI[i] = NULL;
		m_pInfluenceSelectBCU[i] = NULL;
		
	}
	m_pCloseBtn =NULL;
	m_binfluenceSelect = FALSE;
	m_bANIClick = FALSE;
	m_bBCUClick = FALSE;	
	m_nANIButtonState = 0;
	m_nBCUButtonState = 0;
	m_pCloseBtn = NULL;
	//end 2009. 10. 14 by jskim ������ī ����
	// 2007-07-04 by bhsohn ���۷����� �߰�
	m_nShowGear = -1;
	m_nUnitPopupGear= -1;

	m_pOpNPCImage = NULL;	
	int nCnt = 0;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
// 2011. 10. 10 by jskim UI�ý��� ����
	for(nCnt = 0;nCnt < MAX_UNIT_STAT;nCnt++)
	{
		m_pUnitStatImage[nCnt] = NULL;
	}
	// end 2011. 10. 10 by jskim UI�ý��� ����	 
#endif
	m_fUnitHideTime = 0.0f;
	
	m_nUnitShowStep  = 0;
	m_nUnitStatMode = UNIT_STAT_POPUP;
	m_fNPCViewTime = m_fNPCAlpahTotalTime = NPC_ALPHA_TIME;	
	m_dwNPCAlpha = 0;
	m_bNPCRenderFlag = TRUE;
	m_fFirstNPCViewTime = 1.0f;
	// end 2007-07-04 by bhsohn ���۷����� �߰�

	// 2007-07-23 by bhsohn ���۷����� �߰��۾�	
	for(nCnt=0; nCnt<SELECT_CHARATER_BALON; nCnt++)
	{
		m_pBalonChat[nCnt] = NULL;
	}
	m_vecText.clear();

	m_pFontHelpInfo = NULL;

	m_nNarrationGear = -1;
	// end 2007-07-23 by bhsohn ���۷����� �߰��۾�

	// 2007-07-24 by bhsohn �����̼� mp3�߰�
	m_bFirstOpNPCRender = FALSE;

	// 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����	
	m_pINFSecuSelectMenu = NULL;
}


CINFSelect::~CINFSelect()
{
	FLOG( "~CINFSelect()" );
	g_pSelect = NULL;
	util::del(m_pSelectBack);
	util::del(m_pSelectMenu);
	util::del(m_pCreateMenu);
	util::del(m_pPilotFace);
	util::del(m_pInfWindow);
	util::del(m_pGameData);
	util::del( m_pNormalMouse[0] ) ;
	util::del( m_pNormalMouse[1] ) ;
	util::del(m_pUnitCreateInfo);

	// 2005-03-03 by jschoi
	util::del(m_pRequestEnableBack);
	util::del(m_pRequestEnableTriangle);
	// 2005-04-19 by jschoi
	util::del(m_pTutorialSelectImage);
	int i;
	for(i = 0; i < 3;i++)
	{
		util::del(m_pStartButton[i]);
		util::del(m_pSkipButton[i]);				
	}
	// 2012-02-28 by mspark, Ʃ�丮�� ���� ��(Ʃ�丮���� �����Ͻðڽ��ϱ�?) ȭ�鿡�� ��� ��Ӱ� ó��
	util::del(m_pImgFadeBG);
	// end 2012-02-28 by mspark, Ʃ�丮�� ���� ��(Ʃ�丮���� �����Ͻðڽ��ϱ�?) ȭ�鿡�� ��� ��Ӱ� ó��
	// 2009. 10. 14 by jskim ������ī ���� 
	util::del(m_pInfluenceSelectImage);
	for(i = 0; i < 3;i++)
	{
		util::del(m_pInfluenceSelectANI[i]);
		util::del(m_pInfluenceSelectBCU[i]);
		
	}
	util::del(m_pCloseBtn);
	//end 2009. 10. 14 by jskim ������ī ���� 
	// 2007-07-04 by bhsohn ���۷����� �߰�
	util::del(m_pOpNPCImage);	
	
	int nCnt = 0;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
// 2011. 10. 10 by jskim UI�ý��� ����
	for(nCnt = 0;nCnt < MAX_UNIT_STAT;nCnt++)
	{
		util::del(m_pUnitStatImage[nCnt]);	 
 	}	// end 2011. 10. 10 by jskim UI�ý��� ����												  
#endif
	for(nCnt=0; nCnt<SELECT_CHARATER_BALON; nCnt++)
	{
		util::del(m_pBalonChat[nCnt]);
	}

	// 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
	if(m_pINFSecuSelectMenu)
	{
		m_pINFSecuSelectMenu->DeleteDeviceObjects();
		util::del(m_pINFSecuSelectMenu);
	}	 
}

HRESULT CINFSelect::InitDeviceObjects()
{
	FLOG( "CINFSelect::InitDeviceObjects()" );
	char strPath[256];
	g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_TEXTURE, "select.tex");
	SetResourceFile(strPath);

	// 2008-10-15 by bhsohn ���ҽ� �޸� ��ȣ ��� �߰�
	{
		DataHeader	* pDataHeaderDummy = NULL;
		pDataHeaderDummy = FindResource(INTERFCE_NOT_HAVE_TEXTURE);	// ����Ʈ �̹���
		if(pDataHeaderDummy && g_pInterface)
		{
			g_pInterface->SetDummyDataHeader(pDataHeaderDummy);
		}
	}	
	// end 2008-10-15 by bhsohn ���ҽ� �޸� ��ȣ ��� �߰�

	m_pINFSelectOption = new CINFSelectOption;
	m_pINFSelectOption->SetGameData(m_pGameData);
	m_pINFSelectOption->InitDeviceObjects();

	// 2005-03-03 by jschoi
	DataHeader	* pDataHeader = NULL;

	m_pRequestEnableBack = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
	pDataHeader = FindResource("rq_enb");
	m_pRequestEnableBack->InitDeviceObjects( pDataHeader );

	m_pRequestEnableTriangle = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
	pDataHeader = FindResource("rq_ent");
	m_pRequestEnableTriangle->InitDeviceObjects( pDataHeader );


	m_pSelectBack = new CINFSelectBack(this);
	m_pSelectBack->InitDeviceObjects();

	m_pSelectBack->ChangeMode(SELECT_MODE);
//	g_pD3dApp->ChangeGameState( _SELECT ); // 2004-06-03 by dhkwon ����

	m_pSelectMenu = new CINFSelectMenu(this);
	m_pSelectMenu->SetGameData(m_pGameData);
	m_pSelectMenu->InitDeviceObjects();

	m_pCreateMenu = new CINFCreateMenu(this);
	m_pCreateMenu->SetGameData(m_pGameData);
	m_pCreateMenu->InitDeviceObjects();

	// add by jsy ���� �����ϴ� �� ������ ����
	m_pUnitCreateInfo = new CINFUnitCreateInfo(this);
	m_pUnitCreateInfo->SetGameData(m_pGameData);
	m_pUnitCreateInfo->InitDeviceObjects();

	m_pInfWindow = new CINFWindow(this);
	m_pInfWindow->SetGameData(m_pGameData);
	m_pInfWindow->InitMsgBoxObjects();

	m_pPilotFace = new CINFPilotFace();
	m_pPilotFace->InitDeviceObjects("face.tex");

//	DataHeader	* pDataHeader;
	pDataHeader = FindResource("arrow1");
	if(pDataHeader)
	{
		m_pNormalMouse[0] = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
		m_pNormalMouse[0]->InitDeviceObjects( pDataHeader );
	}
	pDataHeader = FindResource("arrow2");
	if(pDataHeader)
	{
		m_pNormalMouse[1] = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
		m_pNormalMouse[1]->InitDeviceObjects( pDataHeader );
	}

	pDataHeader = FindResource("INTRO");
	if(pDataHeader)
	{
		m_pTutorialSelectImage = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
		m_pTutorialSelectImage->InitDeviceObjects( pDataHeader );
	}

	// 2012-02-28 by mspark, Ʃ�丮�� ���� ��(Ʃ�丮���� �����Ͻðڽ��ϱ�?) ȭ�鿡�� ��� ��Ӱ� ó��
	pDataHeader = FindResource("LM_inven");
	if(pDataHeader)
	{
		m_pImgFadeBG = new CINFImageEx;
		if(m_pImgFadeBG)
		{
		m_pImgFadeBG->InitDeviceObjects(pDataHeader);
	}
	}
	// end 2012-02-28 by mspark, Ʃ�丮�� ���� ��(Ʃ�丮���� �����Ͻðڽ��ϱ�?) ȭ�鿡�� ��� ��Ӱ� ó��

	// 2009. 10. 14 by jskim ������ī ����
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	pDataHeader = m_pSelectMenu->m_GruopSelectmanager->FindResource("cityS");
	m_pInfluenceSelectImage = m_pSelectMenu->m_GruopSelectmanager->GetGroupImage( pDataHeader );
	m_pInfluenceSelectImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
#else
	pDataHeader = FindResource("inf_sel");
	if(pDataHeader)
	{
		m_pInfluenceSelectImage = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
		m_pInfluenceSelectImage->InitDeviceObjects( pDataHeader );
	}
#endif
	
	pDataHeader = FindResource("inf_ani3");
	if(pDataHeader)
	{
		m_pInfluenceSelectANI[0] = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
		m_pInfluenceSelectANI[0]->InitDeviceObjects( pDataHeader );
	}
	pDataHeader = FindResource("inf_ani0");
	if(pDataHeader)
	{
		m_pInfluenceSelectANI[1] = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
		m_pInfluenceSelectANI[1]->InitDeviceObjects( pDataHeader );
	}
	pDataHeader = FindResource("inf_ani1");
	if(pDataHeader)
	{
		m_pInfluenceSelectANI[2] = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
		m_pInfluenceSelectANI[2]->InitDeviceObjects( pDataHeader );
	}
	pDataHeader = FindResource("inf_bcu3");
	if(pDataHeader)
	{
		m_pInfluenceSelectBCU[0] = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
		m_pInfluenceSelectBCU[0]->InitDeviceObjects( pDataHeader );
	}
	pDataHeader = FindResource("inf_bcu0");
	if(pDataHeader)
	{
		m_pInfluenceSelectBCU[1] = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
		m_pInfluenceSelectBCU[1]->InitDeviceObjects( pDataHeader );
	}
	pDataHeader = FindResource("inf_bcu1");
	if(pDataHeader)
	{
		m_pInfluenceSelectBCU[2] = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
		m_pInfluenceSelectBCU[2]->InitDeviceObjects( pDataHeader );
	}
	pDataHeader = FindResource("xclose");
	if(pDataHeader)
	{
		m_pCloseBtn = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
		m_pCloseBtn->InitDeviceObjects( pDataHeader );
	}	
	//end 2009. 10. 14 by jskim ������ī ����
	pDataHeader = FindResource("st_dflt");
	if(pDataHeader)
	{
		m_pStartButton[0] = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
		m_pStartButton[0]->InitDeviceObjects( pDataHeader ) ;
	}

	pDataHeader = FindResource("st_ov");
	if(pDataHeader)
	{
		m_pStartButton[1] = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
		m_pStartButton[1]->InitDeviceObjects( pDataHeader ) ;
	}

	pDataHeader = FindResource("st_psh");
	if(pDataHeader)
	{
		m_pStartButton[2] = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
		m_pStartButton[2]->InitDeviceObjects( pDataHeader ) ;
	}

	pDataHeader = FindResource("skip_dft");
	if(pDataHeader)
	{
		m_pSkipButton[0] = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
		m_pSkipButton[0]->InitDeviceObjects( pDataHeader ) ;
	}
	
	pDataHeader = FindResource("skip_ov");
	if(pDataHeader)
	{
		m_pSkipButton[1] = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
		m_pSkipButton[1]->InitDeviceObjects( pDataHeader ) ;
	}
	
	pDataHeader = FindResource("skip_psh");
	if(pDataHeader)
	{
		m_pSkipButton[2] = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
		m_pSkipButton[2]->InitDeviceObjects( pDataHeader ) ;
	}

	// 2007-07-04 by bhsohn ���۷����� �߰�	
	pDataHeader = FindResource("npcsel");
	if(pDataHeader)
	{
		m_pOpNPCImage = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
		m_pOpNPCImage->InitDeviceObjects( pDataHeader ) ;
	}	
	

	int nCnt = 0;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
// 2011. 10. 10 by jskim UI�ý��� ����
	char chUnistImg[MAX_UNIT_STAT][30] = 
	{
		{"selb"},		// B
		{"seli"},		// I
		{"selm"},		// M
		{"sela"}		// A
	};
	for(nCnt = 0;nCnt < MAX_UNIT_STAT;nCnt++)
	{
		pDataHeader = FindResource(chUnistImg[nCnt]);
		if(pDataHeader)
		{
 			m_pUnitStatImage[nCnt] = new CINFImageEx;
 			m_pUnitStatImage[nCnt]->InitDeviceObjects(pDataHeader) ;
 		}	
 	}
// end 2011. 10. 10 by jskim UI�ý��� ����	   
#endif
	// 2007-07-23 by bhsohn ���۷����� �߰��۾�
	{
		DataHeader	* pDataHeader = NULL;
		char szImage[16];
		for(nCnt=0; nCnt<SELECT_CHARATER_BALON; nCnt++)
		{		
			memset(szImage, 0x00, 16);		
			wsprintf(szImage,"sel_bal%d",nCnt);
			
			m_pBalonChat[nCnt] = new CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
			// ���۷��̼� ���� �̹��� �ε�
			pDataHeader = FindResource(szImage);
			m_pBalonChat[nCnt]->InitDeviceObjects( pDataHeader ) ;		
		}			
	}

	{
		m_pFontHelpInfo = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 8, D3DFONT_ZENABLE, TRUE, SEL_NPC_STRING_FONT_WIDTH, 32);
		m_pFontHelpInfo->InitDeviceObjects(g_pD3dDev) ;
	}

	// end 2007-07-23 by bhsohn ���۷����� �߰��۾�

	return S_OK ;
}
void CINFSelect::InitOpString()
{
	m_vecText.clear();
	STRING_CULL(STRMSG_C_070718_0230, SEL_NPC_BUBBLE_WIDTH-(2*ID_HELP_BALON_TLW), &m_vecText, m_pFontHelpInfo);
	
}
HRESULT CINFSelect::RestoreDeviceObjects()
{
	FLOG( "CINFSelect::RestoreDeviceObjects()" );
	if(!m_bRestored)
	{
		m_pINFSelectOption->RestoreDeviceObjects();
		m_pSelectBack->RestoreDeviceObjects();
		m_pSelectMenu->RestoreDeviceObjects();
		m_pCreateMenu->RestoreDeviceObjects();
		// add by jsy ���� �����ϴ� �� ������ ����
		m_pUnitCreateInfo->RestoreDeviceObjects();
		m_pInfWindow->RestoreMsgBoxObjects();
		m_pPilotFace->RestoreDeviceObjects();
		if(m_pNormalMouse[0]) 
			m_pNormalMouse[0]->RestoreDeviceObjects();	
		if(m_pNormalMouse[1]) 
			m_pNormalMouse[1]->RestoreDeviceObjects();	

		// 2005-03-03 by jschoi
		m_pRequestEnableBack->RestoreDeviceObjects();
		m_pRequestEnableTriangle->RestoreDeviceObjects();

		// 2005-04-19 by jschoi - Tutorial
		m_bTutorialMenu = FALSE;
		m_bStartButtonClick = FALSE;
		m_bSkipButtonClick = FALSE;
		m_nStartButtonState = 0;
		m_nSkipButtonState = 0;
		m_pTutorialSelectImage->RestoreDeviceObjects();
		// 2012-02-28 by mspark, Ʃ�丮�� ���� ��(Ʃ�丮���� �����Ͻðڽ��ϱ�?) ȭ�鿡�� ��� ��Ӱ� ó��
		if(m_pImgFadeBG)
		{
		m_pImgFadeBG->RestoreDeviceObjects();
		}		
		// end 2012-02-28 by mspark, Ʃ�丮�� ���� ��(Ʃ�丮���� �����Ͻðڽ��ϱ�?) ȭ�鿡�� ��� ��Ӱ� ó��
		int i;
		for(i = 0; i < 3;i++)
		{
			m_pStartButton[i]->RestoreDeviceObjects();
			m_pSkipButton[i]->RestoreDeviceObjects();
		}
		// 2009. 10. 14 by jskim ������ī ����	
		m_binfluenceSelect = FALSE;
		m_bANIClick = FALSE;
		m_bBCUClick = FALSE;
		m_nANIButtonState = 0;
		m_nBCUButtonState = 0;
		m_pInfluenceSelectImage->RestoreDeviceObjects();
		for(i = 0; i < 3;i++)
		{			
			m_pInfluenceSelectANI[i]->RestoreDeviceObjects();
			m_pInfluenceSelectBCU[i]->RestoreDeviceObjects();
		}
		m_pCloseBtn->RestoreDeviceObjects();
		//end 2009. 10. 14 by jskim ������ī ����
		// 2007-07-04 by bhsohn ���۷����� �߰�		
		m_pOpNPCImage->RestoreDeviceObjects();		
		int nCnt = 0;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
// 2011. 10. 10 by jskim UI�ý��� ����
		for(nCnt = 0;nCnt < MAX_UNIT_STAT;nCnt++)
		{
			m_pUnitStatImage[nCnt]->RestoreDeviceObjects();
		}
// end 2011. 10. 10 by jskim UI�ý��� ���� 
#endif
		// 2007-07-23 by bhsohn ���۷����� �߰��۾�
		for(nCnt=0; nCnt<SELECT_CHARATER_BALON; nCnt++)
		{
			if(m_pBalonChat[nCnt])
			{
				m_pBalonChat[nCnt]->RestoreDeviceObjects();
			}
		}

		// end 2007-07-23 by bhsohn ���۷����� �߰��۾�
		
		// 2007-07-23 by bhsohn ���۷����� �߰��۾�
		if(m_pFontHelpInfo)
		{
			m_pFontHelpInfo->RestoreDeviceObjects();
			InitOpString();
		}
		// end 2007-07-23 by bhsohn ���۷����� �߰��۾�

		// 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
		if(m_pINFSecuSelectMenu)
		{
			m_pINFSecuSelectMenu->RestoreDeviceObjects();
		}
		// end 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����


		m_bRestored = TRUE;
	}
	return S_OK ;
}

HRESULT CINFSelect::InvalidateDeviceObjects()
{
	FLOG( "CINFSelect::InvalidateDeviceObjects()" );
	if(m_bRestored)
	{
		m_pINFSelectOption->InvalidateDeviceObjects();
		m_pSelectBack->InvalidateDeviceObjects();
		m_pSelectMenu->InvalidateDeviceObjects();
		m_pCreateMenu->InvalidateDeviceObjects();
		m_pUnitCreateInfo->InvalidateDeviceObjects();
		m_pInfWindow->InvalidateMsgBoxObjects();
		m_pPilotFace->InvalidateDeviceObjects();
		if(m_pNormalMouse[0]) 
			m_pNormalMouse[0]->InvalidateDeviceObjects();	
		if(m_pNormalMouse[1]) 
			m_pNormalMouse[1]->InvalidateDeviceObjects();	

		// 2005-03-03 by jschoi
		m_pRequestEnableBack->InvalidateDeviceObjects();
		m_pRequestEnableTriangle->InvalidateDeviceObjects();
		// 2005-04-19 by jschoi - Turorial
		m_pTutorialSelectImage->InvalidateDeviceObjects();
		// 2012-02-28 by mspark, Ʃ�丮�� ���� ��(Ʃ�丮���� �����Ͻðڽ��ϱ�?) ȭ�鿡�� ��� ��Ӱ� ó��
		if(m_pImgFadeBG)
		{
		m_pImgFadeBG->InvalidateDeviceObjects();
		}
		// end 2012-02-28 by mspark, Ʃ�丮�� ���� ��(Ʃ�丮���� �����Ͻðڽ��ϱ�?) ȭ�鿡�� ��� ��Ӱ� ó��
		int i;
		for(i = 0; i < 3;i++)
		{
			m_pStartButton[i]->InvalidateDeviceObjects();
			m_pSkipButton[i]->InvalidateDeviceObjects();		
		}		
		// 2009. 10. 14 by jskim ������ī ����	
		m_pInfluenceSelectImage->InvalidateDeviceObjects();
		for(i = 0; i < 3;i++)
		{
			m_pInfluenceSelectANI[i]->InvalidateDeviceObjects();
			m_pInfluenceSelectBCU[i]->InvalidateDeviceObjects();
		}
		m_pCloseBtn->InvalidateDeviceObjects();
		//end 2009. 10. 14 by jskim ������ī ����	
		// 2007-07-04 by bhsohn ���۷����� �߰�				
		m_pOpNPCImage->InvalidateDeviceObjects();		
		int nCnt = 0;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
// 2011. 10. 10 by jskim UI�ý��� ����
		for(nCnt = 0;nCnt < MAX_UNIT_STAT;nCnt++)
		{
			m_pUnitStatImage[nCnt]->InvalidateDeviceObjects();
		}
// end 2011. 10. 10 by jskim UI�ý��� ����					   
#endif

		// 2007-07-23 by bhsohn ���۷����� �߰��۾�
		for(nCnt=0; nCnt<SELECT_CHARATER_BALON; nCnt++)
		{
			if(m_pBalonChat[nCnt])
			{
				m_pBalonChat[nCnt]->InvalidateDeviceObjects();
			}		
		}
		// end 2007-07-23 by bhsohn ���۷����� �߰��۾�

		// 2007-07-23 by bhsohn ���۷����� �߰��۾�
		if(m_pFontHelpInfo)
		{
			m_pFontHelpInfo->InvalidateDeviceObjects();		
		}
		// end 2007-07-23 by bhsohn ���۷����� �߰��۾�
		// 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
		if(m_pINFSecuSelectMenu)
		{
			m_pINFSecuSelectMenu->InvalidateDeviceObjects();
		}
		// end 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����

		m_bRestored = FALSE;
	}
	return S_OK ;
}

HRESULT CINFSelect::DeleteDeviceObjects()
{
	FLOG( "CINFSelect::DeleteDeviceObjects()" );
	m_pINFSelectOption->DeleteDeviceObjects();
	m_pSelectBack->DeleteDeviceObjects();
	m_pSelectMenu->DeleteDeviceObjects();
	m_pCreateMenu->DeleteDeviceObjects();
	// add by jsy ���� �����ϴ� �� ������ ����
	m_pUnitCreateInfo->DeleteDeviceObjects();
	m_pInfWindow->DeleteMsgBoxObjects();
	m_pPilotFace->DeleteDeviceObjects();

	util::del(m_pUnitCreateInfo);
	util::del(m_pSelectBack);
	util::del(m_pSelectMenu);
	util::del(m_pCreateMenu);
	util::del(m_pInfWindow);
	util::del(m_pPilotFace);
	util::del(m_pGameData);

	if(m_pNormalMouse[0]) 
	{
		m_pNormalMouse[0]->DeleteDeviceObjects();	
		util::del(m_pNormalMouse[0]);
	}
	if(m_pNormalMouse[1]) 
	{
		m_pNormalMouse[1]->DeleteDeviceObjects();	
		util::del(m_pNormalMouse[1]);
	}

	// 2005-03-03 by jschoi
	m_pRequestEnableBack->DeleteDeviceObjects();
	m_pRequestEnableTriangle->DeleteDeviceObjects();
	util::del(m_pRequestEnableBack);
	util::del(m_pRequestEnableTriangle);
	// 2005-04-19 by jschoi - Tutorial 
	m_pTutorialSelectImage->DeleteDeviceObjects();
	util::del(m_pTutorialSelectImage);	
	// 2012-02-28 by mspark, Ʃ�丮�� ���� ��(Ʃ�丮���� �����Ͻðڽ��ϱ�?) ȭ�鿡�� ��� ��Ӱ� ó��
	if(m_pImgFadeBG)
	{
	m_pImgFadeBG->DeleteDeviceObjects();
	util::del(m_pImgFadeBG);
	}
	// end 2012-02-28 by mspark, Ʃ�丮�� ���� ��(Ʃ�丮���� �����Ͻðڽ��ϱ�?) ȭ�鿡�� ��� ��Ӱ� ó��
	int i;
	for(i = 0; i < 3;i++)
	{
		m_pStartButton[i]->DeleteDeviceObjects();
		util::del(m_pStartButton[i]);
		m_pSkipButton[i]->DeleteDeviceObjects();
		util::del(m_pSkipButton[i]);	
	}	
	// 2009. 10. 14 by jskim ������ī ����	
	m_pInfluenceSelectImage->DeleteDeviceObjects();
	util::del(m_pInfluenceSelectImage);
	for(i = 0; i < 3;i++)
	{
		m_pInfluenceSelectANI[i]->DeleteDeviceObjects();
		util::del(m_pInfluenceSelectANI[i]);
		m_pInfluenceSelectBCU[i]->DeleteDeviceObjects();
		util::del(m_pInfluenceSelectBCU[i]);
	}
	m_pCloseBtn->DeleteDeviceObjects();
	util::del(m_pCloseBtn);
	//end 2009. 10. 14 by jskim ������ī ����	
	// 2007-07-04 by bhsohn ���۷����� �߰�				
	m_pOpNPCImage->DeleteDeviceObjects();
	util::del(m_pOpNPCImage);

	int nCnt = 0;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
// 2011. 10. 10 by jskim UI�ý��� ����
	for(nCnt = 0;nCnt < MAX_UNIT_STAT;nCnt++)
	{
		m_pUnitStatImage[nCnt]->DeleteDeviceObjects();
		util::del(m_pUnitStatImage[nCnt]);		
	}
// end 2011. 10. 10 by jskim UI�ý��� ����	 
#endif

	// 2007-07-23 by bhsohn ���۷����� �߰��۾�
	for(nCnt=0; nCnt<SELECT_CHARATER_BALON; nCnt++)
	{
		if(m_pBalonChat[nCnt])
		{
			m_pBalonChat[nCnt]->DeleteDeviceObjects();
			util::del(m_pBalonChat[nCnt]);
		}		
	}
	// end 2007-07-23 by bhsohn ���۷����� �߰��۾�

	// 2007-07-23 by bhsohn ���۷����� �߰��۾�
	if(m_pFontHelpInfo)
	{
		m_pFontHelpInfo->DeleteDeviceObjects();
		util::del(m_pFontHelpInfo);
	}
	// end 2007-07-23 by bhsohn ���۷����� �߰��۾�

	// 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
	if(m_pINFSecuSelectMenu)
	{		
		m_pINFSecuSelectMenu->DeleteDeviceObjects();
		util::del(m_pINFSecuSelectMenu);
	}
	// end 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
	return S_OK ;
}

void CINFSelect::Tick()
{
	FLOG( "CINFSelect::Tick()" );
	switch(g_pD3dApp->m_dwGameState)
	{
	case _SELECT:
		{
			m_pSelectBack->Tick();
			m_pSelectMenu->Tick();
			m_pInfWindow->Tick();
			m_pINFSelectOption->Tick();

			// 2007-07-04 by bhsohn ���۷����� �߰�	
			TickNPCColor(g_pD3dApp->m_dwGameState);
			
			// 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
			if(m_pINFSecuSelectMenu)
			{
				m_pINFSecuSelectMenu->Render();
			}
			// end 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
		}
		break;
	case _CREATE:
		{
			m_pSelectBack->Tick();
			m_pINFSelectOption->Tick();

			// 2007-07-04 by bhsohn ���۷����� �߰�	
			TickNPCColor(g_pD3dApp->m_dwGameState);
			TickGearStat(m_pSelectBack->GetCurrentSelectIndex());

			//add by jsy
			m_pUnitCreateInfo->Tick( g_pD3dApp->GetElapsedTime() );
			m_pCreateMenu->Tick();
			m_pInfWindow->Tick();
		}
		break;
	default:
		{
		}
	}
}

void CINFSelect::Render()
{
	FLOG( "CINFSelect::Render()" );
	switch(g_pD3dApp->m_dwGameState)
	{
	case _SELECT:
		{
			// 2010-06-07 by dgwoo ĳ���ͼ���â ���ҽ� ���濡 ���� ����Ʈ ���� ����.
// 			m_pSelectBack->Render();
// 			g_pD3dDev->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
// 			m_pSelectBack->RenderSelectUnit();
// 			g_pD3dDev->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
			//end 2010-06-07 by dgwoo ĳ���ͼ���â ���ҽ� ���濡 ���� ����Ʈ ���� ����.

			// 2007-07-04 by bhsohn ���۷����� �߰�
			// ĳ���� ������ 0�̴�
			if(GetCharacterNumber() == 0)
			{
				RenderNPC(_SELECT);
			}

			m_pSelectMenu->Render();
			m_pInfWindow->RenderMessaegBox();
			if(SET_DELETEMENU_CANDIDATE == g_nRenderCandidate)
				g_pD3dApp->RenderCandidate(m_pCreateMenu->m_nCharacterNamePos[0], m_pCreateMenu->m_nCharacterNamePos[1]-15);

			// 2007-05-21 by bhsohn China IME Working
			g_pD3dApp->RenderIMEType(m_pCreateMenu->m_nCharacterNamePos[0], m_pCreateMenu->m_nCharacterNamePos[1]-15);

			// 2009. 10. 14 by jskim ������ī ���� 
			if(m_binfluenceSelect)
			{					
				const int nInfluenceSelectPos_X = g_pD3dApp->GetBackBufferDesc().Width/2 - INFLUENCE_SELECT_SIZE_X/2;
				const int nInfluenceSelectPos_Y = g_pD3dApp->GetBackBufferDesc().Height/3 - INFLUENCE_SELECT_SIZE_Y/2;
				const int nBCUButtonPos_X = nInfluenceSelectPos_X + INFLUENCE_SELECT_SIZE_X*2/5 - INFLUENCE_BUTTON_SIZE_X;
 				const int nBCUButtonPos_Y = nInfluenceSelectPos_Y+ INFLUENCE_BUTTON_SIZE_Y * 4;
				const int nANIButtonPos_X = nInfluenceSelectPos_X + INFLUENCE_SELECT_SIZE_X*3/5;
				const int nANIButtonPos_Y = nInfluenceSelectPos_Y+ INFLUENCE_BUTTON_SIZE_Y * 4;

				const int nCloseButtonPos_X = nInfluenceSelectPos_X + INFLUENCE_SELECT_SIZE_X - 15;
				const int nCloseButtonPos_Y = nInfluenceSelectPos_Y + 5;

				m_pInfluenceSelectImage->Move(nInfluenceSelectPos_X,nInfluenceSelectPos_Y);
				m_pInfluenceSelectImage->Render();

				m_pInfluenceSelectANI[m_nANIButtonState]->Move(nANIButtonPos_X,nANIButtonPos_Y);
				m_pInfluenceSelectANI[m_nANIButtonState]->Render();
				m_pInfluenceSelectBCU[m_nBCUButtonState]->Move(nBCUButtonPos_X,nBCUButtonPos_Y);
				m_pInfluenceSelectBCU[m_nBCUButtonState]->Render();
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
				m_pCloseBtn->Move(nCloseButtonPos_X,nCloseButtonPos_Y);
				m_pCloseBtn->Render();
#endif

 				char chMsgBuff[256];
				ZERO_MEMORY(chMsgBuff);
								wsprintf(chMsgBuff, STRMSG_C_091014_0401);	//"������ ���ø� ������ �ּ���"

				SIZE size = m_pFontHelpInfo->GetStringSize(chMsgBuff);
 				m_pFontHelpInfo->DrawText(g_pD3dApp->GetBackBufferDesc().Width/2 - size.cx / 2,
 					nInfluenceSelectPos_Y + INFLUENCE_BUTTON_SIZE_Y,
 					NAME_COLOR_NORMAL,
 					chMsgBuff);	
				
				// �ش� ��Ʈ���� ©�� ���κ��� vecMessage�� ����ִ´�
				vector<string> vecMessage;
				STRING_CULL( STRMSG_C_091014_0402, 330, &vecMessage, m_pFontHelpInfo );	// "(���� ���� �̼��� 11���� ���� ���� ���� �� �� ������ �ش� �̼��� ���� ���������� ������ �����ϰ� �˴ϴ�.)"
				
				int		nRenderLine = 2;		// 2��° ���κ��� ����
				string	str;
				char	szBuff[1024];
				// vecMessage ����
				for( vector<string>::iterator it = vecMessage.begin(); it != vecMessage.end(); ++it, ++nRenderLine )
				{
					// �� ���κ��� �߾� �����Ͽ� �׷��ش�
					string str = (*it);
					strcpy( szBuff, str.c_str() );
					size = m_pFontHelpInfo->GetStringSize( szBuff );
					m_pFontHelpInfo->DrawText( g_pD3dApp->GetBackBufferDesc().Width/2 - size.cx / 2,
											   nInfluenceSelectPos_Y + INFLUENCE_BUTTON_SIZE_Y * nRenderLine,
											   NAME_COLOR_NORMAL,
											   szBuff );
				}
			}
			//end 2009. 10. 14 by jskim ������ī ���� 
			if(m_bTutorialMenu)
			{
				// 2005-04-19 by jschoi - Tutorial 
				const int nTutorialSelectPos_X = g_pD3dApp->GetBackBufferDesc().Width/2 - TUTORIAL_SELECT_SIZE_X/2;
				const int nTutorialSelectPos_Y = g_pD3dApp->GetBackBufferDesc().Height/3 - TUTORIAL_SELECT_SIZE_Y/2;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM		
				const int nStartButtonPos_X = nTutorialSelectPos_X + TUTORIAL_SELECT_SIZE_X/2 - TUTORIAL_BUTTON_SIZE_X;
				const int nStartButtonPos_Y = nTutorialSelectPos_Y + TUTORIAL_SELECT_SIZE_Y;
				const int nSkipButtonPos_X = nTutorialSelectPos_X + TUTORIAL_SELECT_SIZE_X/2 + 15;
				const int nSkipButtonPos_Y = nTutorialSelectPos_Y + TUTORIAL_SELECT_SIZE_Y;
#else
				const int nStartButtonPos_X = nTutorialSelectPos_X + TUTORIAL_SELECT_SIZE_X*2/5 - TUTORIAL_BUTTON_SIZE_X/2;
				const int nStartButtonPos_Y = nTutorialSelectPos_Y + TUTORIAL_SELECT_SIZE_Y;
				const int nSkipButtonPos_X = nTutorialSelectPos_X + TUTORIAL_SELECT_SIZE_X*3/5 - TUTORIAL_BUTTON_SIZE_X/2;
				const int nSkipButtonPos_Y = nTutorialSelectPos_Y + TUTORIAL_SELECT_SIZE_Y;
#endif
//				m_nStartButtonState = 0;
//				m_nSkipButtonState = 0;

				// 2012-02-28 by mspark, Ʃ�丮�� ���� ��(Ʃ�丮���� �����Ͻðڽ��ϱ�?) ȭ�鿡�� ��� ��Ӱ� ó��
				if(m_pImgFadeBG)
				{
				m_pImgFadeBG->Move(0,0);
				m_pImgFadeBG->SetScale((INT)g_pD3dApp->GetBackBufferDesc().Width,(INT)g_pD3dApp->GetBackBufferDesc().Height);	
				m_pImgFadeBG->Render();
				}
				// end 2012-02-28 by mspark, Ʃ�丮�� ���� ��(Ʃ�丮���� �����Ͻðڽ��ϱ�?) ȭ�鿡�� ��� ��Ӱ� ó��

				m_pTutorialSelectImage->Move(nTutorialSelectPos_X,nTutorialSelectPos_Y);
				m_pTutorialSelectImage->Render();

				m_pStartButton[m_nStartButtonState]->Move(nStartButtonPos_X,nStartButtonPos_Y);
				m_pStartButton[m_nStartButtonState]->Render();
				m_pSkipButton[m_nSkipButtonState]->Move(nSkipButtonPos_X,nSkipButtonPos_Y);
				m_pSkipButton[m_nSkipButtonState]->Render();
			}
			// 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
			if(m_pINFSecuSelectMenu)
			{
				m_pINFSecuSelectMenu->Render();
			}
			// end 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
			if(m_pINFSelectOption)
			{
				m_pINFSelectOption->Render();
			}
		}
		break;
	case _CREATE:
		{
			// 2010-06-07 by dgwoo ĳ���ͼ���â ���ҽ� ���濡 ���� ����Ʈ ���� ����.
// 			m_pSelectBack->Render();
// 			g_pD3dDev->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
// 			m_pSelectBack->RenderCreateUnit();
// 			g_pD3dDev->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
			//end 2010-06-07 by dgwoo ĳ���ͼ���â ���ҽ� ���濡 ���� ����Ʈ ���� ����.

			// 2007-07-04 by bhsohn ���۷����� �߰�
			m_pUnitCreateInfo->Render( m_pSelectBack->GetCurrentSelectIndex() );
			RenderNPC(_CREATE);			
			RenderGearStat();
			
			//add by jsy
			
			m_pCreateMenu->Render();
			m_pInfWindow->RenderMessaegBox();
		}
		break;
	default:
		{
		}
	}
	m_pNormalMouse[m_nMouseState]->Move(m_ptMouse.x,m_ptMouse.y);
	m_pNormalMouse[m_nMouseState]->Render();

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFSelect::TickGearStat()
/// \brief		Ʃ�丮�� ��ŵ
/// \author		// 2007-07-04 by bhsohn ���۷����� �߰�
/// \date		2007-07-04 ~ 2007-07-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSelect::TickGearStat( int nUnitKind )
{
	int uUnitKind = UNITKIND_BT01 << (4*nUnitKind) ;
	int nSelGear = -1;
	switch(uUnitKind)
	{
	case UNITKIND_BT01:
		{
			nSelGear = UNIT_STAT_B;			
		}
		break;
	case UNITKIND_OT01:
		{
			nSelGear = UNIT_STAT_M;			
		}
		break;
	case UNITKIND_DT01:
		{
			nSelGear = UNIT_STAT_A;			
		}
		break;
	case UNITKIND_ST01:
		{
			nSelGear = UNIT_STAT_I;			
		}
		break;		
	default:
		{
			return;
		}
		break;
	}	
	if(m_nNarrationGear != nSelGear)
	{
		m_nNarrationGear = nSelGear;
		PlayCreateNarration(m_nNarrationGear);
	}
	if(m_nShowGear == nSelGear)
	{
		m_nUnitShowStep = UNIT_SHOW_STEP;
		return;
	}
	
	// �˾����Ͻ��� ��� 
	m_nUnitPopupGear = nSelGear;

	// ��ü�� �ٲ�� 
	if(UNIT_STAT_SHOW == m_nUnitStatMode)
	{
		m_fUnitHideTime = UNIT_INFO_HIDE_TIME;
		m_nUnitStatMode = UNIT_STAT_HIDING;
	}
	switch(m_nUnitStatMode)
	{
	case UNIT_STAT_HIDING:
		{
			m_fUnitHideTime -= g_pD3dApp->GetCheckElapsedTime();
			if(m_fUnitHideTime < 0)
			{
				m_fUnitHideTime = UNIT_INFO_HIDE_TIME;				
				m_nUnitShowStep--;
				if(m_nUnitShowStep <= 0)
				{
					m_nUnitShowStep  = 0;
					m_nUnitStatMode = UNIT_STAT_POPUP;
				}
			}			
		}
		break;
	case UNIT_STAT_POPUP:
		{
			m_fUnitHideTime -= g_pD3dApp->GetCheckElapsedTime();			
			if(m_fUnitHideTime < 0)
			{
				m_fUnitHideTime = UNIT_INFO_HIDE_TIME;				
				m_nUnitShowStep++;
				if(m_nUnitShowStep >= UNIT_SHOW_STEP)
				{
					m_nUnitShowStep = UNIT_SHOW_STEP;
					m_nUnitStatMode = UNIT_STAT_SHOW;					

					// ��� ����
					m_nShowGear = nSelGear;					
				}				
			}
		}
		break;
	case UNIT_STAT_SHOW:
		{

		}
		break;
	}		
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFSelect::PlayCreateNarration(int nShowGear)
/// \brief		�����̼� �÷��� 
/// \author		// 2007-07-24 by bhsohn �����̼� mp3�߰�
/// \date		2007-07-24 ~ 2007-07-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSelect::PlayCreateNarration(int nShowGear)
{
	char chFileName[64];	
	switch(nShowGear)
	{
	case UNIT_STAT_B:
		{
			wsprintf(chFileName, NARRATION_CREATE_B);
		}
		break;
	case UNIT_STAT_M:
		{
			wsprintf(chFileName, NARRATION_CREATE_M);
		}
		break;
	case UNIT_STAT_A:
		{
			wsprintf(chFileName, NARRATION_CREATE_A);
		}
		break;
	case UNIT_STAT_I:
		{
			wsprintf(chFileName, NARRATION_CREATE_I);
		}
		break;		
	default:
		{
			return;
		}
		break;
	}	
	// �����̼� �÷���
	g_pD3dApp->StartNarrationSound(chFileName);

}
void CINFSelect::TickNPCColor(DWORD dwGameState)
{	
	if(_SELECT == dwGameState )
	{
		m_dwNPCAlpha = 0xffffffff;		
		return;
	}
	if(FALSE == m_bNPCRenderFlag )
	{
		return;
	}
	if(m_fFirstNPCViewTime > 0)
	{
		m_fFirstNPCViewTime -= g_pD3dApp->GetCheckElapsedTime();	
		return ;
	}
	
	m_fNPCViewTime -= g_pD3dApp->GetCheckElapsedTime();	
	if(m_fNPCViewTime < 0)
	{			
		m_fNPCAlpahTotalTime = NPC_ALPHA_TIME;
		m_fNPCViewTime = NPC_ALPHA_TIME;
		m_dwNPCAlpha = 0xffffffff;		
		m_bNPCRenderFlag = FALSE;
		return;
	}
	// ���̸� �������..
	m_dwNPCAlpha = g_pGameMain->GetCurrentColor(m_fNPCViewTime, 
													ALPHA_FADE_OUT, 
													m_fNPCAlpahTotalTime,
													NPC_ALPHA_DELAY);	
}

void CINFSelect::InitNPCAlphaColor()
{
	m_fNPCViewTime = NPC_ALPHA_TIME;
	m_dwNPCAlpha = 0;		
	m_bNPCRenderFlag = TRUE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFSelect::RenderGearStat()
/// \brief		Ʃ�丮�� ��ŵ
/// \author		// 2007-07-04 by bhsohn ���۷����� �߰�
/// \date		2007-07-04 ~ 2007-07-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSelect::RenderGearStat()
{
	int nSelGear = m_nShowGear;
	if(UNIT_STAT_POPUP == m_nUnitStatMode)
	{
		// �˾����̰ų� ���̵� ���̴�.
		nSelGear = m_nUnitPopupGear;
	}

	if(nSelGear < 0)
	{
		return;
	}	
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
 		// 2011. 10. 10 by jskim UI�ý��� ����
	//m_nUnitShowStep
	POINT ptImageSize = m_pUnitStatImage[nSelGear]->GetImgSize();
	RECT rcShowRect;
	rcShowRect.left =  ptImageSize.x - ((ptImageSize.x/UNIT_SHOW_STEP) * m_nUnitShowStep);
	if(rcShowRect.left < 0)
	{
		rcShowRect.left = 0;
	}
	rcShowRect.top	= 0;
	rcShowRect.right	= ptImageSize.x;
	rcShowRect.bottom	= ptImageSize.y;
	

	m_pUnitStatImage[nSelGear]->Move(UNIT_STATBK_POS_X, UNIT_STATBK_POS_Y);
	m_pUnitStatImage[nSelGear]->SetRect(rcShowRect.left, rcShowRect.top, rcShowRect.right, rcShowRect.bottom);
	m_pUnitStatImage[nSelGear]->Render();
	// end 2011. 10. 10 by jskim UI�ý��� ����
#endif
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pUnitCreateInfo->SetSelGear(nSelGear);
#endif
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFSelect::RenderNPC()
/// \brief		NPC�׸���
/// \author		// 2007-07-04 by bhsohn ���۷����� �߰�
/// \date		2007-07-04 ~ 2007-07-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSelect::RenderNPC(DWORD dwGameState)
{	
	// 2007-07-23 by bhsohn ���۷����� �߰��۾�
	if(_SELECT == dwGameState)
	{		
		RenderBkBalon(SEL_NPC_BUBBLE_POS_X, SEL_NPC_BUBBLE_POS_Y, 
						SEL_NPC_BUBBLE_WIDTH,SEL_NPC_BUBBLE_HEIGHT,
						SEL_NPC_BUBBLE_ARROW_X);
		RenderString();
		
	}
	// end 2007-07-23 by bhsohn ���۷����� �߰��۾�	
	
	m_pOpNPCImage->SetColor(m_dwNPCAlpha);
	m_pOpNPCImage->Move(SEL_NPC_POS_X,SEL_NPC_POS_Y);
	m_pOpNPCImage->Render();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFSelect::RenderNPC()
/// \brief		NPC�׸���
/// \author		// 2007-07-04 by bhsohn ���۷����� �߰�
/// \date		2007-07-04 ~ 2007-07-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSelect::RenderBkBalon(int x, int y, int cx, int cy, int nBubblePosX)
{	
	// ��
	{
		m_pBalonChat[0]->Move(x,y);
		m_pBalonChat[0]->Render();
		m_pBalonChat[1]->Move(x+ID_HELP_BALON_TLW,y);
		m_pBalonChat[1]->SetScale(cx-(ID_HELP_BALON_TLW), 1);
		m_pBalonChat[1]->Render();
		m_pBalonChat[2]->Move(x+cx - ID_HELP_BALON_TLW+ID_HELP_BALON_TRW+(ID_HELP_BALON_TMW*2),y);
		m_pBalonChat[2]->Render();
	}
	
	// ��
	{
		m_pBalonChat[3]->Move(x, y+ID_HELP_BALON_TLH);
		m_pBalonChat[3]->SetScale(1, cy - (ID_HELP_BALON_TLH + ID_HELP_BALON_BLH));
		m_pBalonChat[3]->Render();
		m_pBalonChat[4]->Move(x+ID_HELP_BALON_MLW, y+ID_HELP_BALON_MRW);
		m_pBalonChat[4]->SetScale(cx-(ID_HELP_BALON_TLW), cy - (ID_HELP_BALON_TLH+ID_HELP_BALON_BLH));
		m_pBalonChat[4]->Render();
		m_pBalonChat[5]->Move(x+cx - ID_HELP_BALON_MLW+ID_HELP_BALON_MRW+(ID_HELP_BALON_MMW*2), y+ID_HELP_BALON_TRH);
		m_pBalonChat[5]->SetScale(1, cy - (ID_HELP_BALON_TRH + ID_HELP_BALON_BRH));
		m_pBalonChat[5]->Render();
	}
	

	// ��
	{
		m_pBalonChat[6]->Move(x, y+cy-ID_HELP_BALON_BLH);
		m_pBalonChat[6]->Render();
		
		m_pBalonChat[7]->Move(x + ID_HELP_BALON_BLW, y+cy-ID_HELP_BALON_BLH);
		m_pBalonChat[7]->SetScale(nBubblePosX, 1);
		m_pBalonChat[7]->Render();
		// ǳ�� 
		m_pBalonChat[9]->Move(x + nBubblePosX + ID_HELP_BALON_BLW, y+cy-ID_HELP_BALON_BLH);
		m_pBalonChat[9]->Render();
		
		POINT ptBubbleSize = m_pBalonChat[9]->GetImgSize();	//ǳ�� ũ��		
		m_pBalonChat[7]->Move(x + nBubblePosX + ptBubbleSize.x + ID_HELP_BALON_BLW, y+cy-ID_HELP_BALON_BLH);
		m_pBalonChat[7]->SetScale(cx- (nBubblePosX+ptBubbleSize.x)- ID_HELP_BALON_BLW, 1);
		m_pBalonChat[7]->Render();
		
		m_pBalonChat[8]->Move(x+cx-ID_HELP_BALON_BLW+ID_HELP_BALON_BRW+(ID_HELP_BALON_BMW*2), y+cy-ID_HELP_BALON_BLH);
		m_pBalonChat[8]->Render();
	}	
}

void CINFSelect::RenderString()
{	
	float fPosX = SEL_NPC_BUBBLE_POS_X +ID_HELP_BALON_TLW;
	float fPosY = SEL_NPC_BUBBLE_POS_Y +ID_HELP_BALON_TLW;
	vector<string>::iterator it = m_vecText.begin();
	while(it != m_vecText.end())
	{		
		char* pTxt = (char*)(*it).c_str();
		if(pTxt)
		{
			m_pFontHelpInfo->DrawText(fPosX,
				fPosY,
				GUI_FONT_COLOR_W,
				pTxt);			
		}
		fPosY += STRING_CAP_ONE_STR_HEIGHT;
		it++;
	}

	// 2007-07-24 by bhsohn �����̼� mp3�߰�
	// ó�� ��Ʈ���� ������ �����Ѵ�. 
	if(FALSE == m_bFirstOpNPCRender)
	{
		m_bFirstOpNPCRender = TRUE;
		// �����̼� �÷���
		char chFileName[64];
		memset(chFileName, 0x00, 64);
		wsprintf(chFileName, NARRATION_SELECT_NPC);
		g_pD3dApp->StartNarrationSound(chFileName);

	}	
}


int CINFSelect::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFSelect::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			m_ptMouse.x = LOWORD(lParam);
			m_ptMouse.y = HIWORD(lParam);
			CheckMouseReverse(&m_ptMouse);
		}
		break;
	}
	// 2009. 10. 14 by jskim ������ī ���� 
	if(m_binfluenceSelect)
	{
		const int nInfluenceSelectPos_X = g_pD3dApp->GetBackBufferDesc().Width/2 - INFLUENCE_SELECT_SIZE_X/2;
		const int nInfluenceSelectPos_Y = g_pD3dApp->GetBackBufferDesc().Height/3 - INFLUENCE_SELECT_SIZE_Y/2;
		const int nBCUButtonPos_X = nInfluenceSelectPos_X + INFLUENCE_SELECT_SIZE_X*2/5 - INFLUENCE_BUTTON_SIZE_X;
		const int nBCUButtonPos_Y = nInfluenceSelectPos_Y+ INFLUENCE_BUTTON_SIZE_Y * 4;
		const int nANIButtonPos_X = nInfluenceSelectPos_X + INFLUENCE_SELECT_SIZE_X*3/5;
		const int nANIButtonPos_Y = nInfluenceSelectPos_Y+ INFLUENCE_BUTTON_SIZE_Y * 4;

		const int nCloseButtonPos_X = nInfluenceSelectPos_X + INFLUENCE_SELECT_SIZE_X - 15;
		const int nCloseButtonPos_Y = nInfluenceSelectPos_Y + 5;
		
		switch(uMsg)
		{
		case WM_MOUSEMOVE:
			
			m_nANIButtonState = 0;
			m_nBCUButtonState = 0;
			
			if(	m_ptMouse.x > nANIButtonPos_X &&
				m_ptMouse.x < nANIButtonPos_X + INFLUENCE_BUTTON_SIZE_X &&
				m_ptMouse.y > nANIButtonPos_Y &&
				m_ptMouse.y < nANIButtonPos_Y + INFLUENCE_BUTTON_SIZE_Y )
			{
				if(m_bANIClick)
				{
					m_nANIButtonState = 2;
				}
				else
				{
					m_nANIButtonState = 1;
				}
			}
			else if(m_ptMouse.x > nBCUButtonPos_X &&
				m_ptMouse.x < nBCUButtonPos_X + INFLUENCE_BUTTON_SIZE_X &&
				m_ptMouse.y > nBCUButtonPos_Y &&
				m_ptMouse.y < nBCUButtonPos_Y + INFLUENCE_BUTTON_SIZE_Y )
			{
				if(m_bBCUClick)
				{
					m_nBCUButtonState = 2;
				}
				else 
				{
					m_nBCUButtonState = 1;
				}
			}
			else
			{
				m_nBCUButtonState = 0;
				m_nANIButtonState = 0;
			}

			break;
		case WM_LBUTTONDOWN:
			if(	m_ptMouse.x > nANIButtonPos_X &&
				m_ptMouse.x < nANIButtonPos_X + INFLUENCE_BUTTON_SIZE_X &&
				m_ptMouse.y > nANIButtonPos_Y &&
				m_ptMouse.y < nANIButtonPos_Y + INFLUENCE_BUTTON_SIZE_Y )
			{
				m_nANIButtonState = 2;
				m_bANIClick = TRUE;
			}
			
			if(	m_ptMouse.x > nBCUButtonPos_X &&
				m_ptMouse.x < nBCUButtonPos_X + INFLUENCE_BUTTON_SIZE_X &&
				m_ptMouse.y > nBCUButtonPos_Y &&
				m_ptMouse.y < nBCUButtonPos_Y + INFLUENCE_BUTTON_SIZE_Y )
			{
				m_nBCUButtonState = 2;
				m_bBCUClick = TRUE;
			}
			if(m_ptMouse.x > nCloseButtonPos_X &&
				m_ptMouse.x < nCloseButtonPos_X + 10 &&
				m_ptMouse.y > nCloseButtonPos_Y &&
				m_ptMouse.y < nCloseButtonPos_Y + 10 )
			{
				m_nStartButtonState = 0;
				m_nSkipButtonState = 0;
				m_nANIButtonState = 0;
				m_nBCUButtonState = 0;	
				m_binfluenceSelect = FALSE;	
				m_bTutorialMenu = FALSE;
			}
			
			break;
		case WM_LBUTTONUP:
			{
				if(	m_ptMouse.x > nBCUButtonPos_X &&
					m_ptMouse.x < nBCUButtonPos_X + INFLUENCE_BUTTON_SIZE_X &&
					m_ptMouse.y > nBCUButtonPos_Y &&
					m_ptMouse.y < nBCUButtonPos_Y + INFLUENCE_BUTTON_SIZE_Y &&
					m_bBCUClick)
				{
					m_nResult = VCN_CITY_MAP_INDEX;
				}
				else if(	m_ptMouse.x > nANIButtonPos_X &&
					m_ptMouse.x < nANIButtonPos_X + INFLUENCE_BUTTON_SIZE_X &&
					m_ptMouse.y > nANIButtonPos_Y &&
					m_ptMouse.y < nANIButtonPos_Y + INFLUENCE_BUTTON_SIZE_Y &&
					m_bANIClick)
				{
					m_nResult = ANI_CITY_MAP_INDEX;
				}
				else 
				{
					m_nANIButtonState = 0;
					m_nBCUButtonState = 0;
					m_bANIClick =FALSE;
					m_bBCUClick =FALSE;
					break;
				}

				if(m_bANIClick || m_bBCUClick)
				{
					m_nStartButtonState = 0;
			 		m_nSkipButtonState = 0;
					m_nANIButtonState = 0;
					m_nBCUButtonState = 0;
					m_binfluenceSelect = FALSE;					
					CHARACTER c = m_character[GetCurrentSelectIndex()];
					MSG_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX sMsg;
					memset(&sMsg,0x00,sizeof(sMsg));
					sMsg.CharacUID = c.CharacterUniqueNumber;
					sMsg.nStartCityMapIdx = m_nResult;
					g_pFieldWinSocket->SendMsg( T_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX, (char*)&sMsg, sizeof(sMsg) );
					if(c.TotalPlayTime < 1)
					{
						m_bTutorialMenu = TRUE;
					}
				}
			}
			break; 
		}
		
	return INF_MSGPROC_NORMAL;
	}
	//end 2009. 10. 14 by jskim ������ī ����
	if(m_bTutorialMenu)	 
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM		
		const int nTutorialSelectPos_X = g_pD3dApp->GetBackBufferDesc().Width/2 - TUTORIAL_SELECT_SIZE_X/2;
		const int nTutorialSelectPos_Y = g_pD3dApp->GetBackBufferDesc().Height/3 - TUTORIAL_SELECT_SIZE_Y/2;
		const int nStartButtonPos_X = nTutorialSelectPos_X + TUTORIAL_SELECT_SIZE_X/2 - TUTORIAL_BUTTON_SIZE_X;
		const int nStartButtonPos_Y = nTutorialSelectPos_Y + TUTORIAL_SELECT_SIZE_Y;
		const int nSkipButtonPos_X = nTutorialSelectPos_X + TUTORIAL_SELECT_SIZE_X/2 + 15;
		const int nSkipButtonPos_Y = nTutorialSelectPos_Y + TUTORIAL_SELECT_SIZE_Y;
#else
		const int nTutorialSelectPos_X = g_pD3dApp->GetBackBufferDesc().Width/2 - TUTORIAL_SELECT_SIZE_X/2;
		const int nTutorialSelectPos_Y = g_pD3dApp->GetBackBufferDesc().Height/3 - TUTORIAL_SELECT_SIZE_Y/2;
		const int nStartButtonPos_X = nTutorialSelectPos_X + TUTORIAL_SELECT_SIZE_X*2/5 - TUTORIAL_BUTTON_SIZE_X/2;
		const int nStartButtonPos_Y = nTutorialSelectPos_Y + TUTORIAL_SELECT_SIZE_Y;
		const int nSkipButtonPos_X = nTutorialSelectPos_X + TUTORIAL_SELECT_SIZE_X*3/5 - TUTORIAL_BUTTON_SIZE_X/2;
		const int nSkipButtonPos_Y = nTutorialSelectPos_Y + TUTORIAL_SELECT_SIZE_Y;
#endif
		
		switch(uMsg)
		{
		case WM_MOUSEMOVE:

			m_nStartButtonState = 0;
			m_nSkipButtonState = 0;
			
			if(	m_ptMouse.x > nStartButtonPos_X &&
				m_ptMouse.x < nStartButtonPos_X + TUTORIAL_BUTTON_SIZE_X &&
				m_ptMouse.y > nStartButtonPos_Y &&
				m_ptMouse.y < nStartButtonPos_Y + TUTORIAL_BUTTON_SIZE_Y )
			{
				if(m_bStartButtonClick)
				{
					m_nStartButtonState = 2;
				}
				else
				{
					m_nStartButtonState = 1;
				}
			}
			else if(m_ptMouse.x > nSkipButtonPos_X &&
					m_ptMouse.x < nSkipButtonPos_X + TUTORIAL_BUTTON_SIZE_X &&
					m_ptMouse.y > nSkipButtonPos_Y &&
					m_ptMouse.y < nSkipButtonPos_Y + TUTORIAL_BUTTON_SIZE_Y )
			{
				if(m_bSkipButtonClick)
				{
					m_nSkipButtonState = 2;
				}
				else
				{
					m_nSkipButtonState = 1;
				}
			}

			break;
		case WM_LBUTTONDOWN:
			if(	m_ptMouse.x > nStartButtonPos_X &&
				m_ptMouse.x < nStartButtonPos_X + TUTORIAL_BUTTON_SIZE_X &&
				m_ptMouse.y > nStartButtonPos_Y &&
				m_ptMouse.y < nStartButtonPos_Y + TUTORIAL_BUTTON_SIZE_Y )
			{
				m_nStartButtonState = 2;
				m_bStartButtonClick = TRUE;
			}

			if(	m_ptMouse.x > nSkipButtonPos_X &&
				m_ptMouse.x < nSkipButtonPos_X + TUTORIAL_BUTTON_SIZE_X &&
				m_ptMouse.y > nSkipButtonPos_Y &&
				m_ptMouse.y < nSkipButtonPos_Y + TUTORIAL_BUTTON_SIZE_Y )
			{
				m_nSkipButtonState = 2;
				m_bSkipButtonClick = TRUE;
			}
			
			break;
		case WM_LBUTTONUP:
			m_bStartButtonClick = FALSE;
			m_bSkipButtonClick = FALSE;
			
			if(m_nStartButtonState == 2)
			{				
				// ������ ����
				g_pD3dApp->m_pSound->PlayD3DSound(75, g_pShuttleChild->m_vPos, FALSE);
				CHARACTER c = m_character[GetCurrentSelectIndex()];
				memcpy(&g_pD3dApp->m_pShuttleChild->m_myShuttleInfo,&c,sizeof(CHARACTER));
				
				// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
				g_pD3dApp->m_pIMSocket->SetChaterInfo(&c);
				g_pD3dApp->SetMFSMyShuttleInfo(&c);
				//memcpy(&g_pD3dApp->m_pIMSocket->m_character,&c,sizeof(CHARACTER));
				
				// 2009. 10. 14 by jskim ������ī ���� - ó�� Ʃ�丮�� ����� ���ѷα� �ȳ����� ���� ó��
				// 2007-07-25 by bhsohn ������ Ʃ�丮��ʿ� �־ ������ ������, ù��? ������ ����
				CHARACTER chSelCharater = m_character[GetCurrentSelectIndex()];
				if((IS_TUTORIAL_MAP_INDEX(chSelCharater.MapChannelIndex.MapIndex))	// Ʃ�丮�� ���̸�
					&& (1 == chSelCharater.Level)		// ������ 1�̸� ó�� ������ ������
					&& (0 == chSelCharater.Experience))	// ����ġ�� 0�̰�		
				{
					g_pTutorial->SetFirstUserIntoFreeSka(TRUE);
				}
				//end 2009. 10. 14 by jskim ������ī ����
				

				strcpy(((CChatMoveData *)g_pD3dApp->m_pShuttleChild->m_pIDChat->m_pChild)->m_szString,c.CharacterName);
				g_pInterface->InitMapLoadObjects();
				g_pInterface->RestoreMapLoadObjects();
				// 2007-12-26 by bhsohn ���� ���ۿ� ���� �κ� ����
				g_pD3dApp->ChangeGameState(_MAPLOAD);			
				{
					// IM Server  ����õ�
					g_pD3dApp->ConnectLoginIMServer(g_pD3dApp->m_strUserID,
						((CShuttleChild *)(g_pD3dApp->m_pShuttleChild))->m_myShuttleInfo.CharacterName,
						g_pD3dApp->m_strUserPassword);											
				}
				// end 2007-12-26 by bhsohn ���� ���ۿ� ���� �κ� ����
			}

			if(m_nSkipButtonState == 2)
			{
				g_pD3dApp->m_pSound->PlayD3DSound(75, g_pShuttleChild->m_vPos, FALSE);
				
				CHARACTER c = m_character[GetCurrentSelectIndex()];
				memcpy(&g_pD3dApp->m_pShuttleChild->m_myShuttleInfo,&c,sizeof(CHARACTER));
				
				// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
				g_pD3dApp->m_pIMSocket->SetChaterInfo(&c);
				g_pD3dApp->SetMFSMyShuttleInfo(&c);
				//memcpy(&g_pD3dApp->m_pIMSocket->m_character,&c,sizeof(CHARACTER));

				strcpy(((CChatMoveData *)g_pD3dApp->m_pShuttleChild->m_pIDChat->m_pChild)->m_szString,c.CharacterName);

				// 2006-09-07 by ispark, ���ѷα�
				g_pInterface->InitSoundScript("Prolog.tex", NULL, TUTORIAL_SKIP, TRUE);
			}
			break;
		}
		
		return INF_MSGPROC_NORMAL;
	}


	if(m_pInfWindow && m_pInfWindow->MsgBoxWndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
		return INF_MSGPROC_BREAK;
	if(m_pINFSelectOption && !m_pINFSelectOption->IsShow() &&
		m_pSelectBack && m_pSelectBack->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
		return INF_MSGPROC_BREAK;
	

	
	switch(g_pD3dApp->m_dwGameState)
	{
	case _SELECT:
		{
			// 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
			if(m_pINFSecuSelectMenu && m_pINFSecuSelectMenu->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;
			// end 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
			if(m_pINFSelectOption && m_pINFSelectOption->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;
			if(m_pINFSelectOption && !m_pINFSelectOption->IsShow() &&
				m_pSelectMenu && m_pSelectMenu->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;
		}
		break;
	case _CREATE:
		{
			if(m_pINFSelectOption && m_pINFSelectOption->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;
			if(m_pCreateMenu && m_pCreateMenu->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;
		}
		break;
	default:
		{
		}
	}
	return INF_MSGPROC_NORMAL;
}

 void CINFSelect::SetUnitRenderInfo(int index, FC_CONNECT_LOGIN_INFO info)
{
	FLOG( "CINFSelect::SetUnitRenderInfo(int index, FC_CONNECT_LOGIN_INFO info)" );
	// 2007-09-28 by dgwoo index�� ������ ���� ��츦 ����Ͽ�.(�߱�)
	if(index < 0)
	{
		g_pD3dApp->NetworkErrorMsgBox(ERRORCODE_000001001);
	}
	m_guiUnitInfo[index] = info;
	// 2005-11-24 by ispark


	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

	//ITEM* pITEM = g_pDatabase->GetServerItemInfo(m_guiUnitInfo[index].CharacterRenderInfo.RI_Center);
	int nItemNum = 0;
	if( m_guiUnitInfo[index].CharacterRenderInfo.RI_Center_ShapeItemNum )
		nItemNum = m_guiUnitInfo[index].CharacterRenderInfo.RI_Center_ShapeItemNum;
	else
		nItemNum = m_guiUnitInfo[index].CharacterRenderInfo.RI_Center;

	ITEM* pITEM	= g_pDatabase->GetServerItemInfo( nItemNum );

	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����


	if(pITEM)
	{
#ifdef _REWORKED_COLORSHOP
		if (pITEM->Kind == 60)
		{
			m_pSelectBack->m_SelectRenderInfo[index].rendertype =
				GetUnitNum(2, pITEM->ItemNum, m_guiUnitInfo[index].UnitKind, FALSE);
		}
		else
		{
#endif
			m_pSelectBack->m_SelectRenderInfo[index].rendertype =
				GetUnitNum(2, pITEM->SourceIndex, m_guiUnitInfo[index].UnitKind, FALSE);
#ifdef _REWORKED_COLORSHOP
		}
#endif
		//m_pSelectBack->m_SelectRenderInfo[index].rendertype = m_guiUnitInfo[index].UnitKind;
	}
	else
	{
		// ������ ����Ʈ
		m_pSelectBack->m_SelectRenderInfo[index].rendertype = 
		GetUnitNum( 2, 0, m_guiUnitInfo[index].UnitKind, FALSE);
		//m_pSelectBack->m_SelectRenderInfo[index].rendertype = m_guiUnitInfo[index].UnitKind;
	}
	
}

BOOL CINFSelect::SetUnitDetailInfo(CHARACTER info)
{
	FLOG( "CINFSelect::SetUnitDetailInfo(CHARACTER info)" );
	// 2008-08-11 by bhsohn �ܵ����� ���� �ǰԲ�
	g_pD3dApp->SetUnitDetailInfo(&info);
	// end 2008-08-11 by bhsohn �ܵ����� ���� �ǰԲ�

	g_pD3dApp->m_bRequestEnable = TRUE;
	g_pD3dApp->m_fRequestEnableTime = REQUEST_ENABLE_INIT_TIME;

	int i;
	for(i=0; i<3; i++)
	{
		if(m_guiUnitInfo[i].CharacterUniqueNumber == info.CharacterUniqueNumber)
			break;
	}
	if(i == 3) 
	{
		DBGOUT("ERROR:CINFSelect::SetUnitDetailInfo(CHARACTER info), reason : index overflowed\n");
		return FALSE;
	}
	
	int nComputerItemNum = m_guiUnitInfo[i].CharacterRenderInfo.RI_ProwIn;
	ITEM *pComputerItem = g_pDatabase->GetServerItemInfo(nComputerItemNum);
	if(pComputerItem)
	{
		GEAR_STAT ComputerStat;
		memset(&ComputerStat,0x00,sizeof(GEAR_STAT));
		// 2009-04-21 by bhsohn ������ DesParam�߰�
// 		SetGearStatByComputerItem(ComputerStat,pComputerItem->DestParameter1,pComputerItem->ParameterValue1);
// 		SetGearStatByComputerItem(ComputerStat,pComputerItem->DestParameter2,pComputerItem->ParameterValue2);
// 		SetGearStatByComputerItem(ComputerStat,pComputerItem->DestParameter3,pComputerItem->ParameterValue3);
// 		SetGearStatByComputerItem(ComputerStat,pComputerItem->DestParameter4,pComputerItem->ParameterValue4);
		int nArrParamCnt = 0;
		for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			SetGearStatByComputerItem(ComputerStat,pComputerItem->ArrDestParameter[nArrParamCnt],
													pComputerItem->ArrParameterValue[nArrParamCnt]);
		}
		// end 2009-04-21 by bhsohn ������ DesParam�߰�

		info.TotalGearStat.AttackPart	+= ComputerStat.AttackPart;
		info.TotalGearStat.DefensePart	+= ComputerStat.DefensePart;
		info.TotalGearStat.DodgePart	+= ComputerStat.DodgePart;
		info.TotalGearStat.FuelPart		+= ComputerStat.FuelPart;
		info.TotalGearStat.ShieldPart	+= ComputerStat.ShieldPart;
		info.TotalGearStat.SoulPart		+= ComputerStat.SoulPart;
	}
	m_character[i] = info;
	wsprintf(m_strAutoStat, "(%s)",CAtumSJ::GetAutoStatTypeString(info.AutoStatType));
	return TRUE;
}

int CINFSelect::GetCharacterNumber()
{
	FLOG( "CINFSelect::GetCharacterNumber()" );
	int count = 0;
	for(int i=0; i<3; i++)
	{
		if(m_guiUnitInfo[i].CharacterUniqueNumber != 0)
			count++;
	}
	return count;
}

int CINFSelect::GetValidIndex()
{
	FLOG( "CINFSelect::GetValidIndex()" );
	for(int i=0; i<3; i++)
	{
		if(m_guiUnitInfo[i].CharacterUniqueNumber != 0)
			return i;
	}
	return -1;
}

int CINFSelect::GetCurrentSelectIndex()
{
	FLOG( "CINFSelect::GetCurrentSelectIndex()" );
	return m_pSelectBack->GetCurrentSelectIndex();
}

int CINFSelect::GetEmptySlot()
{
	FLOG( "CINFSelect::GetEmptySlot()" );
	for(int i=0; i<3; i++)
	{
		if(m_guiUnitInfo[i].CharacterUniqueNumber == 0)
			return i;
	}
	return -1;
}

void CINFSelect::DeleteUnit()
{

	FLOG( "CINFSelect::DeleteUnit()" );
	if(!m_pInfWindow->IsExistMsgBox(_Q_SELECT_DELETE))
	{
		const int nSelectIndex = GetCurrentSelectIndex();
		if(g_pD3dApp->m_pInterface->m_pSelect->m_character[nSelectIndex].CharacterUniqueNumber != 0)
		{
			m_bDelete = TRUE;
			char buf[256];
			wsprintf(buf, STRMSG_C_SELECT_0010);//"�����Ͻ÷��� \\e�����̸�\\e�� �Է����ֽʽÿ�(\\r���������۵� ���� �˴ϴ�\\r)"
			memset(m_strDeleteCharacterName, 0x00, 128);
			strncpy(m_strDeleteCharacterName, m_character[nSelectIndex].CharacterName, 
				strlen(m_character[nSelectIndex].CharacterName));
			m_pInfWindow->AddMsgBox(buf, _Q_SELECT_DELETE, nSelectIndex);
		}
	}
}

void CINFSelect::SetUnitDetailInfoNULL(int nSelect)
{
	FLOG( "CINFSelect::SetUnitDetailInfoNULL(int nSelect)" );
	memset(&m_character[nSelect], 0x00, sizeof(CHARACTER));
}

void CINFSelect::MoveSelect(int nSelect)
{
	FLOG( "CINFSelect::MoveSelect(int nSelect)" );
	m_pSelectBack->MoveSelect(nSelect);
}

HRESULT CINFSelect::SetResourceFile(char* szFileName)
{
	FLOG( "CINFSelect::SetResourceFile(char* szFileName)" );
	util::del(m_pGameData);
	m_pGameData = new CGameData;
	if(!m_pGameData->SetFile(szFileName, FALSE, NULL, 0))
	{
		util::del(m_pGameData);
		return E_FAIL;
	}

	return S_OK;
}

DataHeader * CINFSelect::FindResource(char* szRcName)
{
	FLOG( "CINFSelect::FindResource(char* szRcName)" );
	DataHeader* pHeader = NULL;
	if(m_pGameData)
	{
		pHeader = m_pGameData->Find(szRcName);
	}
	return pHeader;
}

void CINFSelect::ChangeCreateUnit(USHORT uUnitKind)
{
	FLOG( "CINFSelect::ChangeCreateUnit(USHORT uUnitKind)" );
	m_pCreateMenu->CharacterSet(uUnitKind);
}
// 2009. 10. 14 by jskim ������ī ����
//void CINFSelect::GameStart()
void CINFSelect::GameStart(USHORT m_nMapIndex)
//end 2009. 10. 14 by jskim ������ī ����
{
	// 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
	if(TRUE == IsShowSecondPassword())
	{
		return;
	}
	// 2009. 10. 14 by jskim ������ī ����
	if(m_binfluenceSelect == TRUE || m_bTutorialMenu == TRUE)
	{
		return;
	}

	if(m_character[GetCurrentSelectIndex()].InfluenceType == 1 && 
		m_nMapIndex == 0)
	{
		m_binfluenceSelect = TRUE;
		return;
	}
	//end 2009. 10. 14 by jskim ������ī ����
		
	FLOG( "CINFSelect::GameStart()" );

	if(m_character[GetCurrentSelectIndex()].CharacterUniqueNumber != 0)
	{
		CHARACTER c = m_character[GetCurrentSelectIndex()];
		memcpy(&g_pD3dApp->m_pShuttleChild->m_myShuttleInfo,&c,sizeof(CHARACTER));
		
		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
		g_pD3dApp->m_pIMSocket->SetChaterInfo(&c);
		g_pD3dApp->SetMFSMyShuttleInfo(&c);
		
		strcpy(((CChatMoveData *)g_pD3dApp->m_pShuttleChild->m_pIDChat->m_pChild)->m_szString,c.CharacterName);

		g_pInterface->InitMapLoadObjects();
		g_pInterface->RestoreMapLoadObjects();

		g_pD3dApp->ChangeGameState(_MAPLOAD);
		{
			// IM Server  ����õ�
			g_pD3dApp->ConnectLoginIMServer(g_pD3dApp->m_strUserID,
				((CShuttleChild *)(g_pD3dApp->m_pShuttleChild))->m_myShuttleInfo.CharacterName,
				g_pD3dApp->m_strUserPassword);											
		}
		// end 2007-11-20 by bhsohn �ʷε� �ϴ� ��� ����
	}
	// 2007-07-25 by bhsohn ������ Ʃ�丮��ʿ� �־ ������ ������, ù��? ������ ����
	CHARACTER chSelCharater = m_character[GetCurrentSelectIndex()];
	if((IS_TUTORIAL_MAP_INDEX(chSelCharater.MapChannelIndex.MapIndex))	// Ʃ�丮�� ���̸�
		&& (1 == chSelCharater.Level)		// ������ 1�̸� ó�� ������ ������
		&& (0 == chSelCharater.Experience))	// ����ġ�� 0�̰�		
	{
		g_pTutorial->SetFirstUserIntoFreeSka(TRUE);
	}

	// 2008-08-11 by bhsohn �ܵ����� ���� �ǰԲ�
	if(g_pD3dApp->IsSingletonMode())
	{			
		// ���� ��ŸƮ ��������
		{
			MSG_FC_CHARACTER_GAMESTART_OK sMsg;	
			memset(&sMsg, 0x00, sizeof(MSG_FC_CHARACTER_GAMESTART_OK));
			// ĳ���� ���� ����
			char buf[256];
			wsprintf(buf,"gamestartinfo.Inf");
			FILE *	fp;
			fp = fopen(buf,"r+b");
			if(fp)
			{
				fread(&sMsg, 1, sizeof(MSG_FC_CHARACTER_GAMESTART_OK),fp);
				fclose(fp);
			}				
			g_pD3dApp->SingPlayGameStartOk(&sMsg);
		}
		// ������ ����
		{
			MSG_FC_TRADE_INSERT_ITEM sMsg;	
			memset(&sMsg, 0x00, sizeof(MSG_FC_TRADE_INSERT_ITEM));
			// ĳ���� ���� ����
			char buf[256];

			int nCnt = 0;

			for(nCnt = 0;nCnt< 100; nCnt++)	// �ִ� 100�� ���� �κ��� �ֱ� ����
			{
				wsprintf(buf,"iteminfo\\iteminfo[%d].Inf", nCnt);
				FILE *	fp;
				fp = fopen(buf,"r+b");
				if(fp)
				{
					fread(&sMsg, 1, sizeof(MSG_FC_TRADE_INSERT_ITEM),fp);
					fclose(fp);

					g_pD3dApp->FieldSocketTradeInsertItem(&sMsg);					
				}
				
			}
		}		
	}
	// end 2008-08-11 by bhsohn �ܵ����� ���� �ǰԲ�

}

void CINFSelect::GameEnd()
{
	FLOG( "CINFSelect::GameEnd()" );
	memset((void*)m_character, 0x00, sizeof(CHARACTER)*SELECT_MODE);
	if( m_guiUnitInfo[GetCurrentSelectIndex()].CharacterUniqueNumber != 0 )
	{
		MSG_FC_CHARACTER_GET_CHARACTER sMsg;
		memset(&sMsg,0x00,sizeof(sMsg));
		
		sMsg.AccountUniqueNumber = g_pD3dApp->m_accountUniqueNumber;
		sMsg.CharacterUniqueNumber = m_guiUnitInfo[GetCurrentSelectIndex()].CharacterUniqueNumber;
		
		g_pFieldWinSocket->SendMsg( T_FC_CHARACTER_GET_CHARACTER, (char*)&sMsg, sizeof(sMsg) );		

		g_pD3dApp->m_bRequestEnable = FALSE;
	}
}

void CINFSelect::RenderRequestEnable(float fAniTime)
{
	DWORD dwColor = 0x00FFFFFF;
	DWORD dwAlpha;

	if(fAniTime < 0)
	{
		return;
	}
	else if(fAniTime <= REQUEST_ENABLE_TIME/2.0f)
	{
		dwAlpha = fAniTime*2.0f * 255;	
	}
	else
	{
		dwAlpha = (2.0f - fAniTime*2.0f) * 255;	
	}

	dwAlpha = dwAlpha << 24;
	dwColor |= dwAlpha;

	m_pRequestEnableBack->Move((g_pD3dApp->GetBackBufferDesc().Width - REQUEST_ENABLE_BOX_WIDTH)/2,
								(g_pD3dApp->GetBackBufferDesc().Height - REQUEST_ENABLE_BOX_HEIGHT)/2);
	m_pRequestEnableBack->Render();
	m_pRequestEnableTriangle->SetColor(dwColor);
	m_pRequestEnableTriangle->Move((g_pD3dApp->GetBackBufferDesc().Width - REQUEST_ENABLE_BOX_WIDTH)/2,
									(g_pD3dApp->GetBackBufferDesc().Height - REQUEST_ENABLE_BOX_HEIGHT)/2);
	m_pRequestEnableTriangle->Render();
}

void CINFSelect::SetGearStatByComputerItem(GEAR_STAT& sGearStat, int nDestParam, float fDestValue)
{
	switch(nDestParam)
	{
	case DES_ATTACK_PART:				// ���� ����̽� ��Ʈ
		sGearStat.AttackPart += fDestValue;
		break;
	case DES_DEFENSE_PART:				// ���� ����̽� ��Ʈ
		sGearStat.DefensePart += fDestValue;
		break;
	case DES_FUEL_PART:					// ���� ����̽� ��Ʈ
		sGearStat.FuelPart += fDestValue;
		break;
	case DES_SOUL_PART:					// ���� ����̽� ��Ʈ
		sGearStat.SoulPart += fDestValue;
		break;
	case DES_SHIELD_PART:				// ��� ����̽� ��Ʈ
		sGearStat.ShieldPart += fDestValue;
		break;
	case DES_DODGE_PART	:				// ȸ�� ����̽� ��Ʈ
		sGearStat.DodgePart += fDestValue;
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFSelect::CheckForcedTutorial()
/// \brief		���� Ʃ�丮�� üũ
/// \author		ispark
/// \date		2006-07-06 ~ 2006-07-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFSelect::CheckForcedTutorial()
{
	// 2006-09-28 by ispark, üũ ��� ����.
	// ���� �����ϴ� ĳ���� ���� Ʃ�丮�� �ʹ�ȣ��� ���� Ʃ�丮��
	int nCount = 0;
	int newCharacger = 0;

	// ������ ĳ���� ���� Ȯ��
	for(int i = 0; i < SELECT_MODE; i++)
	{
		if(m_guiUnitInfo[i].CharacterUniqueNumber)
		{
			ATUM_DATE_TIME PlayTime;
//			if(m_character[m_pSelectBack->GetCurrentSelectIndex()].LastStartedTime == PlayTime &&
//				IS_TUTORIAL_MAP_INDEX(m_character[m_pSelectBack->GetCurrentSelectIndex()].MapChannelIndex.MapIndex))
			if(IS_TUTORIAL_MAP_INDEX(m_character[i].MapChannelIndex.MapIndex))
			{
				newCharacger++;
			}

			nCount++;
		}
	}

	if(nCount == newCharacger)
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFSelect::SendTutorialSkip()
/// \brief		Ʃ�丮�� ��ŵ
/// \author		ispark
/// \date		2006-09-07 ~ 2006-09-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSelect::SendTutorialSkip()
{
	// 2006-10-13 by ispark, Ʃ�丮�� ��ŵ�� ĳ���� UID ������
	INIT_MSG_WITH_BUFFER(MSG_FC_CHARACTER_TUTORIAL_SKIP, T_FC_CHARACTER_TUTORIAL_SKIP, pTutorialSkip, Sendbuf);
	pTutorialSkip->CharacterUniqueNumber = m_character[GetCurrentSelectIndex()].CharacterUniqueNumber;
	g_pD3dApp->m_pFieldWinSocket->Write(Sendbuf, MSG_SIZE(MSG_FC_CHARACTER_TUTORIAL_SKIP));
//	g_pFieldWinSocket->WriteMessageType(T_FC_CHARACTER_TUTORIAL_SKIP);

	g_pD3dApp->m_bRequestEnable = FALSE;
	m_bTutorialMenu = FALSE;
	g_pTutorial->SetTutorialMode(FALSE);
	g_pTutorial->SetTutorialEd(TRUE);					// 2005-10-17 by ispark
}

// by bhsohn 2007-03-12 China Working
void CINFSelect::ResetMessageString()
{
	if(m_pInfWindow)
	{
		m_pInfWindow->ResetMessageString();	
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFSelect::SendTutorialSkip()
/// \brief		
/// \author		// 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
/// \date		2007-09-12 ~ 2007-09-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSelect::ShowSecondPassword()
{
	if((TRUE == g_pD3dApp->IsUseSecondaryPasswordSystem())
		&& (TRUE == g_pD3dApp->IsSetSecondaryPassword()))
	{
	}
	else
	{
		return;
	}
	
	if(NULL == m_pINFSecuSelectMenu)
	{
		m_pINFSecuSelectMenu = new CINFSecuSelectMenu(this);
		m_pINFSecuSelectMenu->InitDeviceObjects();
		m_pINFSecuSelectMenu->RestoreDeviceObjects();

	}	
	m_pINFSecuSelectMenu->ShowSetPassWnd();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFSelect::IsShowSecondPassword()
/// \brief		
/// \author		// 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
/// \date		2007-09-12 ~ 2007-09-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFSelect::IsShowSecondPassword()
{
	if(NULL == m_pINFSecuSelectMenu)
	{
		return FALSE;
	}
	return m_pINFSecuSelectMenu->IsShowWnd();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
/// \date		2007-09-12 ~ 2007-09-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSelect::ShowSecutiryWnd(BOOL bShowWnd)
{
	if(NULL == m_pINFSecuSelectMenu)
	{
		return ;
	}
	m_pINFSecuSelectMenu->ShowSecutiryWnd(bShowWnd);	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
/// \date		2007-09-12 ~ 2007-09-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char* CINFSelect::GetSelectCharcterName(int nSelectIndex)
{
	return m_character[nSelectIndex].CharacterName;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
/// \date		2007-09-12 ~ 2007-09-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSelect::AddMsgBox(char* strMsg, int nType)
{
	m_pInfWindow->AddMsgBox(strMsg, nType);
}
void CINFSelect::ShowOption(BOOL i_bShow)
{
	m_pINFSelectOption->SetShow(i_bShow);
}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
void CINFSelect::SetBKInfoFadeOut()
{
	m_pUnitCreateInfo->SetFadeMode(FADE_IN);
}

void CINFSelect::SetBKInfoFadeIn()
{
	m_pUnitCreateInfo->SetFadeMode(FADE_OUT);
}
#endif