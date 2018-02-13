// INFCommunityParty.cpp: implementation of the CINFCommunityParty class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFCommunityParty.h"
#include "AtumApplication.h"
#include "EnemyData.h"
#include "GameDataLast.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "SceneData.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "INFGameMain.h"
#include "INFWindow.h"

// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
#include "IMSocketManager.h"
//#include "IMSocket.h"

#include "FieldWinSocket.h"
#include "ClientParty.h"
#include "Chat.h"
#include "dxutil.h"

// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
#include "INFImageBtn.h"
#include "INFCommunity.h"
#include "AtumSound.h"
#include "INFArenaScrollBar.h"
#include "INFCommuPartyCre.h"
#include "INFEditBox.h"
#include "INFSecuEditBox.h"
#include "INFNumEditBox.h"
#include "INFImageRadioBtn.h"
#include "INFListBox.h"

// 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
#include "INFSingleCurselEditBox.h"
// end 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�

// 2009. 08. 11 by ckPark ��� ���� ��ų
#include "AtumDatabase.h"
// end 2009. 08. 11 by ckPark ��� ���� ��ų

// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
#include "INFIcon.h"
// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����

#include "INFImageEx.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"								// 2011. 10. 10 by jskim UI�ý��� ����
#include "INFToolTip.h"			// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
#include "Interface.h"

#define MASTER_SPEAKING	1	// ��Ƽ �����Ͱ� ���ϴ���  
#define PARTYS_SPEAKING	2	// ��Ƽ���� ���ϴ���
#define RENDER_SPEAKING_TIMING	0.5f
// 2008-07-10 by bhsohn Ÿ������ �ȵǴ� ���� �ذ�
//#define FRAME_START_Y		250//85

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
// ���

#include "AresPoint.h"

constexpr APoint COMMU_BK_START { 2, 25 };

constexpr APoint COMMU_BK_START_SIZE { 100, 25 };

constexpr APoint COMMU_BK_BTN_START { COMMU_BK_START + APoint { 40, 10 } };

constexpr auto COMMU_BK_BTN_GAP = 60;

constexpr auto SIZE_MAX_COM_TEAM_PW = 10;

constexpr auto COMMU_BK_SEARCH_WIDTH = 465;	

constexpr APoint CURRENT_PARTY_BTN_START { 355, 316 };	

constexpr APoint CURRENT_PARTY_OPTION_BTN_START { 32, 340 };	

constexpr APoint CURRENT_PARTY_JOIN_BTN { 350, 361 };

constexpr APoint CURRENT_PARTY_REFRESH_BTN { 417, 361 };

constexpr auto MAX_PARTY_LIST_LINE = 8;

constexpr APoint SEL_IMAGE_POS { 26, 84 };
constexpr auto SEL_IMAGE_POS_CAP_Y = 31;

////////////////////// ��� �ɼ�////////////////////// 
// �ʴ�
constexpr APoint COMMUNITY_OP_INVITE { 256, 355 };
// Ż��
constexpr APoint COMMUNITY_OP_LEAVE { 315, 355 };
// �߹�
constexpr APoint COMMUNITY_OP_PURGE { 374, 355 };
// ����
constexpr APoint COMMUNITY_OP_CHARE { 433, 355 };
// Ȯ�� 
constexpr APoint COMMUNITY_OP_OK { 187, 359 };
// ������� 
constexpr APoint COMMUNITY_OP_PARTY_BATTLE { 103, 359 };


// ���� ID
constexpr APoint PARTYOP_LIST_START { 260, 236 };
constexpr APoint PARTYOP_LIST_VOIP_START { 385, 236 };
constexpr auto PARTYOP_LIST_HEIGHT = 16;
// 2008-12-02 by bhsohn ���� �¿������� ����üũ//if its online
constexpr APoint PARTYOP_LIST_ON_START { 448, 236 };

constexpr auto COMMUNITY_OPBK_WIDTH = 230;
constexpr auto COMMUNITY_OPBK_HEIGHT = 623;

// Edit ��Ʈ�� (Pass)
constexpr APoint SECURITY_WND_PASS_EDIT { 68, 168 };
constexpr auto SECURITY_WND_PASS_EDIT_W = 85;
constexpr auto SECURITY_WND_PASS_EDIT_H = 17;

// �ּ� ����Ʈ ��Ʈ��
constexpr APoint MIN_NUM_EDIT { (50), (205) };
constexpr auto MIN_NUM_EDIT_W = (43);
constexpr auto MIN_NUM_EDIT_H = (16);

// �ִ� ����Ʈ ��Ʈ��
constexpr APoint MAX_NUM_EDIT { (115), (205) };
constexpr auto MAX_NUM_EDIT_W = (43);
constexpr auto MAX_NUM_EDIT_H = (16);

// ���� �Ұ�
constexpr APoint PARTY_NAME_EDIT { 96, 69 };
constexpr auto PARTY_NAME_EDIT_W = 150;
constexpr auto PARTY_NAME_FONT_LINE_HEIGHT = 15;

// ���� ��� ����
constexpr APoint PARTYCRE_JOIN { 46, 108 };
constexpr auto PARTYCRE_JOIN_CAP_Y = 18;

// ���� �ɼ� ����
constexpr APoint PARTYCRE_AUDIO { 46, 261 };
constexpr auto PARTYCRE_AUDIO_CAP_Y = 18;

// �޺� �ڽ� 
constexpr auto DIVISION_COMBO_MAIN_WIDTH = 110;
constexpr auto DIVISION_COMBO_MAIN_HEIGHT = 17;
constexpr auto DIVISION_COMBO_ELE_WIDTH = 110;
constexpr auto DIVISION_COMBO_ELE_HEIGHT = 15;
//wsw
#define DIVISION_COMBO_MAIN_X (ptBkPos.x+58)
#define DIVISION_COMBO_MAIN_Y (ptBkPos.y+280)
#define DIVISION_COMBO_ELE_X (ptBkPos.x+58)
#define DIVISION_COMBO_ELE_Y (ptBkPos.y+317)

// �޺��ڽ�
constexpr auto PARTY_FORMAT_COMBO_MAIN_X = 283;
constexpr auto PARTY_FORMAT_COMBO_MAIN_Y = 90;
constexpr auto PARTY_FORMAT_COMBO_MAIN_WIDTH = 110;
constexpr auto PARTY_FORMAT_COMBO_MAIN_HEIGHT = 17;
constexpr auto PARTY_FORMAT_COMBO_ELE_WIDTH = 110;
constexpr auto PARTY_FORMAT_COMBO_ELE_HEIGHT = 15;

// ��� ���� ����
constexpr APoint COMMUNITY_FORMATTXT { 263, 110 };
constexpr auto COMMUNITY_FORMATTXT_WIDTH = 130;
constexpr auto COMMUNITY_FORMATTXT_LINE = 4;
constexpr auto COMMUNITY_FORMATTXT_CAP_Y = 18;
// ��� ����
constexpr APoint COMMUNITY_FORMAT { 393, 120 };
// �ִ� ��Ƽ�� 
constexpr auto MAX_PARTY_LEN = 10;


// �ִ� ��Ƽ�� 
constexpr auto RQ_PARTYLIST_TIME = (900);


#define VOICECHAT_SPK_POS_X (m_ptWindowPos.x+65)
#define VOICECHAT_SPK_POS_Y (m_ptWindowPos.y+324)
#define VOICECHAT_SPK_W 65
constexpr auto VOICECHAT_SPK_CONTROL_TIME_GAP = 0.25f;

constexpr auto EDIT_SIZE_MAX_PARTY_NAME = 20;



// 2009. 08. 11 by ckPark ��� ���� ��ų

// ��� ���� ����
constexpr APoint COMMUNITY_FORMATION_SKILL_DESC { 263, 110 };
constexpr auto COMMUNITY_FORMATION_SKILL_DESC_WIDTH = 118;
constexpr auto CCOMMUNITY_FORMATION_SKILL_DESC_LINE = 4;
constexpr auto CCOMMUNITY_FORMATION_SKILL_DESC_CAP_Y = 18;

// end 2009. 08. 11 by ckPark ��� ���� ��ų



// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â	 
#else
// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
// ���
#define	COMMU_BK_START_X		21
#define	COMMU_BK_START_Y		48
#define	COMMU_BK_START_WIDTH	120
#define	COMMU_BK_START_HEIGHT	23

////////////////////// ��� �˻�////////////////////// 
#define	COMMU_BK_SEARCH_WIDTH	450	// ��� �˻� �ʺ�
// ����
#define	CURRENT_PARTY_CREATE_BTN_X		283
#define	CURRENT_PARTY_CREATE_BTN_Y		361
// ����
#define	CURRENT_PARTY_JOIN_BTN_X		350
#define	CURRENT_PARTY_JOIN_BTN_Y		361
// ���ΰ�ħ
#define	CURRENT_PARTY_REFRESH_BTN_X		417
#define	CURRENT_PARTY_REFRESH_BTN_Y		361

#define MAX_PARTY_LIST_LINE				12

// ����Ʈ �̹���
#define	SEL_IMAGE_POS_X					52
#define	SEL_IMAGE_POS_Y					102
#define	SEL_IMAGE_POS_CAP_Y				21
////////////////////// ��� �ɼ�////////////////////// 
// �ʴ�
#define	COMMUNITY_OP_INVITE_X			256
#define	COMMUNITY_OP_INVITE_Y			355
// Ż��
#define	COMMUNITY_OP_LEAVE_X			315
#define	COMMUNITY_OP_LEAVE_Y			355
// �߹�
#define	COMMUNITY_OP_PURGE_X			374
#define	COMMUNITY_OP_PURGE_Y			355
// ����
#define	COMMUNITY_OP_CHARE_X			433
#define	COMMUNITY_OP_CHARE_Y			355
// Ȯ�� 
#define	COMMUNITY_OP_OK_X				187
#define	COMMUNITY_OP_OK_Y				359
// ������� 
#define	COMMUNITY_OP_PARTY_BATTLE_X		103
#define	COMMUNITY_OP_PARTY_BATTLE_Y		359


// ���� ID
#define	PARTYOP_LIST_START_X			258
#define	PARTYOP_LIST_START_Y			254
#define PARTYOP_LIST_VOIP_START_X		385
#define PARTYOP_LIST_VOIP_START_Y		254
#define PARTYOP_LIST_HEIGHT				16
// 2008-12-02 by bhsohn ���� �¿������� ����üũ
#define	PARTYOP_LIST_ON_START_X			448
#define	PARTYOP_LIST_ON_START_Y			254

#define	COMMUNITY_OPBK_WIDTH			230
#define	COMMUNITY_OPBK_HEIGHT			623

// Edit ��Ʈ�� (Pass)
#define		SECURITY_WND_PASS_EDIT_X		127
#define		SECURITY_WND_PASS_EDIT_Y		164
#define		SECURITY_WND_PASS_EDIT_W		85
#define		SECURITY_WND_PASS_EDIT_H		20

// �ּ� ����Ʈ ��Ʈ��
#define		MIN_NUM_EDIT_X		(127)
#define		MIN_NUM_EDIT_Y		(182)
#define		MIN_NUM_EDIT_W		(35)
#define		MIN_NUM_EDIT_H		(20)

// �ִ� ����Ʈ ��Ʈ��
#define		MAX_NUM_EDIT_X		(187)
#define		MAX_NUM_EDIT_Y		(182)
#define		MAX_NUM_EDIT_W		(35)
#define		MAX_NUM_EDIT_H		(20)

// ���� �Ұ�
#define		PARTY_NAME_EDIT_X						93
#define		PARTY_NAME_EDIT_Y						87
#define		PARTY_NAME_EDIT_W						150
#define		PARTY_NAME_FONT_LINE_HEIGHT			15

// ���� ��� ����
#define	PARTYCRE_JOIN_X				46
#define	PARTYCRE_JOIN_Y				126
#define	PARTYCRE_JOIN_CAP_Y			18

// ���� �ɼ� ����
#define	PARTYCRE_AUDIO_X				46
#define	PARTYCRE_AUDIO_Y				279
#define	PARTYCRE_AUDIO_CAP_Y			18

// �޺� �ڽ� 
#define DIVISION_COMBO_MAIN_WIDTH			110
#define DIVISION_COMBO_MAIN_HEIGHT		17
#define DIVISION_COMBO_ELE_WIDTH			110
#define DIVISION_COMBO_ELE_HEIGHT			13

// �޺��ڽ�
#define	PARTY_FORMAT_COMBO_MAIN_X				258
#define	PARTY_FORMAT_COMBO_MAIN_Y				107
#define PARTY_FORMAT_COMBO_MAIN_WIDTH			110
#define PARTY_FORMAT_COMBO_MAIN_HEIGHT			17
#define PARTY_FORMAT_COMBO_ELE_WIDTH			110
#define PARTY_FORMAT_COMBO_ELE_HEIGHT			13

// ��� ���� ����
#define	COMMUNITY_FORMATTXT_X				23
#define	COMMUNITY_FORMATTXT_Y				355
#define	COMMUNITY_FORMATTXT_WIDTH			130
#define	COMMUNITY_FORMATTXT_LINE			4
#define	COMMUNITY_FORMATTXT_CAP_Y			18
// ��� ����
#define	COMMUNITY_FORMAT_X				391
#define	COMMUNITY_FORMAT_Y				142
// �ִ� ��Ƽ�� 
#define	MAX_PARTY_LEN					6


// �ִ� ��Ƽ�� 
#define	RQ_PARTYLIST_TIME				(900)


#define VOICECHAT_SPK_POS_X			(m_ptWindowPos.x + 67)
#define VOICECHAT_SPK_POS_Y			(m_ptWindowPos.y + 340)
#define VOICECHAT_SPK_W				65
#define VOICECHAT_SPK_CONTROL_TIME_GAP		0.25f

#define EDIT_SIZE_MAX_PARTY_NAME		20



// 2009. 08. 11 by ckPark ��� ���� ��ų

// ��� ���� ����
#define	COMMUNITY_FORMATION_SKILL_DESC_X		265
#define	COMMUNITY_FORMATION_SKILL_DESC_Y		130
#define	COMMUNITY_FORMATION_SKILL_DESC_WIDTH	120
#define	CCOMMUNITY_FORMATION_SKILL_DESC_LINE	4
#define	CCOMMUNITY_FORMATION_SKILL_DESC_CAP_Y	18

// end 2009. 08. 11 by ckPark ��� ���� ��ų



// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCommunityParty::CINFCommunityParty(CAtumNode* pParent)
{
	FLOG( "CINFCommunityParty(CAtumNode* pParent)" );
	m_pParent = pParent;

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pFormat = NULL;
#endif
	m_pForcus = NULL;
	//m_pCombo = NULL;
//	m_pLogo = NULL;

	int i=0;
	for(i=0;i<5;i++)
	{
		m_nPartyButtonState[i] = BUTTON_STATE_NORMAL;
	}
	m_nJoinFormationButtonState = PARTY_JOIN_BUTTON_STATE_NORMAL;

	for(i=0;i<MAX_PARTY_NUMBER;i++)
	{
		m_pPartyList[i] = NULL;
		m_pPartyInfo[i] = NULL;
		m_pFontPartyFrame[i] = NULL;
	}
	// 2009. 12. 03 by jskim ��Ƽ ������ ��Ƽ�� ���ԵǾ� �ִ��� üũ
	//m_nPartyRequestID = 0;
	//end 2009. 12. 03 by jskim ��Ƽ ������ ��Ƽ�� ���ԵǾ� �ִ��� üũ
	memset(m_strInviteUser, 0x00, sizeof(m_strInviteUser));
	m_nPartySelectMember = -1; // non select
	m_nPartyBanCharacterUniqueNumber = 0;
	m_nPartyTransferMasterCharacterUniqueNumber = 0;
	for(i=0;i<PARTY_FORMATION_NUMBER;i++)
	{
		//m_pPartyFormation[i] = NULL;
		memset(m_strPartyFormation[i], 0x00, FORMATION_NAME_LENGTH);
	}
	m_bShowFormationList = FALSE;

	//m_pCaptainCombo = NULL;
	//m_pFormationInfo = NULL;
	m_pPartyFrame = NULL;
	m_pPartyFrameFuel = NULL;
	m_pPartyFrameHP = NULL;
	m_pPartyTalking = NULL;
	m_bRestored = FALSE;
	m_bInvalidated = FALSE;
	m_bEnablePartyMenu = TRUE;
	m_bRenderingSpeak = FALSE;


	m_nVOIPButtonState = PARTY_VOIP_BUTTON_STATE_NORMAL;
	m_fSpeakingTimers = RENDER_SPEAKING_TIMING;
	
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â	
	m_pSCrateBtn = NULL ;			// ����
	m_pSJoinBtn = NULL ;				// ����
	m_pSRefreshBtn= NULL ;			// ���ΰ�ħ		
	m_bPartyTypeBtn = _NOPARTY;		// ��ư�� ��Ƽ Ÿ��

	m_vecPartyListInfo.clear();
	
	m_pScrollPartyList = NULL;
	m_nTotalPartyCnt = 0;
	m_pImgSelect = NULL;
	m_nSelParty = -1;

	m_pLockImage = NULL;
	m_pUnLockImage= NULL;

	m_nSubWndMode = WND_PARTYMODE_OPTION;		// ��Ƽ �ɼ�


	//////////////////////��� �ɼ� //////////////////////
	m_pOJoinBtn = NULL ;
	m_pOLeaveBtn = NULL;
	m_pOPurgeBtn = NULL;
	m_pOChargeBtn= NULL;	
	m_pOOkBtn = NULL;
	m_pOPartyBattle		= NULL;

	m_pOPartyList = NULL;
	m_nPartyOPSel = -1;

	m_pSelImage = NULL;

	m_pINFSecuEditBox = NULL;
	m_pNumMinEditBox= NULL;
	m_pNumMaxEditBox= NULL;
	m_pEditPartyName = NULL;	

	int nCnt = 0;	
	{
		for(nCnt = 0;nCnt < MAX_RADIO_JOINT;nCnt++)
		{
			m_pPartyJoint[nCnt] = NULL;
		}
		
		for(nCnt = 0;nCnt < MAX_RADIO_AUDIO;nCnt++)
		{
			m_pAudioOp[nCnt] = NULL;
		}		
	}
	for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
	{
		m_pComboDivision[nCnt] = NULL;
	}
	for(nCnt = 0;nCnt < MAX_COMBO_FLIGHT_FORM;nCnt++)
	{
		m_pFormatImage[nCnt] = NULL;
	}
	for(nCnt = 0;nCnt < MAX_WND_PARTYMODE;nCnt++)
	{
		m_pImgBk[nCnt] = NULL;	// ���
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		m_pPartyControlBtn[nCnt] = NULL;														  
#endif
	}
	
	m_pComboForMation = NULL;		// �޺� �ڽ�

	m_nAduioIdx = RADIO_AUDIO_KEY;
	memset(&m_pSPartyInfoSetup, 0x00, sizeof(SPARTY_INFO));
	
	m_curRqPartyListTime.Year = 0;
	m_curRqPartyListTime.Month = 0;
	m_curRqPartyListTime.Day = 0;
	m_curRqPartyListTime.Hour = 0;
	m_curRqPartyListTime.Minute = 0;
	m_curRqPartyListTime.Second = 0;	// ��Ƽ ���� ��û�ð�

	m_bClickRefresh = FALSE;
}

CINFCommunityParty::~CINFCommunityParty()
{
	DeletePartyList();

	FLOG( "~CINFCommunityParty()" );

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	util::del(m_pFormat);
#endif
	util::del(m_pForcus);

	util::del( m_pPartyFrame);
	util::del( m_pPartyFrameFuel);	
	util::del( m_pPartyFrameHP);	
	util::del( m_pPartyTalking);

	int i=0;
	for(i=0;i<MAX_PARTY_NUMBER;i++)
	{
		util::del(m_pPartyList[i]);
		util::del(m_pPartyInfo[i]);
		util::del(m_pFontPartyFrame[i]);
	}

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â	
	util::del(m_pSCrateBtn);	// ����
	util::del(m_pSJoinBtn);	// ����
	util::del(m_pSRefreshBtn);	// ����	
	util::del(m_pScrollPartyList);
	util::del(m_pImgSelect);
	util::del(m_pLockImage);
	util::del(m_pUnLockImage);

	util::del(m_pOJoinBtn);
	util::del(m_pOLeaveBtn);
	util::del(m_pOPurgeBtn);
	util::del(m_pOChargeBtn);		
	util::del(m_pOOkBtn);	
	util::del(m_pOPartyBattle);
	util::del(m_pOPartyList);	
	util::del(m_pSelImage);	

	util::del(m_pINFSecuEditBox);
	util::del(m_pNumMinEditBox);
	util::del(m_pNumMaxEditBox);
	util::del(m_pEditPartyName);
	int nCnt = 0;	
	{
		for(nCnt = 0;nCnt < MAX_RADIO_JOINT;nCnt++)
		{		
			util::del(m_pPartyJoint[nCnt]);		
		}
		
		for(nCnt = 0;nCnt < MAX_RADIO_AUDIO;nCnt++)
		{
			util::del(m_pAudioOp[nCnt]);
		}		
	}
	for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
	{
		util::del(m_pComboDivision[nCnt]);
	}
	for(nCnt = 0;nCnt < MAX_COMBO_FLIGHT_FORM;nCnt++)
	{
		util::del(m_pFormatImage[nCnt]);
	}
	for(nCnt = 0;nCnt < MAX_WND_PARTYMODE;nCnt++)
	{
		util::del(m_pImgBk[nCnt]);	// ���
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		m_pPartyControlBtn[nCnt] = NULL;
#endif
	}
	util::del(m_pComboForMation);	
	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â	
}

HRESULT CINFCommunityParty::InitDeviceObjects()
{
	FLOG( "CINFCommunityParty::InitDeviceObjects()" );
	
	int nCnt = 0;

	for(int i=0;i<MAX_PARTY_NUMBER;i++)
	{
		m_pPartyList[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,128,32);
		m_pPartyList[i]->InitDeviceObjects(g_pD3dDev);

		m_pPartyInfo[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,64,32);
		m_pPartyInfo[i]->InitDeviceObjects(g_pD3dDev);

		m_pFontPartyFrame[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,128,32);
		m_pFontPartyFrame[i]->InitDeviceObjects(g_pD3dDev);
	}

	strcpy(m_strPartyFormation[0], FLIGHT_FORM_NONE_NAME);
	strcpy(m_strPartyFormation[1], FLIGHT_FORM_2_COLUMN_NAME);
	strcpy(m_strPartyFormation[2], FLIGHT_FORM_2_LINE_NAME);
	strcpy(m_strPartyFormation[3], FLIGHT_FORM_TRIANGLE_NAME);
	strcpy(m_strPartyFormation[4], FLIGHT_FORM_INVERTED_TRIANGLE_NAME);
	strcpy(m_strPartyFormation[5], FLIGHT_FORM_BELL_NAME);
	strcpy(m_strPartyFormation[6], FLIGHT_FORM_INVERTED_BELL_NAME);
	strcpy(m_strPartyFormation[7], FLIGHT_FORM_X_NAME);
	strcpy(m_strPartyFormation[8], FLIGHT_FORM_STAR_NAME);

	DataHeader* pDataHeader;


#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pFormat = new CINFImageEx;	
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	//pDataHeader = FindResource("Format");
	pDataHeader = FindResource("commubk0");
	m_pFormat->InitDeviceObjects(pDataHeader ) ;
#endif

	m_pForcus = new CINFImageEx;
	pDataHeader = FindResource("Focus");
	m_pForcus->InitDeviceObjects(pDataHeader ) ;

	m_pPartyFrame = new CINFImageEx; 
	pDataHeader = FindResource("pframe");
	m_pPartyFrame->InitDeviceObjects(pDataHeader ) ;

	m_pPartyFrameFuel = new CINFImageEx;
	pDataHeader = FindResource("pframe_f");
	m_pPartyFrameFuel->InitDeviceObjects(pDataHeader ) ;	

	m_pPartyFrameHP = new CINFImageEx;
	pDataHeader = FindResource("pframe_h");
	m_pPartyFrameHP->InitDeviceObjects(pDataHeader ) ;	
	
	m_pPartyTalking = new CINFImageEx;
	pDataHeader = FindResource("gtalkma");
	m_pPartyTalking->InitDeviceObjects(pDataHeader ) ;	
	
	{	
		// ����
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		wsprintf(szUpBtn, "cres03");
		wsprintf(szDownBtn, "cres01");
		wsprintf(szSelBtn, "cres00");
		wsprintf(szDisBtn, "cres02");															  
#else						  
		wsprintf(szUpBtn, "createb3");
		wsprintf(szDownBtn, "createb1");
		wsprintf(szSelBtn, "createb0");
		wsprintf(szDisBtn, "createb2");
#endif
		if(NULL == m_pSCrateBtn)
		{
			m_pSCrateBtn = new CINFImageBtn;
			// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
			//m_pSCrateBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
			m_pSCrateBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP47");
			// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
		}		
	}
	
	{	
		// ����
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����				
		wsprintf(szUpBtn, "pars03");
		wsprintf(szDownBtn, "pars01");
		wsprintf(szSelBtn, "pars00");
		wsprintf(szDisBtn, "pars02");															  
#else	
		wsprintf(szUpBtn, "c_pj3");
		wsprintf(szDownBtn, "c_pj1");
		wsprintf(szSelBtn, "c_pj0");
		wsprintf(szDisBtn, "c_pj2");
#endif
		if(NULL == m_pSJoinBtn)
		{
			m_pSJoinBtn = new CINFImageBtn;
			// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
			//m_pSJoinBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
			m_pSJoinBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP48");
			// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
		}		
	}	
	{	
		// ���ΰ�ħ
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����						
		wsprintf(szUpBtn, "refs03");
		wsprintf(szDownBtn, "refs01");
		wsprintf(szSelBtn, "refs00");
		wsprintf(szDisBtn, "refs02");																			
#else				 
		wsprintf(szUpBtn, "c_pr3");
		wsprintf(szDownBtn, "c_pr1");
		wsprintf(szSelBtn, "c_pr0");
		wsprintf(szDisBtn, "c_pr2");		
#endif
		
		if(NULL == m_pSRefreshBtn)
		{
			m_pSRefreshBtn = new CINFImageBtn;
			// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
			//m_pSRefreshBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
			m_pSRefreshBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP46");
			// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
		}		
	}
	{
		// ���� ����Ʈ 
		char  szScBall[30];	
		if(NULL == m_pScrollPartyList)
		{
			m_pScrollPartyList = new CINFArenaScrollBar;
		}		
		wsprintf(szScBall,"c_scrlb");		
		m_pScrollPartyList->InitDeviceObjects(MAX_PARTY_LIST_LINE, szScBall);
	}
	{
		pDataHeader = m_pGameData->Find("c_ps");		
		m_pImgSelect = new CINFImageEx;
		m_pImgSelect->InitDeviceObjects( pDataHeader );
	}
	{
		pDataHeader = m_pGameData->Find("lock1");		
		m_pLockImage = new CINFImageEx;
		m_pLockImage->InitDeviceObjects( pDataHeader );
	}
	{
		pDataHeader = m_pGameData->Find("lock0");		
		m_pUnLockImage = new CINFImageEx;
		m_pUnLockImage->InitDeviceObjects( pDataHeader );
	}

	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����					
		wsprintf(szUpBtn, "invs03");
		wsprintf(szDownBtn, "invs01");
		wsprintf(szSelBtn, "invs00");
		wsprintf(szDisBtn, "invs02");																	
#else	   
		wsprintf(szUpBtn, "p_bt03");
		wsprintf(szDownBtn, "p_bt01");
		wsprintf(szSelBtn, "p_bt00");
		wsprintf(szDisBtn, "p_bt02");
#endif
		if(NULL == m_pOJoinBtn)
		{
			m_pOJoinBtn = new CINFImageBtn;
			// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
			//m_pOJoinBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
			m_pOJoinBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP57");
			// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
		}		
	}

	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����									
		wsprintf(szUpBtn, "outs03");
		wsprintf(szDownBtn, "outs01");
		wsprintf(szSelBtn, "outs00");
		wsprintf(szDisBtn, "outs02");																				
#else  
		wsprintf(szUpBtn, "c_sbt13");
		wsprintf(szDownBtn, "c_sbt11");
		wsprintf(szSelBtn, "c_sbt10");
		wsprintf(szDisBtn, "c_sbt12");
#endif
		if(NULL == m_pOLeaveBtn)
		{
			m_pOLeaveBtn = new CINFImageBtn;
			// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
			//m_pOLeaveBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
			m_pOLeaveBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP59");
			// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
		}		
	}
	
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
		
		wsprintf(szUpBtn, "p_bt23");
		wsprintf(szDownBtn, "p_bt21");
		wsprintf(szSelBtn, "p_bt20");
		wsprintf(szDisBtn, "p_bt22");
		
		if(NULL == m_pOPurgeBtn)
		{
			m_pOPurgeBtn = new CINFImageBtn;
			// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
			//m_pOPurgeBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
			m_pOPurgeBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP58");
			// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
		}		
	}
	
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����		
		wsprintf(szUpBtn, "coms03");
		wsprintf(szDownBtn, "coms01");
		wsprintf(szSelBtn, "coms00");
		wsprintf(szDisBtn, "coms02");
#else									
		wsprintf(szUpBtn, "c_sbt33");
		wsprintf(szDownBtn, "c_sbt31");
		wsprintf(szSelBtn, "c_sbt30");
		wsprintf(szDisBtn, "c_sbt32");
#endif
		
		if(NULL == m_pOChargeBtn)
		{
			m_pOChargeBtn = new CINFImageBtn;
			// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
			//m_pOChargeBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
			m_pOChargeBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP60");
			// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
		}		
	}
	
	{
		// Ȯ��
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		wsprintf(szUpBtn, "apps03");
		wsprintf(szDownBtn, "apps01");
		wsprintf(szSelBtn, "apps00");
		wsprintf(szDisBtn, "apps02");															  
#else		 
		wsprintf(szUpBtn, "Ook3");
		wsprintf(szDownBtn, "Ook1");
		wsprintf(szSelBtn, "Ook0");
		wsprintf(szDisBtn, "Ook2");
#endif
		if(NULL == m_pOOkBtn)
		{
			m_pOOkBtn = new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
		//m_pOOkBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		m_pOOkBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP51");
		// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	}
	{
		// ��� ����
		char szName[30];
		wsprintf(szName,"p_pwar");
		if(NULL	== m_pOPartyBattle)
		{
			m_pOPartyBattle = new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
		//m_pOPartyBattle->InitDeviceObjects(szName);
		m_pOPartyBattle->InitDeviceObjects(szName,"STRTOOLTIP49");
		// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	}

	{
		if(NULL == m_pOPartyList)
		{
			// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
			//m_pOPartyList = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,128,32);
			m_pOPartyList = new CD3DHanFont( _T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE,  FALSE, 256, 32 );
			// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
		}
		m_pOPartyList->InitDeviceObjects(g_pD3dDev);
	}
	{
		if(NULL == m_pSelImage)
		{
			m_pSelImage = new CINFImageEx;	
		}		
		pDataHeader = FindResource("c_ps1");
		m_pSelImage->InitDeviceObjects(pDataHeader);
	}

	{
		if(NULL == m_pINFSecuEditBox)
		{
			m_pINFSecuEditBox = new CINFSecuEditBox;
		}
		
		POINT ptPos = {SECURITY_WND_PASS_EDIT.x, SECURITY_WND_PASS_EDIT.y};
		m_pINFSecuEditBox->InitDeviceObjects(9, ptPos, SECURITY_WND_PASS_EDIT_W, TRUE, SECURITY_WND_PASS_EDIT_H);		

		m_pINFSecuEditBox->SetMaxStringLen(SIZE_MAX_TEAM_PW-1);
		
	}
	{
		if(NULL == m_pNumMinEditBox)
		{
			m_pNumMinEditBox = new CINFNumEditBox;
		}
		char chBuff[32];
		char chMaxMixCnt[64];

		wsprintf(chBuff, "1");
		wsprintf(chMaxMixCnt, "%d", CHARACTER_MAX_LEVEL);

		POINT ptPos = {MIN_NUM_EDIT.x, MIN_NUM_EDIT.y};
		m_pNumMinEditBox->InitDeviceObjects(9, ptPos, MIN_NUM_EDIT_W, TRUE, MIN_NUM_EDIT_H);				
		m_pNumMinEditBox->SetMaxStringLen(strlen(chMaxMixCnt));
		m_pNumMinEditBox->SetString(chBuff, 32);
	}

	{
		if(NULL == m_pNumMaxEditBox)
		{
			m_pNumMaxEditBox = new CINFNumEditBox;
		}
		char chBuff[32];
		char chMaxMixCnt[64];

		wsprintf(chBuff, "100");
		wsprintf(chMaxMixCnt, "%d", CHARACTER_MAX_LEVEL);

		POINT ptPos = {MAX_NUM_EDIT.x, MAX_NUM_EDIT.y};
		m_pNumMaxEditBox->InitDeviceObjects(9, ptPos, MAX_NUM_EDIT_W, TRUE, MAX_NUM_EDIT_H);				
		m_pNumMaxEditBox->SetMaxStringLen(strlen(chMaxMixCnt));
		m_pNumMaxEditBox->SetString(chBuff, 32);
	}
	{
		if(NULL == m_pEditPartyName)
		{
			// 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
			//m_pEditPartyName = new CINFEditBox;
			m_pEditPartyName = new CINFSingleCurselEditBox;
		}
		POINT ptPos = {PARTY_NAME_EDIT.x, PARTY_NAME_EDIT.y};		
		m_pEditPartyName->InitDeviceObjects(9, ptPos, PARTY_NAME_EDIT_W, TRUE, PARTY_NAME_FONT_LINE_HEIGHT, 
										TRUE, PARTY_NAME_FONT_LINE_HEIGHT);				
		m_pEditPartyName->SetStringMaxBuff(EDIT_SIZE_MAX_PARTY_NAME);				
	}
	
	{
		char chRadioOff[30], chRadioOn[30];
		wsprintf(chRadioOff, "radio_17b");
		wsprintf(chRadioOn, "radio_17a");

		char chRadioOff1[30], chRadioOn1[30];
		
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����		
		wsprintf(chRadioOff1, "radio_17b");
		wsprintf(chRadioOn1, "radio_17a");														  
#else	
		wsprintf(chRadioOff1, "radio_b");
		wsprintf(chRadioOn1, "radio_a");
#endif

		for(nCnt = 0;nCnt < MAX_RADIO_JOINT;nCnt++)
		{		
			if(NULL == m_pPartyJoint[nCnt])
			{		
				m_pPartyJoint[nCnt] = new CINFImageRadioBtn;
			}				
			m_pPartyJoint[nCnt]->InitDeviceObjects(chRadioOff, chRadioOn);
		}		
		for(nCnt = 0;nCnt < MAX_RADIO_AUDIO;nCnt++)
		{		
			if(NULL == m_pAudioOp[nCnt])
			{		
				m_pAudioOp[nCnt] = new CINFImageRadioBtn;
			}				
			
			if(RADIO_AUDIO_CHAT == nCnt)
			{
				m_pAudioOp[nCnt]->InitDeviceObjects(chRadioOff1, chRadioOn1);				
			}
			else
			{
				m_pAudioOp[nCnt]->InitDeviceObjects(chRadioOff, chRadioOn);
			}
		}		
	}

	{
		char chTmpBuff[256];
		memset(chTmpBuff, 0x00, 256);

		for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
		{
			if(NULL == m_pComboDivision[nCnt])
			{
				//m_pComboDivision[nCnt] = new CINFListBox("cbopa","s_combo");				
				m_pComboDivision[nCnt] = new CINFListBox("c_cc","c_ccbk");

				m_pComboDivision[nCnt]->InitDeviceObjects();				
			}			
		}
		
		{
			wsprintf(chTmpBuff, STRMSG_C_080603_0200);		// "���� ����"
			m_pComboDivision[COMBO_DIVISION_EXP]->AddElement(chTmpBuff);	// 
			wsprintf(chTmpBuff, STRMSG_C_080603_0201);		// "���� �й�"
			m_pComboDivision[COMBO_DIVISION_EXP]->AddElement(chTmpBuff);	// 
		}
		{
			wsprintf(chTmpBuff, STRMSG_C_080603_0200);		// "���� ����"
			m_pComboDivision[COMBO_DIVISION_ITEM]->AddElement(chTmpBuff);	// 
			wsprintf(chTmpBuff, STRMSG_C_080603_0202);		// "���� ����"
			m_pComboDivision[COMBO_DIVISION_ITEM]->AddElement(chTmpBuff);	// 
			wsprintf(chTmpBuff, STRMSG_C_080603_0203);		// "���� ����"
			m_pComboDivision[COMBO_DIVISION_ITEM]->AddElement(chTmpBuff);	// 
		}
		
		for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
		{
			m_pComboDivision[nCnt]->UpdateItem();				
		}
	}
	{
		if(NULL == m_pComboForMation)
		{
			//m_pComboForMation = new CINFListBox("cbopa","s_combo");				
			m_pComboForMation = new CINFListBox("c_cc","c_ccbk");			
			m_pComboForMation->InitDeviceObjects();				
		}			
	}
	for(nCnt = 0;nCnt < MAX_COMBO_FLIGHT_FORM;nCnt++)
	{		
		char chBuff[16];
		if(NULL == m_pFormatImage[nCnt])
		{
			m_pFormatImage[nCnt] = new CINFImageEx;	
		}				
		wsprintf(chBuff, "c_pf%d", nCnt);
		pDataHeader = FindResource(chBuff);

		m_pFormatImage[nCnt]->InitDeviceObjects(pDataHeader);
	}
	for(nCnt = 0;nCnt < MAX_WND_PARTYMODE;nCnt++)	
	{		
		// ���
		char chBuff[16];
// 		if(NULL == m_pImgBk[nCnt])
// 		{
			//m_pImgBk[nCnt] = g_pGameMain->m_p;
			
//		}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		wsprintf(chBuff, "commu_F%d", nCnt + 1 );
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( chBuff );
		m_pImgBk[nCnt] = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pImgBk[nCnt]->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pImgBk[nCnt]->RestoreDeviceObjects();													  
#else  
		if(NULL == m_pImgBk[nCnt])
		{
			m_pImgBk[nCnt] = new CINFImageEx;	
		}				
		wsprintf(chBuff, "commu0_%d", nCnt);
		pDataHeader = FindResource(chBuff);

		m_pImgBk[nCnt]->InitDeviceObjects(pDataHeader);
#endif
	}
	m_pImgSpkVolBar = new CINFImageEx;
	pDataHeader = FindResource("scrl_b");
	m_pImgSpkVolBar->InitDeviceObjects(pDataHeader);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "F1_btn1" );
	m_pPartyControlBtn[ WND_PARTYMODE_OPTION ] = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );

	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "F2_btn1" );
	m_pPartyControlBtn[ WND_PARTYMODE_SEARCH ] = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
#endif
	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â

	return S_OK ;
}

HRESULT CINFCommunityParty::RestoreDeviceObjects()
{
	FLOG( "CINFCommunityParty::RestoreDeviceObjects()" );

	int nCnt = 0, i;

	for(i=0;i<3;i++)
	{
//		if(m_pPartyCreateButton[i])
//		m_pPartyCreateButton[i]->RestoreDeviceObjects();
//		if(m_pPartyInviteButton[i])
//		m_pPartyInviteButton[i]->RestoreDeviceObjects();
//		if(m_pPartySecedeButton[i])
//		m_pPartySecedeButton[i]->RestoreDeviceObjects();
//		if(m_pPartyBanButton[i])
//		m_pPartyBanButton[i]->RestoreDeviceObjects();
//		if(m_pPartyTrustButton[i])
//		m_pPartyTrustButton[i]->RestoreDeviceObjects();
//		if(m_pJoinFormationButton[i])
//		m_pJoinFormationButton[i]->RestoreDeviceObjects();
	}
	for(i=0;i<MAX_PARTY_NUMBER;i++)
	{
		if(m_pPartyList[i])
		m_pPartyList[i]->RestoreDeviceObjects();
		if(m_pPartyInfo[i])
		m_pPartyInfo[i]->RestoreDeviceObjects();
		if(m_pFontPartyFrame[i])
		m_pFontPartyFrame[i]->RestoreDeviceObjects();
	}
//	for(i=0;i<PARTY_FORMATION_NUMBER;i++)
//	{
//		if(m_pPartyFormation[i])
//		m_pPartyFormation[i]->RestoreDeviceObjects();
//	}

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	if(m_pFormat )
		m_pFormat->RestoreDeviceObjects();
#endif
//	if(m_pFriend )
//		m_pFriend->RestoreDeviceObjects();
//	if(m_pReject )
//		m_pReject->RestoreDeviceObjects();
	if(m_pForcus )
		m_pForcus->RestoreDeviceObjects();
//	if(m_pCombo )
//		m_pCombo->RestoreDeviceObjects();
//	if(m_pLogo )
//		m_pLogo->RestoreDeviceObjects();
//	if(m_pCaptainCombo )
//		m_pCaptainCombo->RestoreDeviceObjects();
//	if(m_pFormationInfo )
//		m_pFormationInfo->RestoreDeviceObjects();

	if(m_pPartyFrame )
		m_pPartyFrame->RestoreDeviceObjects();
	if(m_pPartyFrameFuel )
		m_pPartyFrameFuel->RestoreDeviceObjects();
	if(m_pPartyFrameHP )
		m_pPartyFrameHP->RestoreDeviceObjects();
	if(m_pPartyTalking )
		m_pPartyTalking->RestoreDeviceObjects();

//	for(i=0;i<4;i++)
//	{
//		if(m_pPartyVOIPButton[i])
//			m_pPartyVOIPButton[i]->RestoreDeviceObjects();
//	}

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();
	
	int nPosX, nPosY;
	nPosX = nPosY =0;	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����	
	if(m_pSCrateBtn)
	{
		m_pSCrateBtn->RestoreDeviceObjects();				
		nPosX = ptBkPos.x + CURRENT_PARTY_BTN_START.x;
		nPosY = ptBkPos.y + CURRENT_PARTY_BTN_START.y;		
		m_pSCrateBtn->SetBtnPosition(nPosX, nPosY);	
	}
	
	if(m_pSJoinBtn)
	{
		m_pSJoinBtn->RestoreDeviceObjects();				
		nPosX = ptBkPos.x + CURRENT_PARTY_BTN_START.x;
		nPosY = ptBkPos.y + CURRENT_PARTY_BTN_START.y;		
		m_pSJoinBtn->SetBtnPosition(nPosX, nPosY);	
	}	
	if(m_pSRefreshBtn)
	{
		m_pSRefreshBtn->RestoreDeviceObjects();				
		nPosX = ptBkPos.x + CURRENT_PARTY_BTN_START.x;
		nPosY = ptBkPos.y + CURRENT_PARTY_BTN_START.y;		
		m_pSRefreshBtn->SetBtnPosition(nPosX, nPosY);	
	}
	
	if(m_pScrollPartyList)
	{
		m_pScrollPartyList->RestoreDeviceObjects();		

		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;
		
		ptScrollPos.x += 490;
		ptScrollPos.y;
		
		// ��ũ�� x = ��ġ�� -5
		// ��ũ�� height = �̹��� ������ - 34
		m_pScrollPartyList->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,247);
		rcMouseWhell.left		= ptScrollPos.x - COMMU_BK_SEARCH_WIDTH;
		rcMouseWhell.top		= ptScrollPos.y;
		rcMouseWhell.right		= ptScrollPos.x;
		rcMouseWhell.bottom		= ptScrollPos.y + 280;
		m_pScrollPartyList->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y - 247;
		rcMousePos.right		= ptScrollPos.x + 11;
		rcMousePos.bottom		= ptScrollPos.y;
		m_pScrollPartyList->SetMouseBallRect(rcMousePos);
	}		
#else		 
	if(m_pSCrateBtn)
	{
		m_pSCrateBtn->RestoreDeviceObjects();				
		nPosX = ptBkPos.x + CURRENT_PARTY_CREATE_BTN_X;
		nPosY = ptBkPos.y + CURRENT_PARTY_CREATE_BTN_Y;		
		m_pSCrateBtn->SetBtnPosition(nPosX, nPosY);	
	}
	
	if(m_pSJoinBtn)
	{
		m_pSJoinBtn->RestoreDeviceObjects();				
		nPosX = ptBkPos.x + CURRENT_PARTY_JOIN_BTN_X;
		nPosY = ptBkPos.y + CURRENT_PARTY_JOIN_BTN_Y;		
		m_pSJoinBtn->SetBtnPosition(nPosX, nPosY);	
	}	
	if(m_pSRefreshBtn)
	{
		m_pSRefreshBtn->RestoreDeviceObjects();				
		nPosX = ptBkPos.x + CURRENT_PARTY_REFRESH_BTN_X;
		nPosY = ptBkPos.y + CURRENT_PARTY_REFRESH_BTN_Y;		
		m_pSRefreshBtn->SetBtnPosition(nPosX, nPosY);	
	}
	
	if(m_pScrollPartyList)
	{
		m_pScrollPartyList->RestoreDeviceObjects();		

		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;
		
		ptScrollPos.x += 485;
		ptScrollPos.y += 98;
		
		// ��ũ�� x = ��ġ�� -5
		// ��ũ�� height = �̹��� ������ - 34
		m_pScrollPartyList->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,217);
		rcMouseWhell.left		= ptScrollPos.x - COMMU_BK_SEARCH_WIDTH;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 227;
		m_pScrollPartyList->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 237;
		m_pScrollPartyList->SetMouseBallRect(rcMousePos);
	}
#endif
	if(m_pImgSelect)
	{
		m_pImgSelect->RestoreDeviceObjects();
	}
	if(m_pLockImage)
	{
		m_pLockImage->RestoreDeviceObjects();
	}
	if(m_pUnLockImage)
	{
		m_pUnLockImage->RestoreDeviceObjects();
	}
	/////////// ��� �ɼ� â///////////////
	if(m_pOJoinBtn)
	{
		m_pOJoinBtn->RestoreDeviceObjects();						
	}
	if(m_pOLeaveBtn)
	{
		m_pOLeaveBtn->RestoreDeviceObjects();						
	}
	if(m_pOPurgeBtn)
	{
		m_pOPurgeBtn->RestoreDeviceObjects();						
	}
	if(m_pOChargeBtn)
	{
		m_pOChargeBtn->RestoreDeviceObjects();						
	}	
	if(m_pOOkBtn)
	{
		m_pOOkBtn->RestoreDeviceObjects();						
	}
	if(m_pOPartyBattle)
	{
		m_pOPartyBattle->RestoreDeviceObjects();						
	}
	if(m_pOPartyList)
	{
		m_pOPartyList->RestoreDeviceObjects();
	}
	if(m_pSelImage)
	{
		m_pSelImage->RestoreDeviceObjects();
	}

	if(m_pINFSecuEditBox)
	{
		m_pINFSecuEditBox->RestoreDeviceObjects();		
	}
	if(m_pNumMinEditBox)
	{
		m_pNumMinEditBox->RestoreDeviceObjects();		
	}
	if(m_pNumMaxEditBox)
	{
		m_pNumMaxEditBox->RestoreDeviceObjects();		
	}
	if(m_pEditPartyName)
	{
		m_pEditPartyName->RestoreDeviceObjects();		
	}	
	if(m_pImgSpkVolBar)
	{
		m_pImgSpkVolBar->RestoreDeviceObjects();
	}
	
	{			
		for(nCnt = 0;nCnt < MAX_RADIO_JOINT;nCnt++)
		{	
			if(m_pPartyJoint[nCnt])
			{
				m_pPartyJoint[nCnt]->RestoreDeviceObjects();			
			}
		}
		for(nCnt = 0;nCnt < MAX_RADIO_AUDIO;nCnt++)
		{				
			if(m_pAudioOp[nCnt])
			{
				m_pAudioOp[nCnt]->RestoreDeviceObjects();			
			}
		}		
	}
	for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
	{	
		if(m_pComboDivision[nCnt])
		{
			m_pComboDivision[nCnt]->RestoreDeviceObjects();			
		}
	}
	if(m_pComboForMation)
	{
		m_pComboForMation->RestoreDeviceObjects();						
	}
	for(nCnt = 0;nCnt < MAX_COMBO_FLIGHT_FORM;nCnt++)
	{	
		if(m_pFormatImage[nCnt])
		{
			m_pFormatImage[nCnt]->RestoreDeviceObjects();
		}		
	}
	for(nCnt = 0;nCnt < MAX_WND_PARTYMODE;nCnt++)
	{
		// ���
		if(m_pImgBk[nCnt])
		{
			m_pImgBk[nCnt]->RestoreDeviceObjects();
		}
	}

	InitComboInfo();	// �޺��ڽ�

	UpdateUIPos(); // ��ġ ����
	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	
//	if(m_pGuild )
//		m_pGuild->RestoreDeviceObjects();

	m_bRestored = TRUE;
	return S_OK ;
}

HRESULT CINFCommunityParty::DeleteDeviceObjects()
{
	FLOG( "CINFCommunityParty::DeleteDeviceObjects()" );

	int nCnt = 0, i;

	for(i=0;i<3;i++)
	{
//		if(m_pPartyCreateButton[i])
//		m_pPartyCreateButton[i]->DeleteDeviceObjects();
//		util::del(m_pPartyCreateButton[i]);
//		if(m_pPartyInviteButton[i])
//		m_pPartyInviteButton[i]->DeleteDeviceObjects();
//		util::del(m_pPartyInviteButton[i]);
//		if(m_pPartySecedeButton[i])
//		m_pPartySecedeButton[i]->DeleteDeviceObjects();
//		util::del(m_pPartySecedeButton[i]);
//		if(m_pPartyBanButton[i])
//		m_pPartyBanButton[i]->DeleteDeviceObjects();
//		util::del(m_pPartyBanButton[i]);
//		if(m_pPartyTrustButton[i])
//		m_pPartyTrustButton[i]->DeleteDeviceObjects();
//		util::del(m_pPartyTrustButton[i]);
//		if(m_pJoinFormationButton[i])
//		m_pJoinFormationButton[i]->DeleteDeviceObjects();
//		util::del(m_pJoinFormationButton[i]);
	}
	for(i=0;i<MAX_PARTY_NUMBER;i++)
	{
		if(m_pPartyList[i])
		m_pPartyList[i]->DeleteDeviceObjects();
		if(m_pPartyInfo[i])
		m_pPartyInfo[i]->DeleteDeviceObjects();
		if(m_pFontPartyFrame[i])
		m_pFontPartyFrame[i]->DeleteDeviceObjects();
		util::del(m_pPartyList[i]);
		util::del(m_pPartyInfo[i]);
		util::del(m_pFontPartyFrame[i]);
	}
//	for(i=0;i<PARTY_FORMATION_NUMBER;i++)
//	{
//		if(m_pPartyFormation[i])
//		m_pPartyFormation[i]->DeleteDeviceObjects();
//		util::del(m_pPartyFormation[i]);
//	}

//	for(i=0;i<4;i++)
//	{
//		m_pPartyVOIPButton[i]->DeleteDeviceObjects();
//		util::del(m_pPartyVOIPButton[i]);
//	}

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	if(m_pFormat )
		m_pFormat->DeleteDeviceObjects();
	util::del(m_pFormat );
#endif

	if(m_pForcus )
		m_pForcus->DeleteDeviceObjects();
	util::del(m_pForcus );

//	if(m_pCombo )
//		m_pCombo->DeleteDeviceObjects();
//	util::del(m_pCombo );
//	if(m_pCaptainCombo )
//		m_pCaptainCombo->DeleteDeviceObjects();
//	util::del(m_pCaptainCombo );
//	if(m_pFormationInfo )
//		m_pFormationInfo->DeleteDeviceObjects();
//	util::del(m_pFormationInfo );
	if(m_pPartyFrame )
		m_pPartyFrame->DeleteDeviceObjects();
	util::del(m_pPartyFrame );
	if(m_pPartyFrameFuel )
		m_pPartyFrameFuel->DeleteDeviceObjects();
	util::del(m_pPartyFrameFuel );
	if(m_pPartyFrameHP )
		m_pPartyFrameHP->DeleteDeviceObjects();
	util::del(m_pPartyFrameHP );
	if(m_pPartyTalking )
		m_pPartyTalking->DeleteDeviceObjects();
	util::del(m_pPartyTalking );
	
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â	
	if(m_pSCrateBtn)
	{		
		m_pSCrateBtn->DeleteDeviceObjects();
		util::del(m_pSCrateBtn);
	}	
	if(m_pSJoinBtn)
	{		
		m_pSJoinBtn->DeleteDeviceObjects();
		util::del(m_pSJoinBtn);
	}
	
	if(m_pSRefreshBtn)
	{		
		m_pSRefreshBtn->DeleteDeviceObjects();
		util::del(m_pSRefreshBtn);
	}
	if(m_pScrollPartyList)
	{
		m_pScrollPartyList->DeleteDeviceObjects();
		util::del(m_pScrollPartyList);		
	}
	if(m_pImgSelect)
	{
		m_pImgSelect->DeleteDeviceObjects();
		util::del(m_pImgSelect);
	}
	if(m_pLockImage)
	{
		m_pLockImage->DeleteDeviceObjects();
		util::del(m_pLockImage);
	}
	if(m_pUnLockImage)
	{
		m_pUnLockImage->DeleteDeviceObjects();
		util::del(m_pUnLockImage);
	}
	////////// �ɼ�â
	if(m_pOJoinBtn)
	{		
		m_pOJoinBtn->DeleteDeviceObjects();
		util::del(m_pOJoinBtn);
	}
	if(m_pOLeaveBtn)
	{		
		m_pOLeaveBtn->DeleteDeviceObjects();
		util::del(m_pOLeaveBtn);
	}
	if(m_pOPurgeBtn)
	{		
		m_pOPurgeBtn->DeleteDeviceObjects();
		util::del(m_pOPurgeBtn);
	}
	if(m_pOChargeBtn)
	{		
		m_pOChargeBtn->DeleteDeviceObjects();
		util::del(m_pOChargeBtn);
	}
	
	if(m_pOOkBtn)
	{		
		m_pOOkBtn->DeleteDeviceObjects();
		util::del(m_pOOkBtn);
	}	
	if(m_pOPartyBattle)
	{		
		m_pOPartyBattle->DeleteDeviceObjects();
		util::del(m_pOPartyBattle);
	}	
	if(m_pOPartyList)
	{	
		m_pOPartyList->DeleteDeviceObjects();
		util::del(m_pOPartyList);
	}
	if(m_pSelImage)
	{
		m_pSelImage->DeleteDeviceObjects();
		util::del(m_pSelImage );
	}
	if(m_pINFSecuEditBox)
	{
		m_pINFSecuEditBox->DeleteDeviceObjects();	
		util::del(m_pINFSecuEditBox);
	}
	if(m_pNumMinEditBox)
	{
		m_pNumMinEditBox->DeleteDeviceObjects();	
		util::del(m_pNumMinEditBox);
	}		
	if(m_pNumMaxEditBox)
	{
		m_pNumMaxEditBox->DeleteDeviceObjects();	
		util::del(m_pNumMaxEditBox);
	}
	if(m_pEditPartyName)
	{
		m_pEditPartyName->DeleteDeviceObjects();
		util::del(m_pEditPartyName);		
	}	
	if(m_pImgSpkVolBar)
	{
		m_pImgSpkVolBar->DeleteDeviceObjects();
		util::del(m_pImgSpkVolBar);	
	}
	{
		for(nCnt = 0;nCnt < MAX_RADIO_JOINT;nCnt++)
		{		
			if(m_pPartyJoint[nCnt])
			{
				m_pPartyJoint[nCnt]->DeleteDeviceObjects();	
				util::del(m_pPartyJoint[nCnt]);
			}
		}
		for(nCnt = 0;nCnt < MAX_RADIO_AUDIO;nCnt++)
		{				
			if(m_pAudioOp[nCnt])
			{
				m_pAudioOp[nCnt]->DeleteDeviceObjects();	
				util::del(m_pAudioOp[nCnt]);
			}
		}		
	}
	for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
	{			
		if(m_pComboDivision[nCnt])
		{
			m_pComboDivision[nCnt]->DeleteDeviceObjects();	
			util::del(m_pComboDivision[nCnt]);
		}
	}	
	if(m_pComboForMation)
	{		
		m_pComboForMation->DeleteDeviceObjects();
		util::del(m_pComboForMation);
	}
	for(nCnt = 0;nCnt < MAX_COMBO_FLIGHT_FORM;nCnt++)
	{	
		if(m_pFormatImage[nCnt])
		{
			m_pFormatImage[nCnt]->DeleteDeviceObjects();	
			util::del(m_pFormatImage[nCnt]);
		}		
	}
	for(nCnt = 0;nCnt < MAX_WND_PARTYMODE;nCnt++)
	{
		// ���
		if(m_pImgBk[nCnt])
		{
			m_pImgBk[nCnt]->DeleteDeviceObjects();	
			util::del(m_pImgBk[nCnt]);
		}
	}
	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	for(nCnt = 0;nCnt < MAX_WND_PARTYMODE;nCnt++)
	{
		// ���
		if(m_pPartyControlBtn[nCnt])
		{
			m_pPartyControlBtn[nCnt]->DeleteDeviceObjects();	
			util::del(m_pPartyControlBtn[nCnt]);
		}
	}
#endif

	m_bInvalidated = FALSE;
	return S_OK ;
}


HRESULT CINFCommunityParty::InvalidateDeviceObjects()
{
	FLOG( "CINFCommunityParty::InvalidateDeviceObjects()" );
	int i;
	for(i=0;i<3;i++)
	{
//		if(m_pPartyCreateButton[i])
//		m_pPartyCreateButton[i]->InvalidateDeviceObjects();
//		if(m_pPartyInviteButton[i])
//		m_pPartyInviteButton[i]->InvalidateDeviceObjects();
//		if(m_pPartySecedeButton[i])
//		m_pPartySecedeButton[i]->InvalidateDeviceObjects();
//		if(m_pPartyBanButton[i])
//		m_pPartyBanButton[i]->InvalidateDeviceObjects();
//		if(m_pPartyTrustButton[i])
//		m_pPartyTrustButton[i]->InvalidateDeviceObjects();
//		if(m_pJoinFormationButton[i])
//		m_pJoinFormationButton[i]->InvalidateDeviceObjects();
	}
	for(i=0;i<MAX_PARTY_NUMBER;i++)
	{
		if(m_pPartyList[i])
		m_pPartyList[i]->InvalidateDeviceObjects();
		if(m_pPartyInfo[i])
		m_pPartyInfo[i]->InvalidateDeviceObjects();
		if(m_pFontPartyFrame[i])
		m_pFontPartyFrame[i]->InvalidateDeviceObjects();
	}
//	for(i=0;i<PARTY_FORMATION_NUMBER;i++)
//	{
//		if(m_pPartyFormation[i])
//		m_pPartyFormation[i]->InvalidateDeviceObjects();
//	}

//	for(i=0;i<4;i++)
//	{
//		m_pPartyVOIPButton[i]->InvalidateDeviceObjects();
//	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	if(m_pFormat )
		m_pFormat->InvalidateDeviceObjects();
#endif

	if(m_pForcus )
		m_pForcus->InvalidateDeviceObjects();
//	if(m_pCombo )
//		m_pCombo->InvalidateDeviceObjects();

//	if(m_pCaptainCombo )
//		m_pCaptainCombo->InvalidateDeviceObjects();
//	if(m_pFormationInfo )
//		m_pFormationInfo->InvalidateDeviceObjects();
	if(m_pPartyFrame )
		m_pPartyFrame->InvalidateDeviceObjects();
	if(m_pPartyFrameFuel )
		m_pPartyFrameFuel->InvalidateDeviceObjects();
	if(m_pPartyFrameHP )
		m_pPartyFrameHP->InvalidateDeviceObjects();
	if(m_pPartyTalking )
		m_pPartyTalking->InvalidateDeviceObjects();

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â	
	if(m_pSCrateBtn)
	{
		m_pSCrateBtn->InvalidateDeviceObjects();
	}	
	if(m_pSJoinBtn)
	{
		m_pSJoinBtn->InvalidateDeviceObjects();
	}
	if(m_pSRefreshBtn)
	{
		m_pSRefreshBtn->InvalidateDeviceObjects();
	}
	if(m_pScrollPartyList)
	{
		m_pScrollPartyList->InvalidateDeviceObjects();
	}
	if(m_pImgSelect)
	{
		m_pImgSelect->InvalidateDeviceObjects();
	}
	if(m_pLockImage)
	{
		m_pLockImage->InvalidateDeviceObjects();
	}
	if(m_pUnLockImage)
	{
		m_pUnLockImage->InvalidateDeviceObjects();
	}
	/////////////// ��� �ɼ�â ///////////////////////// 
	if(m_pOJoinBtn)
	{		
		m_pOJoinBtn->InvalidateDeviceObjects();
	}
	if(m_pOLeaveBtn)
	{		
		m_pOLeaveBtn->InvalidateDeviceObjects();
	}
	if(m_pOPurgeBtn)
	{		
		m_pOPurgeBtn->InvalidateDeviceObjects();
	}
	if(m_pOChargeBtn)
	{		
		m_pOChargeBtn->InvalidateDeviceObjects();
	}	
	if(m_pOOkBtn)
	{		
		m_pOOkBtn->InvalidateDeviceObjects();		
	}
	if(m_pOPartyBattle)
	{		
		m_pOPartyBattle->InvalidateDeviceObjects();		
	}
	if(m_pOPartyList)
	{
		m_pOPartyList->InvalidateDeviceObjects();
	}
	if(m_pSelImage)
	{
		m_pSelImage->InvalidateDeviceObjects();
	}
	if(m_pINFSecuEditBox)
	{
		m_pINFSecuEditBox->InvalidateDeviceObjects();			
	}
	if(m_pNumMinEditBox)
	{
		m_pNumMinEditBox->InvalidateDeviceObjects();
	}
	if(m_pNumMaxEditBox)
	{
		m_pNumMaxEditBox->InvalidateDeviceObjects();
	}
	if(m_pEditPartyName)
	{
		m_pEditPartyName->InvalidateDeviceObjects();
	}	
	if(m_pImgSpkVolBar)
	{
		m_pImgSpkVolBar->InvalidateDeviceObjects();
	}

	int nCnt = 0;
	{
		for(nCnt = 0;nCnt < MAX_RADIO_JOINT;nCnt++)
		{		
			if(m_pPartyJoint[nCnt])
			{
				m_pPartyJoint[nCnt]->InvalidateDeviceObjects();
			}
		}
		for(nCnt = 0;nCnt < MAX_RADIO_AUDIO;nCnt++)
		{				
			if(m_pAudioOp[nCnt])
			{
				m_pAudioOp[nCnt]->InvalidateDeviceObjects();				
			}
		}
	}
	for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
	{			
		if(m_pComboDivision[nCnt])
		{
			m_pComboDivision[nCnt]->InvalidateDeviceObjects();
		}
	}
	if(m_pComboForMation)
	{
		m_pComboForMation->InvalidateDeviceObjects();
	}
	for(nCnt = 0;nCnt < MAX_COMBO_FLIGHT_FORM;nCnt++)
	{	
		if(m_pFormatImage[nCnt])
		{
			m_pFormatImage[nCnt]->InvalidateDeviceObjects();				
		}		
	}
	for(nCnt = 0;nCnt < MAX_WND_PARTYMODE;nCnt++)
	{
		// ���
		if(m_pImgBk[nCnt])
		{
			m_pImgBk[nCnt]->InvalidateDeviceObjects();
		}
	}
	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	for(nCnt = 0;nCnt < MAX_WND_PARTYMODE;nCnt++)
	{
		// ���
		if(m_pPartyControlBtn[nCnt])
		{
			m_pPartyControlBtn[nCnt]->InvalidateDeviceObjects();	
		}
	}																							  
#endif

	m_bRestored = FALSE;
	m_bInvalidated = TRUE;
	return S_OK ;
}

void CINFCommunityParty::Tick()
{
	FLOG( "CINFCommunityParty::Tick()" );
	
	if(g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.size())
	{
		m_fSpeakingTimers -= g_pD3dApp->GetElapsedTime();
	}
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	if(m_bPartyTypeBtn != g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType)
	{
		ShowPartyFrame();
	}
	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
}


// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
void	CINFCommunityParty::RenderPartyDebuffIcon( PARTYENEMYINFO* pPartyMember, const int nLineNum )
{
	// ��Ʈ ������ ���������� ������
	// 5���� 2�� �ִ� 10�� ����
	BOOL bBoolTip = TRUE;
	int nCount = 0;
	for( std::vector<sDebuffDotInfo>::iterator it = pPartyMember->m_vecDebuffDotInfo.begin();
		 it != pPartyMember->m_vecDebuffDotInfo.end() && nCount < 10;
		 ++it, ++nCount )
	{
		char temp[ 64 ];
		wsprintf( temp, "%08d", (*it).nSourceIndex );
		
		// ������ ��ġ ���
		int nX, nY;
		if( nCount < PARTY_FRAME_DEBUFF_LINE_PER_ICON )
		{
			nX = nCount;
			nY = 0;
		}
		else
		{
			nX = nCount - PARTY_FRAME_DEBUFF_LINE_PER_ICON;
			nY = PARTY_FRAME_DEBUFF_ICON_SIZE;
		}

		g_pGameMain->m_pIcon->SetIcon( temp, PARTY_FRAME_DEBUFF_START_X + PARTY_FRAME_DEBUFF_ICON_SIZE * nX, FRAME_START_Y + PARTY_FRAME_SIZE_Y * nLineNum + nY, 0.5f );
		g_pGameMain->m_pIcon->Render();

		// ���� ���콺 ����
		RECT rect;
		rect.left	= PARTY_FRAME_DEBUFF_START_X + PARTY_FRAME_DEBUFF_ICON_SIZE * nX;
		rect.top	= FRAME_START_Y + PARTY_FRAME_SIZE_Y * nLineNum + nY;
		rect.right	= rect.left + PARTY_FRAME_DEBUFF_ICON_SIZE;
		rect.bottom	= rect.top + PARTY_FRAME_DEBUFF_ICON_SIZE;

		POINT ptMouse;
		GetCursorPos( &ptMouse );
		ScreenToClient(g_pD3dApp->GetHwnd(), &ptMouse);
		
		// ���� ���� üũ
		if( bBoolTip && PtInRect( &rect, ptMouse ) )
		{
			ITEM* pItem = g_pDatabase->GetServerItemInfo( (*it).nSkillItemNum );
			if( pItem )
			{
				POINT	drawPt;
				SIZE	strSize = m_pOPartyList->GetStringSize( pItem->ItemName );
				drawPt.x	= ptMouse.x + PARTY_FRAME_DEBUFF_ICON_SIZE * 2;
				drawPt.y	= ptMouse.y;
				
				// ���� ������
				g_pGameMain->RenderPopUpWindowImage(drawPt.x, drawPt.y + strSize.cy * 0.1f, strSize.cx, 1);
				m_pOPartyList->DrawText(drawPt.x, drawPt.y, GUI_FONT_COLOR_Y, pItem->ItemName );

				// ������ �Ѱ��� ������ �Ѵ�
				bBoolTip = FALSE;
			}
		}
	}
}
// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����


void CINFCommunityParty::RenderPartyFrame()
{
	FLOG( "CINFCommunityParty::RenderPartyFrame()" );

	DWORD dwColor;
	float fCurrent;
	int i=0;
	char buf[128];
	
	auto itParty = g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.begin();
	
	while (itParty != g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.end())
	{
		m_pPartyFrame->Move(0, FRAME_START_Y+PARTY_FRAME_SIZE_Y*i);
		m_pPartyFrame->Render();

		auto& info = (*itParty)->m_ImPartyMemberInfo;

		strcpy(buf, info.CharacterName);
		
		// 2008-02-26 by bhsohn ���� �Ʒ��� ���� ���� ����
		g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), buf);

		switch((*itParty)->m_bPartyType)
		{
		case _PARTYMASTER:
			{
				strcat(buf," @");

				dwColor = info.NameColor == 0 ? ColorConvert8to24(info.NameColor) : GUI_FONT_COLOR;

				// 2008-12-02 by bhsohn ���� �¿������� ����üũ
				if(!(*itParty)->m_bUserLogOn) dwColor = GUI_SELECT_FONT_COLOR; // ���������� ǥ��
				// end 2008-12-02 by bhsohn ���� �¿������� ����üũ

				m_pFontPartyFrame[i]->DrawText(PARTY_FRAME_ID_START_X,FRAME_START_Y+PARTY_FRAME_ID_START_Y+PARTY_FRAME_SIZE_Y*i,dwColor,buf,0L);
				m_pPartyFrameHP->Move(PARTY_FRAME_HP_START_X, FRAME_START_Y+PARTY_FRAME_HP_START_Y+PARTY_FRAME_SIZE_Y*i);
				fCurrent = (float)(*itParty)->m_pEnemyData->m_infoCharacter.CurrentHP/(float)(*itParty)->m_pEnemyData->m_infoCharacter.HP;
				if(fCurrent > 1.0f)
					fCurrent = 1.0f;
				m_pPartyFrameHP->SetScale(fCurrent, 1.0f);
				
				m_pPartyFrameHP->Render();
				m_pPartyFrameFuel->Move(PARTY_FRAME_FUEL_START_X, FRAME_START_Y+PARTY_FRAME_FUEL_START_Y+PARTY_FRAME_SIZE_Y*i);
				
				fCurrent = (float)(*itParty)->m_pEnemyData->m_infoCharacter.CurrentDP/(float)(*itParty)->m_pEnemyData->m_infoCharacter.DP;
				if(fCurrent > 1.0f)
					fCurrent = 1.0f;
				m_pPartyFrameFuel->SetScale(fCurrent, 1.0f);
				m_pPartyFrameFuel->Render();
				
				// ���ϱ� �̹��� ������ 
				if (m_fSpeakingTimers < 0)
				{
					m_bRenderingSpeak = !m_bRenderingSpeak;
					m_fSpeakingTimers = RENDER_SPEAKING_TIMING;
				}
				// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
				RenderPartyDebuffIcon( (*itParty), i );
				// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
			}
			break;
		case _PARTYMEMBER:
			{
				dwColor = info.NameColor == 0 ? ColorConvert8to24(info.NameColor) : GUI_FONT_COLOR;
				// 2008-12-02 by bhsohn ���� �¿������� ����üũ
				if(FALSE == (*itParty)->m_bUserLogOn)
				{
					dwColor = GUI_SELECT_FONT_COLOR; // ���������� ǥ��
				}				
				// end 2008-12-02 by bhsohn ���� �¿������� ����üũ

				m_pFontPartyFrame[i]->DrawText(PARTY_FRAME_ID_START_X,FRAME_START_Y+PARTY_FRAME_ID_START_Y+PARTY_FRAME_SIZE_Y*i,dwColor,buf,0L);
				m_pPartyFrameHP->Move(PARTY_FRAME_HP_START_X, FRAME_START_Y+PARTY_FRAME_HP_START_Y+PARTY_FRAME_SIZE_Y*i);
				fCurrent = (float)(*itParty)->m_pEnemyData->m_infoCharacter.CurrentHP/(float)(*itParty)->m_pEnemyData->m_infoCharacter.HP;
				if(fCurrent > 1.0f)
					fCurrent = 1.0f;
				m_pPartyFrameHP->SetScale(fCurrent, 1.0f);
				m_pPartyFrameHP->Render();
				
				fCurrent = (float)(*itParty)->m_pEnemyData->m_infoCharacter.CurrentDP/(float)(*itParty)->m_pEnemyData->m_infoCharacter.DP;
				if(fCurrent > 1.0f)
					fCurrent = 1.0f;
				m_pPartyFrameFuel->Move(PARTY_FRAME_FUEL_START_X, FRAME_START_Y+PARTY_FRAME_FUEL_START_Y+PARTY_FRAME_SIZE_Y*i);
				m_pPartyFrameFuel->SetScale(fCurrent, 1.0f);
				m_pPartyFrameFuel->Render();
				
				// ���ϱ� �̹��� ������ 
				if(m_fSpeakingTimers < 0)
				{
					m_bRenderingSpeak = !m_bRenderingSpeak;				
					m_fSpeakingTimers = RENDER_SPEAKING_TIMING;
				}				
				// end 2008-10-14 by bhsohn VOIP����


				// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
				RenderPartyDebuffIcon( (*itParty), i );
				// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
			}
			break;
		case _PARTYOTHERMAPMASTER:
			{
				strcat(buf," @");
				// 2008-12-02 by bhsohn ���� �¿������� ����üũ
				dwColor = info.NameColor == 0 ? ColorDivide24(ColorConvert8to24(info.NameColor), 2) : RGB(128,128,128);
				
				if (!(*itParty)->m_bUserLogOn) dwColor = GUI_SELECT_FONT_COLOR; // ���������� ǥ��
				
				m_pFontPartyFrame[i]->DrawText(
					PARTY_FRAME_ID_START_X,FRAME_START_Y+PARTY_FRAME_ID_START_Y+PARTY_FRAME_SIZE_Y*i,dwColor,buf,0L);
				// end 2008-12-02 by bhsohn ���� �¿������� ����üũ
				
				// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
				RenderPartyDebuffIcon( (*itParty), i );
				// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
			}
			break;
		case _PARTYOTHERMAPMEMBER:
			{				
				// 2008-12-02 by bhsohn ���� �¿������� ����üũ
				dwColor = info.NameColor == 0 ? ColorDivide24(ColorConvert8to24(info.NameColor), 2) : RGB(128,128,128);
				if(FALSE == (*itParty)->m_bUserLogOn)
				{
					dwColor = GUI_SELECT_FONT_COLOR; // ���������� ǥ��
				}
				m_pFontPartyFrame[i]->DrawText(
					PARTY_FRAME_ID_START_X,FRAME_START_Y+PARTY_FRAME_ID_START_Y+PARTY_FRAME_SIZE_Y*i,dwColor,buf,0L);
				// end 2008-12-02 by bhsohn ���� �¿������� ����üũ
				
				// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
				RenderPartyDebuffIcon( (*itParty), i );
				// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
			}
			break;
		case _PARTYRESTARTMEMBER:
			{
				// 2008-12-02 by bhsohn ���� �¿������� ����üũ
				dwColor = info.NameColor == 0 ? ColorDivide24(ColorConvert8to24(info.NameColor), 4) : RGB(64,64,64);
				if(FALSE == (*itParty)->m_bUserLogOn)
				{
					dwColor = GUI_SELECT_FONT_COLOR; // ���������� ǥ��
				}
				m_pFontPartyFrame[i]->DrawText(
					PARTY_FRAME_ID_START_X,FRAME_START_Y+PARTY_FRAME_ID_START_Y+PARTY_FRAME_SIZE_Y*i,dwColor,buf,0L);
				// end 2008-12-02 by bhsohn ���� �¿������� ����üũ

				// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
				RenderPartyDebuffIcon( (*itParty), i );
				// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
			}
			break;
		default:
			{
				// 2008-12-02 by bhsohn ���� �¿������� ����üũ
				dwColor = RGB(128,128,128);
				if (!(*itParty)->m_bUserLogOn) dwColor = GUI_SELECT_FONT_COLOR; // ���������� ǥ��
							
				m_pFontPartyFrame[i]->DrawText(PARTY_FRAME_ID_START_X,
					FRAME_START_Y+PARTY_FRAME_ID_START_Y+PARTY_FRAME_SIZE_Y*i,dwColor,buf,0L);					
				// end 2008-12-02 by bhsohn ���� �¿������� ����üũ

				// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
				RenderPartyDebuffIcon( (*itParty), i );
				// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
			}
			break;
			}
			i++;
			itParty++;
	}
}


void CINFCommunityParty::Render(POINT ptPos)
{
	FLOG( "CINFCommunityParty::RenderParty()" );
	//int nWindowPosY = g_pGameMain->m_nLeftWindowY;
	m_ptWindowPos = ptPos;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	if(m_pFormat)
	{
		//m_pFormat->Move(13,nWindowPosY + 30);
		m_pFormat->Move(ptPos.x, ptPos.y);
		m_pFormat->Render() ;
	}
#endif
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â	
	int nSelImg = m_nSubWndMode;
	if( (nSelImg >=0)&& (nSelImg < MAX_WND_PARTYMODE))
	{
		// ���
		m_pImgBk[nSelImg]->Move(ptPos.x + COMMU_BK_START.x, ptPos.y + COMMU_BK_START.y);
		m_pImgBk[nSelImg]->Render();		
	}
	switch(m_nSubWndMode)
	{
	case WND_PARTYMODE_OPTION:
		{
			RenderOption(ptPos);
		}
		break;
	case WND_PARTYMODE_SEARCH:
		{
			RenderSeach(ptPos);			
		}
		break;
	}
	
	return;
	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â	
	
//	if(g_pD3dApp->m_pShuttleChild->m_infoParty.bPartyType == _PARTYMASTER)
//	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)
//	{
		// ��ư : ����		
//		m_pPartyCreateButton[BUTTON_STATE_DISABLE]->Move(21 + SIZE_PARTY_BUTTON_X*0, nWindowPosY + 212);
//		m_pPartyCreateButton[BUTTON_STATE_DISABLE]->Render();
//		if(m_bEnablePartyMenu)
//		{			
			// ��ư : �ʴ�
//			if(m_nPartyButtonState[1] != BUTTON_STATE_NORMAL)
//			{
//				m_pPartyInviteButton[m_nPartyButtonState[1]]->Move(21 + SIZE_PARTY_BUTTON_X*1, nWindowPosY + 212);
//				m_pPartyInviteButton[m_nPartyButtonState[1]]->Render();
//			}
			// ��ư : Ż��
//			if(m_nPartyButtonState[2] != BUTTON_STATE_NORMAL)
//			{
//				m_pPartySecedeButton[m_nPartyButtonState[2]]->Move(21 + SIZE_PARTY_BUTTON_X*2, nWindowPosY + 212);
//				m_pPartySecedeButton[m_nPartyButtonState[2]]->Render();
//			}
			// ��ư : �߹�
//			if(m_nPartyButtonState[3] != BUTTON_STATE_NORMAL)
//			{
//				m_pPartyBanButton[m_nPartyButtonState[3]]->Move(21 + SIZE_PARTY_BUTTON_X*3, nWindowPosY + 212);
//				m_pPartyBanButton[m_nPartyButtonState[3]]->Render();
//			}
			// ��ư : ����
//			if(m_nPartyButtonState[4] != BUTTON_STATE_NORMAL)
//			{
//				m_pPartyTrustButton[m_nPartyButtonState[4]]->Move(21 + SIZE_PARTY_BUTTON_X*4, nWindowPosY + 212);
//				m_pPartyTrustButton[m_nPartyButtonState[4]]->Render();
//			}
//		}
//		else
//		{	
			
			// ��ư : �ʴ�
//			m_pPartyInviteButton[BUTTON_STATE_DISABLE]->Move(21 + SIZE_PARTY_BUTTON_X*1, nWindowPosY + 212);
//			m_pPartyInviteButton[BUTTON_STATE_DISABLE]->Render();
			// ��ư : Ż��
//			if(m_nPartyButtonState[2] != BUTTON_STATE_NORMAL)
//			{
//				m_pPartySecedeButton[m_nPartyButtonState[2]]->Move(21 + SIZE_PARTY_BUTTON_X*2, nWindowPosY + 212);
//				m_pPartySecedeButton[m_nPartyButtonState[2]]->Render();
//			}
//			m_pPartySecedeButton[BUTTON_STATE_DISABLE]->Move(21 + SIZE_PARTY_BUTTON_X*2, nWindowPosY + 212);
//			m_pPartySecedeButton[BUTTON_STATE_DISABLE]->Render();
			// ��ư : �߹�
//			m_pPartyBanButton[BUTTON_STATE_DISABLE]->Move(21 + SIZE_PARTY_BUTTON_X*3, nWindowPosY + 212);
//			m_pPartyBanButton[BUTTON_STATE_DISABLE]->Render();
			// ��ư : ����
//			m_pPartyTrustButton[BUTTON_STATE_DISABLE]->Move(21 + SIZE_PARTY_BUTTON_X*4, nWindowPosY + 212);
//			m_pPartyTrustButton[BUTTON_STATE_DISABLE]->Render();
//		}
//	}
//	if(g_pD3dApp->m_pShuttleChild->m_infoParty.bPartyType == _PARTYMEMBER)
//	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMEMBER)
//	{		
		// ��ư : ����
//		m_pPartyCreateButton[BUTTON_STATE_DISABLE]->Move(21 + SIZE_PARTY_BUTTON_X*0, nWindowPosY + 212);
//		m_pPartyCreateButton[BUTTON_STATE_DISABLE]->Render();
		// ��ư : �ʴ�
//		m_pPartyInviteButton[BUTTON_STATE_DISABLE]->Move(21 + SIZE_PARTY_BUTTON_X*1, nWindowPosY + 212);
//		m_pPartyInviteButton[BUTTON_STATE_DISABLE]->Render();
		// ��ư : Ż��
//		if(m_bEnablePartyMenu && m_nPartyButtonState[2] != BUTTON_STATE_NORMAL)
//		{
//			m_pPartySecedeButton[m_nPartyButtonState[2]]->Move(21 + SIZE_PARTY_BUTTON_X*2, nWindowPosY + 212);
//			m_pPartySecedeButton[m_nPartyButtonState[2]]->Render();
//		}
		// ��ư : �߹�
//		m_pPartyBanButton[BUTTON_STATE_DISABLE]->Move(21 + SIZE_PARTY_BUTTON_X*3, nWindowPosY + 212);
//		m_pPartyBanButton[BUTTON_STATE_DISABLE]->Render();
		// ��ư : ����
//		m_pPartyTrustButton[BUTTON_STATE_DISABLE]->Move(21 + SIZE_PARTY_BUTTON_X*4, nWindowPosY + 212);
//		m_pPartyTrustButton[BUTTON_STATE_DISABLE]->Render();
//	}
//	if(g_pD3dApp->m_pShuttleChild->m_infoParty.bPartyType == _NOPARTY)
//	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _NOPARTY)
//	{
		// ��ư : ����
//		if(m_nPartyButtonState[0] != BUTTON_STATE_NORMAL)
//		{
//			m_pPartyCreateButton[m_nPartyButtonState[0]]->Move(21 + SIZE_PARTY_BUTTON_X*0, nWindowPosY + 212);
//			m_pPartyCreateButton[m_nPartyButtonState[0]]->Render();
//		}
		// ��ư : �ʴ�
//		m_pPartyInviteButton[BUTTON_STATE_DISABLE]->Move(21 + SIZE_PARTY_BUTTON_X*1, nWindowPosY + 212);
//		m_pPartyInviteButton[BUTTON_STATE_DISABLE]->Render();
		// ��ư : Ż��
//		m_pPartySecedeButton[BUTTON_STATE_DISABLE]->Move(21 + SIZE_PARTY_BUTTON_X*2, nWindowPosY + 212);
//		m_pPartySecedeButton[BUTTON_STATE_DISABLE]->Render();
		// ��ư : �߹�
//		m_pPartyBanButton[BUTTON_STATE_DISABLE]->Move(21 + SIZE_PARTY_BUTTON_X*3, nWindowPosY + 212);
//		m_pPartyBanButton[BUTTON_STATE_DISABLE]->Render();
		// ��ư : ����
//		m_pPartyTrustButton[BUTTON_STATE_DISABLE]->Move(21 + SIZE_PARTY_BUTTON_X*4, nWindowPosY + 212);
//		m_pPartyTrustButton[BUTTON_STATE_DISABLE]->Render();
//	}
//	if(m_nPartySelectMember >= 0)
//	{
//		m_pForcus->Move(23, nWindowPosY + 56 + 17* m_nPartySelectMember );
//		m_pForcus->Render();
//	}
//	if(g_pD3dApp->m_pShuttleChild->m_infoParty.bPartyType != _NOPARTY)
//	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType != _NOPARTY)
//	{
//		// Member detail info
//		DWORD dwColor = GUI_FONT_COLOR;
//		int i=0;
//		char buf[128];
//		strcpy(buf, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName);
//		
//		// 2008-02-26 by bhsohn ���� �Ʒ��� ���� ���� ����
//		g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), buf);
//
//		if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)
//		{
//			strcat(buf," @");//�����
//		}
//		if(m_nPartySelectMember == i)
//		{
//			dwColor = RGB(1,1,1);
//		}
//		else
//		{
//			dwColor = GUI_FONT_COLOR;
//		}
//		m_pPartyList[i]->DrawText(PARTY_LIST_START_X,nWindowPosY+PARTY_LIST_START_Y+PARTY_LIST_HEIGHT*i,dwColor,buf,0L);
//		i++;
//
//		vector<PARTYENEMYINFO*>::iterator itParty = g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.begin();
//		while(itParty != g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.end())
//		{
//			strcpy(buf, (*itParty)->m_ImPartyMemberInfo.CharacterName);
//			// 2008-02-26 by bhsohn ���� �Ʒ��� ���� ���� ����
//			g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), buf);
//
//			switch((*itParty)->m_bPartyType)
//				{
//				case _PARTYMASTER:
//					{
//						strcat(buf," @");
//						if(m_nPartySelectMember == i)
//						{
//							dwColor = GUI_SELECT_FONT_COLOR;
//						}
//						else
//						{
//							dwColor = GUI_FONT_COLOR;
//						}
//						m_pPartyList[i]->DrawText(PARTY_LIST_START_X,nWindowPosY+PARTY_LIST_START_Y+PARTY_LIST_HEIGHT*i,dwColor,buf,0L);
//					}
//					break;
//				case _PARTYMEMBER:
//					{
//						if(m_nPartySelectMember == i)
//						{
//							dwColor = GUI_SELECT_FONT_COLOR;
//						}
//						else
//						{
//							dwColor = GUI_FONT_COLOR;
//						}
//						m_pPartyList[i]->DrawText(PARTY_LIST_START_X,nWindowPosY+PARTY_LIST_START_Y+PARTY_LIST_HEIGHT*i,dwColor,buf,0L);
//					}
//					break;
//				case _PARTYOTHERMAPMASTER:
//					{
//						strcat(buf," @");
//						if(m_nPartySelectMember == i)
//						{
//							dwColor = RGB(128,128,128);
//						}
//						else
//						{
//							dwColor = RGB(192,192,192);
//						}
//						m_pPartyList[i]->DrawText(PARTY_LIST_START_X,nWindowPosY+PARTY_LIST_START_Y+PARTY_LIST_HEIGHT*i,dwColor,buf,0L);
//					}
//					break;
//				case _PARTYOTHERMAPMEMBER:
//					{
//						if(m_nPartySelectMember == i)
//						{
//							dwColor = RGB(128,128,128);
//						}
//						else
//						{
//							dwColor = RGB(192,192,192);
//						}
//						m_pPartyList[i]->DrawText(PARTY_LIST_START_X,nWindowPosY+PARTY_LIST_START_Y+PARTY_LIST_HEIGHT*i,dwColor,buf,0L);
//					}
//					break;
//				case _PARTYRESTARTMEMBER:
//					{
//						if(m_nPartySelectMember == i)
//						{
//							dwColor = RGB(64,64,64);
//						}
//						else
//						{
//							dwColor = RGB(128,128,128);
//						}
//						m_pPartyList[i]->DrawText(PARTY_LIST_START_X,nWindowPosY+PARTY_LIST_START_Y+PARTY_LIST_HEIGHT*i,dwColor,buf,0L);
//					}
//					break;
//			default:
//				{
//					if(m_nPartySelectMember == i)
//					{
//						dwColor = RGB(128,128,128);
//					}
//					else
//					{
//						dwColor = RGB(192,192,192);
//					}
//					m_pPartyList[i]->DrawText(PARTY_LIST_START_X,nWindowPosY+PARTY_LIST_START_Y+PARTY_LIST_HEIGHT*i,dwColor,buf,0L);	
//				}
//				break;
//				}
//				i++;
//			itParty++;
//		}
//		// formation info
//		if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)// show combo box
//		{
////			m_pCaptainCombo->Move(PARTY_COMBO_START_X, nWindowPosY+PARTY_COMBO_START_Y);
////			m_pCaptainCombo->Render();
//			// show current formation
////			m_pPartyFormation[g_pShuttleChild->m_pClientParty->GetPartyInfo().bFormationFlyingType]->DrawText(PARTY_COMBO_X, nWindowPosY+PARTY_COMBO_Y,
////				GUI_FONT_COLOR,m_strPartyFormation[g_pShuttleChild->m_pClientParty->GetPartyInfo().bFormationFlyingType],0L);
////			if(m_bShowFormationList)
////			{
////				for(int i=0;i<PARTY_FORMATION_NUMBER;i++)
////				{
////					m_pCombo->Move(PARTY_COMBO_X,nWindowPosY+PARTY_COMBO_Y+PARTY_COMBO_HEIGHT*(i+1));
////					m_pCombo->Render();
////					m_pPartyFormation[i]->DrawText(PARTY_COMBO_X, nWindowPosY+PARTY_COMBO_Y+PARTY_COMBO_HEIGHT*(i+1), 
////						GUI_FONT_COLOR,m_strPartyFormation[i],0L);
////				}
////			}
//
//			// ���� ��ȭ ���̱�
////			m_pPartyVOIPButton[m_nVOIPButtonState]->Move(PARTY_VOIP_X, nWindowPosY+PARTY_VOIP_Y);
////			m_pPartyVOIPButton[m_nVOIPButtonState]->Render();
//		}
//		else 
//		{
////			m_pJoinFormationButton[m_nJoinFormationButtonState]->Move(PARTY_INFO_BUTTON_X, nWindowPosY+PARTY_COMBO_START_Y);
////			m_pJoinFormationButton[m_nJoinFormationButtonState]->Render();
////			m_pFormationInfo->Move(PARTY_INFO_START_X2, nWindowPosY+PARTY_COMBO_START_Y);
////			m_pFormationInfo->Render();
////			m_pPartyFormation[g_pShuttleChild->m_pClientParty->GetPartyInfo().bFormationFlyingType]->DrawText(
////				PARTY_INFO_START_X2, nWindowPosY+PARTY_COMBO_START_Y,
////				GUI_FONT_COLOR,m_strPartyFormation[g_pShuttleChild->m_pClientParty->GetPartyInfo().bFormationFlyingType],0L);
//
//			// ���� ��ȭ ��Ȱ��ȭ
////			m_pPartyVOIPButton[PARTY_VOIP_BUTTON_STATE_DISABLE]->Move(PARTY_VOIP_X, nWindowPosY+PARTY_VOIP_Y);
////			m_pPartyVOIPButton[PARTY_VOIP_BUTTON_STATE_DISABLE]->Render();
//		}
//	} 
/*	else
	{
		m_pJoinFormationButton[m_nJoinFormationButtonState]->Move(PARTY_INFO_BUTTON_X, nWindowPosY+PARTY_COMBO_START_Y);
		m_pJoinFormationButton[m_nJoinFormationButtonState]->Render();
		m_pFormationInfo->Move(PARTY_INFO_START_X2, nWindowPosY+PARTY_COMBO_START_Y);
		m_pFormationInfo->Render();
//		m_pPartyFormation[g_pD3dApp->m_pShuttleChild->m_infoParty.bFormationFlyingType]->DrawText(
//			PARTY_INFO_START_X2, nWindowPosY+PARTY_COMBO_START_Y,
//			GUI_FONT_COLOR,m_strPartyFormation[g_pD3dApp->m_pShuttleChild->m_infoParty.bFormationFlyingType],0L);
	}
*/
	
}
/*
void CINFCommunityParty::RenderFriend()
{
	FLOG( "CINFCommunityParty::RenderFriend()" );
	int nWindowPosY = g_pGameMain->m_nLeftWindowY;
//	int nWindowPosY = POS_LEFT_WINDOW_Y;
//	if(nWindowPosY < SIZE_ROLL_Y )
//		nWindowPosY = SIZE_ROLL_Y;
//	if(nWindowPosY > LEFT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y)
//		nWindowPosY = LEFT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y;
	if(m_pFriend)
	{
		m_pFriend->Move(13,nWindowPosY + 30);
		m_pFriend->Render() ;
	}

}

void CINFCommunityParty::RenderGuild()
{
	FLOG( "CINFCommunityParty::RenderGuild()" );
	int nWindowPosY = g_pGameMain->m_nLeftWindowY;
//	int nWindowPosY = POS_LEFT_WINDOW_Y;
//	if(nWindowPosY < SIZE_ROLL_Y )
//		nWindowPosY = SIZE_ROLL_Y;
//	if(nWindowPosY > LEFT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y)
//		nWindowPosY = LEFT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y;
	if(m_pGuild)
	{
		m_pGuild->Move(13,nWindowPosY + 30);
		m_pGuild->Render() ;
	}

}

void CINFCommunityParty::RenderReject()
{
	FLOG( "CINFCommunityParty::RenderReject()" );
	int nWindowPosY = g_pGameMain->m_nLeftWindowY;
//	int nWindowPosY = POS_LEFT_WINDOW_Y;
//	if(nWindowPosY < SIZE_ROLL_Y )
//		nWindowPosY = SIZE_ROLL_Y;
//	if(nWindowPosY > LEFT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y)
//		nWindowPosY = LEFT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y;
	if(m_pReject)
	{
		m_pReject->Move(13,nWindowPosY + 30);
		m_pReject->Render() ;
	}

}

void CINFCommunityParty::Render()
{
	FLOG( "CINFCommunityParty::Render()" );
	int nWindowPosY = g_pGameMain->m_nLeftWindowY;
	if(m_pLogo)
	{
		m_pLogo->Move(8, nWindowPosY + 5);
		m_pLogo->Render();
	}
	switch(m_nCommunityType)
	{
	case COMMUNITY_PARTY:
		{
			RenderParty();
		}
		break;
	case COMMUNITY_FRIEND:
		{
			RenderFriend();
		}
		break;
	case COMMUNITY_GUILD:
		{
			RenderGuild();
		}
		break;
	case COMMUNITY_REJECT:
		{
			RenderReject();
		}
		break;
	}
	
}

int CINFCommunityParty::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFCommunityParty::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt = MAKEPOINT(lParam);
			int nWindowPosY = g_pGameMain->m_nLeftWindowY;
			if(pt.y>nWindowPosY + 30 && pt.y<nWindowPosY + 44)
			{
				if(pt.x>13 && pt.x<58 )
				{
					m_nCommunityType = COMMUNITY_PARTY;
				}
				if(pt.x>59 && pt.x<104 )
				{
					m_nCommunityType = COMMUNITY_FRIEND;
				}
				if(pt.x>105 && pt.x<150 )
				{
					m_nCommunityType = COMMUNITY_GUILD;
				}
				if(pt.x>151 && pt.x<203 )
				{
					m_nCommunityType = COMMUNITY_REJECT;
				}
			}
			if(g_pD3dApp->m_pShuttleChild->m_infoParty.bPartyType == _PARTYMASTER && 
				pt.x>PARTY_COMBO_X && pt.x<PARTY_COMBO_X+PARTY_COMBO_WIDTH+14 && 
				pt.y>nWindowPosY+PARTY_COMBO_Y && pt.y<nWindowPosY+PARTY_COMBO_Y+PARTY_COMBO_HEIGHT)
			{
				m_bShowFormationList = !m_bShowFormationList;
			}
			// ��� ���� ����
			if(g_pD3dApp->m_pShuttleChild->m_infoParty.bPartyType == _PARTYMASTER && m_bShowFormationList &&
				pt.x>PARTY_COMBO_X && pt.x<PARTY_COMBO_X+PARTY_COMBO_WIDTH )
			{
				//pt.y>nWindowPosY+PARTY_COMBO_Y+PARTY_COMBO_WIDTH*i && pt.y<nWindowPosY+PARTY_COMBO_Y+PARTY_COMBO_WIDTH*(i+1)
				int i = (pt.y-nWindowPosY-PARTY_COMBO_Y)/PARTY_COMBO_HEIGHT-1;
				if(i>=0 && i<PARTY_FORMATION_NUMBER)
				{
					ChagePartyFormation(i);
					m_bShowFormationList = FALSE;
				}
			}

		}
		break;
	case WM_LBUTTONUP:
		{
		}
		break;
	}
	switch(m_nCommunityType)
	{
	case COMMUNITY_PARTY:
		{
			PartyWndProc(uMsg, wParam, lParam);
		}
		break;
	case COMMUNITY_FRIEND:
		{
			FriendWndProc(uMsg, wParam, lParam);
		}
		break;
	case COMMUNITY_GUILD:
		{
			GuildWndProc(uMsg, wParam, lParam);
		}
		break;
	case COMMUNITY_REJECT:
		{
			RejectWndProc(uMsg, wParam, lParam);
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}
*/

int CINFCommunityParty::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFCommunityParty::PartyWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);			
			int nCnt = 0;
			POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			int nCheckX = ptBkPos.x + COMMU_BK_BTN_START.x;
			int nCheckY = ptBkPos.y + COMMU_BK_BTN_START.y;
			
			if(pt.y>nCheckY && pt.y<nCheckY + COMMU_BK_START_SIZE.y)
			{
				for(nCnt = 0;nCnt < MAX_WND_PARTYMODE;nCnt++)
				{
					nCheckX = ptBkPos.x + COMMU_BK_BTN_START.x + (nCnt*( COMMU_BK_START_SIZE.x + COMMU_BK_BTN_GAP) );
					if(pt.x>=(nCheckX) && pt.x<(nCheckX+COMMU_BK_START_SIZE.x))
					{
						ChangePartySubMode(nCnt);
						break;						
					}
				}
			}	  
#else						
			int nCheckX = ptBkPos.x + COMMU_BK_START_X;
			int nCheckY = ptBkPos.y + COMMU_BK_START_Y;
			
			if(pt.y>nCheckY && pt.y<nCheckY + 44)
			{
				for(nCnt = 0;nCnt < MAX_WND_PARTYMODE;nCnt++)
				{
					nCheckX = ptBkPos.x + COMMU_BK_START_X + (nCnt*COMMU_BK_START_WIDTH);
					if(pt.x>=(nCheckX) && pt.x<(nCheckX+COMMU_BK_START_WIDTH))
					{
						ChangePartySubMode(nCnt);
						break;						
					}
				}
			}

#endif
		}
		break;
	}
	switch(m_nSubWndMode)
	{
	case WND_PARTYMODE_OPTION:
		{
			return WndProcOption(uMsg, wParam, lParam);
		}
		break;
	case WND_PARTYMODE_SEARCH:
		{
			return WndProcSearch(uMsg, wParam, lParam);
		}
		break;
	}		
	return INF_MSGPROC_NORMAL;
}
int CINFCommunityParty::WndProcOption(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);			
			{
				if(TRUE == m_pOJoinBtn->OnLButtonDown(pt))
				{
					// ��ư���� ���콺�� �ִ�.
					return  INF_MSGPROC_BREAK;
				}		
			}
			
			{
				if(TRUE == m_pOLeaveBtn->OnLButtonDown(pt))
				{
					// ��ư���� ���콺�� �ִ�.
					return  INF_MSGPROC_BREAK;
				}		
			}
			
			{
				if(TRUE == m_pOPurgeBtn->OnLButtonDown(pt))
				{
					// ��ư���� ���콺�� �ִ�.
					return  INF_MSGPROC_BREAK;
				}		
			}
			
			{
				if(TRUE == m_pOChargeBtn->OnLButtonDown(pt))
				{
					// ��ư���� ���콺�� �ִ�.
					return  INF_MSGPROC_BREAK;
				}		
			}
						
			if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType != _PARTYMASTER)
			{
				// ������� �ƴϸ� �ƹ� ó���� ����
				return INF_MSGPROC_NORMAL;

			}
			{
				if(TRUE == m_pOOkBtn->OnLButtonDown(pt))
				{
					// ��ư���� ���콺�� �ִ�.
					return  INF_MSGPROC_BREAK;
				}		
			}
			{
				if(TRUE == m_pOPartyBattle->OnLButtonDown(pt))
				{
					// ��ư���� ���콺�� �ִ�.
					return  INF_MSGPROC_BREAK;
				}		
			}

			// ����� �ʴ� ����� ��� ��ȣ�� ����.	
			if(RADIO_JOINT_FREE == m_pSPartyInfoSetup.PartyJoinType)	
			{
				// ���� ��ĸ� Ȱ��ȭ
				if(TRUE == m_pINFSecuEditBox->OnLButtonDown(pt))
				{	
					AllEditBoxDisable();
					m_pINFSecuEditBox->EnableEdit(TRUE);				
					
					// ��ư���� ���콺�� �ִ�.
					return  INF_MSGPROC_BREAK;
				}
			}
			{
				if(TRUE == m_pNumMinEditBox->OnLButtonDown(pt))
				{
					AllEditBoxDisable();
					m_pNumMinEditBox->EnableEdit(TRUE);

					// ��ư���� ���콺�� �ִ�.
					return  INF_MSGPROC_BREAK;
				}
			}
			
			{
				if(TRUE == m_pNumMaxEditBox->OnLButtonDown(pt))
				{
					AllEditBoxDisable();
					m_pNumMaxEditBox->EnableEdit(TRUE);
					
					// ��ư���� ���콺�� �ִ�.
					return  INF_MSGPROC_BREAK;
				}
			}
			{
				if(TRUE == m_pEditPartyName->OnLButtonDown(pt))
				{				
					AllEditBoxDisable();
					m_pEditPartyName->EnableEdit(TRUE, TRUE);
					return INF_MSGPROC_BREAK;
				}
			}
			int nCnt = 0;
			{					
				for(nCnt = 0;nCnt <MAX_RADIO_JOINT;nCnt++)
				{					
					if(TRUE == m_pPartyJoint[nCnt]->OnLButtonDown(pt))
					{						
						OnClickJoint(nCnt);
						return  INF_MSGPROC_BREAK;
					}		
				}
				/*
				for(nCnt = 0;nCnt < MAX_RADIO_AUDIO;nCnt++)
				{					
					if(TRUE == m_pAudioOp[nCnt]->OnLButtonDown(pt))
					{
						OnClickAudioOp(nCnt);
						return  INF_MSGPROC_BREAK;
					}		
				}
				*/
				// 2008-10-20 by bhsohn VOIP ���� ����
				// �������� ����
//				if(pt.x > VOICECHAT_SPK_POS_X + m_nPosVolum &&
//					pt.x < VOICECHAT_SPK_POS_X + m_nPosVolum + m_pImgSpkVolBar->GetImgSize().x &&
//					pt.y > VOICECHAT_SPK_POS_Y &&
//					pt.y < VOICECHAT_SPK_POS_Y + m_pImgSpkVolBar->GetImgSize().y)
//				{
//					m_bVolumCon = TRUE;
//					m_ptOldVolumPos = pt;
//				}


			}
			
			{
				// �޺�
				int nGraphicOptionSel = -1;
				int nLBtnDown = m_pComboForMation->LButtonDown(pt);
				if(1 == nLBtnDown)				
				{
//					int nCursel = m_pComboForMation->GetSelect();
//					ChagePartyFormation(nCursel);
					// ���̴ٰ� �Ⱥ��̴� ��Ȳ			
					return  INF_MSGPROC_BREAK;
				}
			}
			{
				POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();

				int nPosX, nPosY;
				int nSelItem =-1;
				for(int nCnt =0;nCnt <MAX_PARTY_LEN;nCnt++)
				{
					nPosX = PARTYOP_LIST_START.x+ptBkPos.x;
					nPosY = PARTYOP_LIST_START.y+ptBkPos.y + (nCnt*PARTYOP_LIST_HEIGHT);
					if((pt.x >= nPosX && (pt.x <= nPosX+COMMUNITY_OPBK_WIDTH))
						&& (pt.y >= nPosY && (pt.y <= nPosY+PARTYOP_LIST_HEIGHT)))
					{
						nSelItem = nCnt;
						break;
					}
				}
				int nMaxSize = (g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.size()+1);
				if(nSelItem < nMaxSize)
				{
					m_nPartyOPSel = nSelItem;
				}
				else
				{
					m_nPartyOPSel = -1;
				}
			}
			int nGraphicOptionSel = -1;
			{
				for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
				{
					int nLBtnDown = m_pComboDivision[nCnt]->LButtonDown(pt);
					if(-1 != nLBtnDown)				
					{
						// �Ⱥ��̴ٰ� ���̴� ��Ȳ
						nGraphicOptionSel = nCnt;
						break;
					}
				}
				// ���õȰŸ� �����Ѱ� ��ü�� ��������
				if(nGraphicOptionSel != -1)
				{
					for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
					{
						if(nCnt != nGraphicOptionSel)
						{
							m_pComboDivision[nCnt]->ShowItem(FALSE);					
						}
					}
					UpdateUIPos();
					return  INF_MSGPROC_BREAK;
				}			
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

			m_pOJoinBtn->OnMouseMove(pt);
			m_pOLeaveBtn->OnMouseMove(pt);
			m_pOPurgeBtn->OnMouseMove(pt);
			m_pOChargeBtn->OnMouseMove(pt);			
			m_pOOkBtn->OnMouseMove(pt);	
			m_pOPartyBattle->OnMouseMove(pt);	
			for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
			{
				m_pComboDivision[nCnt]->MouseMove(pt);
			}	
			m_pComboForMation->MouseMove(pt);	
			if(m_bVolumCon)
			{// ���� ũ�� ��Ʈ����.
				m_nPosVolum += (pt.x - m_ptOldVolumPos.x);
				if(m_nPosVolum > VOICECHAT_SPK_W)
				{
					m_nPosVolum = VOICECHAT_SPK_W;
				}else if(m_nPosVolum < 0)
				{
					m_nPosVolum = 0;
				}
				DWORD nVolum;
				nVolum = (DWORD)(m_nPosVolum/VOICECHAT_SPK_W)*100;
				m_ptOldVolumPos = pt;
				SetVoiceChatVolum(nVolum);

			}

		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			{
				if(TRUE == m_pOJoinBtn->OnLButtonUp(pt))
				{		
					// 2009-01-21 by bhsohn Editâ ���� ó��
					AllEditBoxDisable();

					((CINFCommunity*)m_pParent)->OnPopupInviteParty();		// ��Ƽ �ʴ�
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;
				}
			}
			
			{
				if(TRUE == m_pOLeaveBtn->OnLButtonUp(pt))
				{				
					((CINFCommunity*)m_pParent)->OnClickSecedeParty();	// ��ƼŻ��
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}
			
			{
				if(TRUE == m_pOPurgeBtn->OnLButtonUp(pt))
				{				
					PartyQuestionBanUser();//�߹��� �����.
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}
			
			{
				if(TRUE == m_pOChargeBtn->OnLButtonUp(pt))
				{				
					PartyQuestionTransferMaster(); // ������ �����.
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}			
			{
				if(TRUE == m_pOOkBtn->OnLButtonUp(pt))
				{				
					OnClickOkBtn();
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}	
			{
				if(TRUE == m_pOPartyBattle->OnLButtonUp(pt))
				{				
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

						break;
					}
					// end 2008-02-27 by bhsohn ���� �Ʒ��� ����
					// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
					if(!g_pD3dApp->IsOptionEtc(OPTION_RADIO_FORMATIONWAR))
					{
						break;
					}
					// end 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
					g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_080917_0102, _Q_PARTY_BATTLE);
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}	
			/*//warrior
			if(m_bVolumCon)
			{// ���� ũ�� ��Ʈ����.				
				if(m_nPosVolum > VOICECHAT_SPK_W)
				{
					m_nPosVolum = VOICECHAT_SPK_W;
				}else if(m_nPosVolum < 0)
				{
					m_nPosVolum = 0;
				}
				DWORD nVolum;
				nVolum = (DWORD)(m_nPosVolum/VOICECHAT_SPK_W)*100;				
				// 2008-10-20 by bhsohn VOIP ���� ����
				// �������� ����
				//g_pGameMain->SetVoiceVolum(nVolum);				
				m_bVolumCon = FALSE;
			}*/

		}
		break;
	case WM_IME_STARTCOMPOSITION:
//	case WM_IME_NOTIFY:
	case WM_IME_COMPOSITION:
	case WM_INPUTLANGCHANGE:	
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_SETCONTEXT:	
	case WM_CHAR:	
		{
			// 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�			
// 			if(!m_pEditPartyName->IsEditMode())
// 			{
// 				return INF_MSGPROC_NORMAL;		
// 			}			
// 						
// 			int nLienCnt  =-1;
// 			BOOL bRtn = m_pEditPartyName->WndProc(uMsg, wParam, lParam, &nLienCnt, TRUE);
// 			if(nLienCnt < 0)
// 			{
// 				// �ƹ� ó���� ���ߴ�.
// 				return INF_MSGPROC_NORMAL;
// 			}
//			return INF_MSGPROC_BREAK;
			if(INF_MSGPROC_BREAK == WndProcEdit(uMsg, wParam, lParam))
			{
				return INF_MSGPROC_BREAK;
			}
			// end 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
			
		}
		break;
	case WM_KEYDOWN:
		{
			// ����� �ʴ� ����� ��� ��ȣ�� ����.	
			if(RADIO_JOINT_FREE == m_pSPartyInfoSetup.PartyJoinType)	
			{
				if(m_pINFSecuEditBox->WndProc(uMsg, wParam, lParam))
				{
					return INF_MSGPROC_BREAK;
				}
			}
			
			if(m_pNumMinEditBox->WndProc(uMsg, wParam, lParam))
			{				
				return INF_MSGPROC_BREAK;
			}
			if(m_pNumMaxEditBox->WndProc(uMsg, wParam, lParam))
			{				
				return INF_MSGPROC_BREAK;
			}
			// 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
			if(INF_MSGPROC_BREAK == WndProcEdit(uMsg, wParam, lParam))
			{
				return INF_MSGPROC_BREAK;
			}
			// end 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

int CINFCommunityParty::WndProcSearch(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFCommunityParty::PartyWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	switch(uMsg)
	{	
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â			
			{
				// ����
				if(TRUE == m_pSCrateBtn->OnLButtonDown(pt))
				{
					// ��ư���� ���콺�� �ִ�.
					return  INF_MSGPROC_BREAK;
				}		
			}
			
			{
				// ����
				if(TRUE == m_pSJoinBtn->OnLButtonDown(pt))
				{
					// ��ư���� ���콺�� �ִ�.
					return  INF_MSGPROC_BREAK;
				}		
			}
			
			{
				// ����
				if(TRUE == m_pSRefreshBtn->OnLButtonDown(pt))
				{
					// ��ư���� ���콺�� �ִ�.
					return  INF_MSGPROC_BREAK;
				}		
			}			
			{
				BOOL bClick = m_pScrollPartyList->IsMouseBallPos(pt);
				if(bClick)
				{
					m_pScrollPartyList->SetMouseMoveMode(TRUE);
					return INF_MSGPROC_BREAK;
				}
			}
			{
				POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();

				int nPosX, nPosY;
				int nSelItem =-1;
				for(int nCnt =0;nCnt <MAX_PARTY_LIST_LINE; nCnt++)
				{
					nPosX = SEL_IMAGE_POS.x+ptBkPos.x;
					nPosY = SEL_IMAGE_POS.y+ptBkPos.y + (nCnt*SEL_IMAGE_POS_CAP_Y);
					if((pt.x >= nPosX && (pt.x <= nPosX+COMMU_BK_SEARCH_WIDTH))
						&& (pt.y >= nPosY && (pt.y <= nPosY+SEL_IMAGE_POS_CAP_Y)))
					{
						nSelItem = nCnt;
						break;
					}
				}
				nSelItem += m_pScrollPartyList->GetScrollStep();
				int nMaxSize = (int)m_vecPartyListInfo.size();
				if(nSelItem < nMaxSize)
				{
					m_nSelParty = nSelItem;
				}
				else
				{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
					m_nSelParty = nMaxSize;														  
#else		
					m_nSelParty = -1;
#endif
				}
			}
			
			// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â			

		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â			
			m_pSCrateBtn->OnMouseMove(pt);	// ����
			m_pSJoinBtn->OnMouseMove(pt);	// ����
			m_pSRefreshBtn->OnMouseMove(pt);	// ���ΰ�ħ
			{
				if(m_pScrollPartyList->GetMouseMoveMode())
				{
					if(FALSE == m_pScrollPartyList->IsMouseScrollPos(pt))
					{
						m_pScrollPartyList->SetMouseMoveMode(FALSE);
					}
					else
					{
						m_pScrollPartyList->SetScrollPos(pt);
						CheckPartyListInfo(); // ��û

						return INF_MSGPROC_BREAK;
					}
				}
			}
			// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â			
			{
				// ����
				if(TRUE == m_pSCrateBtn->OnLButtonUp(pt))
				{				
					((CINFCommunity*)m_pParent)->OnPopupCreateParty();
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;					
				}
			}			
			{
				// ����
				if(TRUE == m_pSJoinBtn->OnLButtonUp(pt))
				{				
					OnClickJoinBtn();
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;					
				}
			}			
			{
				// ���ΰ�ħ
				if(TRUE == m_pSRefreshBtn->OnLButtonUp(pt))
				{	
					OnClickRefresh();					
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;					
				}
			}
			{
				BOOL bClick = m_pScrollPartyList->GetMouseMoveMode();
				if(bClick)
				{
					m_pScrollPartyList->SetMouseMoveMode(FALSE);
					return INF_MSGPROC_BREAK;
				}				
			}

			// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
		}
		break;
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);			
			BOOL bClick = FALSE;			
			
			{
				bClick = m_pScrollPartyList->IsMouseWhellPos(pt);
				if(bClick)		
				{			
					m_pScrollPartyList->OnMouseWheel(wParam, lParam);					
					CheckPartyListInfo(); // ��û
					return INF_MSGPROC_BREAK;
				}
				
			}
		}
		break;		
	}	
	return INF_MSGPROC_NORMAL;

}

void CINFCommunityParty::OnButtonClicked(int nButton)
{
	FLOG( "CINFCommunityParty::OnButtonClicked(int nButton)" );
	switch(nButton)
	{
	case 0://����
		{
				if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _NOPARTY &&
					!g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_PARTY_CREATE))
				{
					g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_PARTY_0023, _Q_PARTY_CREATE);//"�������� �غ��Ͻðڽ��ϱ�?"
				}
			}
		break;
	case 1://�ʴ�
		{
				if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER &&
					!g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_PARTY_INVITE_USER))
				{
					g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_PARTY_0024, _Q_PARTY_INVITE_USER);//"�ʴ��� ������ �Է��Ͻÿ�."
				}
			}
		break;
	case 2://Ż��
		{
			if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType != _NOPARTY &&
				!g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_PARTY_INVITE_USER))
			{
				g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_PARTY_0025, _Q_PARTY_SECEDE);//"��뿡�� Ż���Ͻðڽ��ϱ�?"
			}
		}
		break;
	case 3://�߹�
		{
				PartyQuestionBanUser();
			}
		break;
	case 4://����
		{
				PartyQuestionTransferMaster();
			}
		break;
	case 5://��� ���� ����
		{
			if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bFormationFlyingType && 
				!g_pShuttleChild->m_pClientParty->GetPartyInfo().bFormationFlyingPositionType
				&& g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMEMBER)
			{
				vector<PARTYENEMYINFO*>::iterator itParty = g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.begin();
				while(itParty != g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.end())
				{
					if(	(*itParty)->m_pEnemyData &&
						(*itParty)->m_bPartyType == _PARTYMASTER &&
						D3DXVec3Length(&((*itParty)->m_pEnemyData->m_vPos - g_pShuttleChild->m_vPos)) < 300.0f)
					{
						g_pShuttleChild->m_pClientParty->ISendPartyGetFlightPosition();

						// 2005-10-24 by ispark
						// ������ ��û�� �� ��� ���� Ǭ��.
						g_pShuttleChild->m_bUnitStop = FALSE;
						g_pShuttleChild->m_nBooster = BOOSTER_TYPE_MIN;
						g_pShuttleChild->m_bPenaltyUnitStop = FALSE;
						g_pShuttleChild->m_bMouseLock = FALSE;
						g_pShuttleChild->m_bMouseMoveLock = FALSE;
						g_pD3dApp->m_bFixUnitDirMode = FALSE;
						g_pGameMain->InitShowWindow();
						// 2010. 03. 30 by jskim ���� ���� �ٵ������ ���� ����	
						g_pShuttleChild->SendFieldSocketChangeBodyCondition( g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_SET_OR_CLEAR_MASK );
						//end 2010. 03. 30 by jskim ���� ���� �ٵ������ ���� ����

//				CMapEnemyIterator itEnemy = g_pD3dApp->m_pScene->m_mapEnemyList.begin();
//				while(itEnemy != g_pD3dApp->m_pScene->m_mapEnemyList.end())
//				{
//					if(itEnemy->second->m_infoParty.bPartyType == _PARTYMASTER && D3DXVec3Length(&(itEnemy->second->m_vPos - g_pD3dApp->m_pShuttleChild->m_vPos)) < 300.0f)
//					{
//						MSG_IC_PARTY_GET_FLIGHT_POSITION sMsg;
//						memset(&sMsg,0x00,sizeof(sMsg));
//						char buffer[SIZE_MAX_PACKET];
//						int nType = T_IC_PARTY_GET_FLIGHT_POSITION;
//						sMsg.CharacterUniqueNumber = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
//						memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//						memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//						g_pD3dApp->m_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
//						g_pD3dApp->m_pShuttleChild->m_infoParty.bFormationFlyingPositionType = 5;
//						g_pD3dApp->m_pShuttleChild->m_bFormationFlight = TRUE;
						break;
					}
//					itEnemy++;
					itParty++;
				}
			}
		}
		break;
	case 6://���� ä��
		{
			/*//warrior
			if(g_pD3dApp->m_VOIPState.nVOIPState == _VOIP_NOTLOGIN)
			{
				g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_VOIP_0014, _Q_PARTYMASTER_VOIP);//"��밣 ��ȭ�� �Ͻðڽ��ϱ�?"
				g_pD3dApp->m_VOIPState.bCaller = TRUE;
				g_pD3dApp->m_VOIPState.nVOIPState = _VOIP_TRY_REGISTER;
				// 2005-10-07 by ispark �����ȭ ��ü�� ����
				g_pD3dApp->m_VOIPState.nConnectCount = 50;
			}
			else
			{
				if( g_pD3dApp->m_VOIPState.nVOIPState != _VOIP_NOTLOGIN &&
					g_pShuttleChild->m_pClientParty->GetNtoNCallState() == TRUE &&
					g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)
				{
					g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_VOIP_0015, _Q_PARTY_VOIP_END);//"��밣 ��ȭ�� ���� �Ͻðڽ��ϱ�?"
				}
				else
				{
					if(g_pD3dApp->m_VOIPState.nCallState == _CALLSTATE_1TO1)
					{
						g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_VOIP_0011, _Q_VOIP_USING);//"1:1 ������ȭ�� �ϰ� �ֽ��ϴ�"
					}
					else if(g_pD3dApp->m_VOIPState.nCallState == _CALLSTATE_GUILD)
					{
						g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_VOIP_0013, _Q_VOIP_USING);//"���� ������ȭ�� �ϰ� �ֽ��ϴ�"
					}
					else if(g_pD3dApp->m_VOIPState.nCallState == _CALLSTATE_PARTY)
					{
						g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_VOIP_0012, _Q_VOIP_USING);
					}
				}
			}*/
		}
		break;
	}
}
// 2008-06-03 by bhsohn EP3 ��� ���� ó��
void CINFCommunityParty::PartySendCreate() // ��� ���� ���������� ������.
{	
	FLOG( "CINFCommunityParty::PartySendCreate()" );
	
	// �ɼ�â���� ����
	SPARTY_INFO* pSPartyInfo = g_pGameMain->m_pCommunity->GetPartyCreate()->GetSPartyCreateInfo();
	MSG_IC_PARTY_CREATE sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	int nType = T_IC_PARTY_CREATE;
	
	sMsg.CharacterUniqueNumber = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
	memcpy(&sMsg.PartyInfo, pSPartyInfo, sizeof(SPARTY_INFO));

	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pD3dApp->m_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
}

BOOL CINFCommunityParty::PartyQuestionInviteUser(CINFMessageBox* pMsgBox, int x, int y)//������ ��Ȯ�� ���� ��� return TRUE
{
	FLOG( "CINFCommunityParty::PartyQuestionInviteUser(CINFMessageBox* pMsgBox, int x, int y)" );
	D3DXVECTOR2 vPos1 = D3DXVECTOR2(x,y);
	float fLength = 100.0f;
	CEnemyData * pEnemy = NULL;
	CMapEnemyIterator itEnemy = g_pD3dApp->m_pScene->m_mapEnemyList.begin();
	while(itEnemy != g_pD3dApp->m_pScene->m_mapEnemyList.end())
	{
		if(	itEnemy->second->m_nPartyID == 0 && 
			itEnemy->second->m_nObjScreenW > 0)
		{
			D3DXVECTOR2 vPos2 = D3DXVECTOR2(itEnemy->second->m_nObjScreenX,itEnemy->second->m_nObjScreenY);
			float fLengthTemp = D3DXVec2Length(&(vPos1-vPos2));
			if(fLengthTemp < 50.0f && fLengthTemp < fLength)
			{
				fLength = fLengthTemp;
				pEnemy = itEnemy->second;
			}
		}
		itEnemy++;
	}
	if(pEnemy)
	{
		strcpy(pMsgBox->m_strInputMessage,pEnemy->m_infoCharacter.CharacterInfo.CharacterName);
		strcat(pMsgBox->m_strInputMessage, "_");
/*		strcpy(m_strInviteUser, pParty->m_infoCharacter.CharacterName);
		char buf[128];
		wsprintf(buf, "%s(��)�� �ʴ��Ͻðڽ��ϱ�?",pParty->m_infoCharacter.CharacterName);
		g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_PARTY_INVITE_USER_OK);
*/		return TRUE;
	}

	return FALSE;
}

void CINFCommunityParty::PartySendInviteUser()// ��� �ʴ� ���������� ������.
{
	FLOG( "CINFCommunityParty::PartySendInviteUser()" );
	if(!m_strInviteUser[0])
		return;

	// 2008-06-03 by bhsohn EP3 ��� ���� ó��
//	CEnemyData * pParty = NULL;
////	CEnemyData * pEnemy = NULL;
//	CMapEnemyIterator itEnemy = g_pD3dApp->m_pScene->m_mapEnemyList.begin();
//	while(itEnemy != g_pD3dApp->m_pScene->m_mapEnemyList.end())
//	{
//		// 2006-05-22 by ispark, strcmp -> stricmp
//		if(	itEnemy->second->m_nPartyID == 0 &&
//			!stricmp(itEnemy->second->m_infoCharacter.CharacterInfo.CharacterName,m_strInviteUser) )
//		{
//			pParty = itEnemy->second;
//		}
//		itEnemy++;
//	}
//	if(pParty)
	{
		MSG_FC_PARTY_REQUEST_INVITE sMsg;
		memset(&sMsg,0x00,sizeof(sMsg));
		char buffer[SIZE_MAX_PACKET];
		strcpy(sMsg.CharacterName,m_strInviteUser);
		int nType = T_FC_PARTY_REQUEST_INVITE;
		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
		g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
		m_strInviteUser[0] = 0;
	}
}

void CINFCommunityParty::PartyQuestionJoin(char* strMasterName, PartyID_t nPartyID)
{
	FLOG( "CINFCommunityParty::PartyQuestionJoin(char* strMasterName, PartyID_t nPartyID)" );
	char buf[128];
	// 2008-02-26 by bhsohn ���� �Ʒ��� ���� ���� ����
	//wsprintf(buf, STRMSG_C_PARTY_0026,strMasterName);//"%s�� ��뿡 �շ��ϰڽ��ϱ�?"
	char szCharName[SIZE_MAX_ARENA_FULL_NAME];
	util::strncpy(szCharName, strMasterName, SIZE_MAX_ARENA_FULL_NAME);	
	g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharName);
	wsprintf(buf, STRMSG_C_PARTY_0026,szCharName);//"%s�� ��뿡 �շ��ϰڽ��ϱ�?"	
	
	// 2009. 12. 03 by jskim ��Ƽ ������ ��Ƽ�� ���ԵǾ� �ִ��� üũ
	//m_nPartyRequestID = nPartyID;
	//g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_PARTY_INVITE,0,0,0,0,strMasterName);
	//g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_PARTY_INVITE,0,0,0,0,szCharName);	
	g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_PARTY_INVITE,0,0,0,nPartyID,szCharName);	
	//end 2009. 12. 03 by jskim ��Ƽ ������ ��Ƽ�� ���ԵǾ� �ִ��� üũ
}
// 2009. 12. 03 by jskim ��Ƽ ������ ��Ƽ�� ���ԵǾ� �ִ��� üũ
//void CINFCommunityParty::PartyRecvInvite(int i)// ���� �ʴ� ���� ��� ����(i=0)/����(i=1)�� �����Ѵ�.
//{
//	FLOG( "CINFCommunityParty::PartyRecvInvite(int i, PartyID_t PartyId)" );
void CINFCommunityParty::PartyRecvInvite(int i, PartyID_t PartyId)// ���� �ʴ� ���� ��� ����(i=0)/����(i=1)�� �����Ѵ�.
{
	FLOG( "CINFCommunityParty::PartyRecvInvite(int i, PartyID_t PartyId)" );
//end 2009. 12. 03 by jskim ��Ƽ ������ ��Ƽ�� ���ԵǾ� �ִ��� üũ
	if(i == 0)
	{
//		MSG_FC_PARTY_ACCEPT_INVITE sMsg;
//		memset(&sMsg,0x00,sizeof(sMsg));
//		char buffer[SIZE_MAX_PACKET];
//		int nType = T_FC_PARTY_ACCEPT_INVITE;
//		sMsg.PartyID = m_nPartyRequestID;
//		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//		g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
//		
//		g_pD3dApp->m_pShuttleChild->m_infoParty.PartyID = m_nPartyRequestID;
		// 2008-07-14 by bhsohn EP3 ���� ó��
		// 2009. 12. 03 by jskim ��Ƽ ������ ��Ƽ�� ���ԵǾ� �ִ��� üũ	
// 		g_pShuttleChild->m_pClientParty->SetFirstPartySetupInfo(TRUE);
// 
// 		g_pShuttleChild->m_pClientParty->FSendPartyAcceptInvite(m_nPartyRequestID);		
// 		m_nPartyRequestID = 0;
// 		}
// 		else
// 		{
// 		//		MSG_FC_PARTY_REJECT_INVITE sMsg;
// 		//		memset(&sMsg,0x00,sizeof(sMsg));
// 		//		char buffer[SIZE_MAX_PACKET];
// 		//		int nType = T_FC_PARTY_REJECT_INVITE;
// 		//		sMsg.PartyID = m_nPartyRequestID;
// 		//		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
// 		//		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
// 		//		g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
// 		//		g_pD3dApp->m_pShuttleChild->m_infoParty.PartyID = 0;
// 		g_pShuttleChild->m_pClientParty->FSendPartyRejectInvite(m_nPartyRequestID);
// 		m_nPartyRequestID = 0;
// 	} 	
		g_pShuttleChild->m_pClientParty->SetFirstPartySetupInfo(TRUE);
		g_pShuttleChild->m_pClientParty->FSendPartyAcceptInvite( PartyId );

 		vector<CINFMessageBox*>::iterator it = g_pGameMain->m_pInfWindow->m_vecMessageBox.begin();
 		while(it != g_pGameMain->m_pInfWindow->m_vecMessageBox.end())
		{
 			g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_PARTY_INVITE);
 		}
	}
}

void CINFCommunityParty::PartySendSecede()//Ż��
{
	FLOG( "CINFCommunityParty::PartySendSecede()" );
//	if(g_pD3dApp->m_pShuttleChild->m_infoParty.PartyID != 0)
	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().PartyID != 0)
	{
		g_pShuttleChild->m_pClientParty->ISendPartyLeave();
//		MSG_IC_PARTY_LEAVE sMsg;
//		memset(&sMsg,0x00,sizeof(sMsg));
//		char buffer[SIZE_MAX_PACKET];
//		int nType = T_IC_PARTY_LEAVE;
//		sMsg.PartyID = g_pD3dApp->m_pShuttleChild->m_infoParty.PartyID;
//		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//		g_pD3dApp->m_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CClientParty::ISendPartyLeaveArena()
/// \brief		�Ʒ������� �������� ���� ��Ƽ Ż��
/// \author		// 2008-02-27 by bhsohn ���� �Ʒ��� ����
/// \date		2007-12-17 ~ 2007-12-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityParty::ISendPartyLeaveArena()
{
	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().PartyID != 0)
	{
		g_pShuttleChild->m_pClientParty->ISendPartyLeaveArena();
	}

}

//void CINFCommunityParty::PartyQuestionBanUser()//�߹��� �����.
//{
//	FLOG( "CINFCommunityParty::PartyQuestionBanUser()" );
//	switch(m_nPartySelectMember)
//	{
//	case -1:
//		{
//			g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_PARTY_0027, _MESSAGE);//"����Ʈ���� �߹��� ����� �����ϼž� �մϴ�."
//		}
//		break;
//	case 0:
//		{
//			g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_PARTY_0028, _MESSAGE);//"�ڽ��� �߹��� �� �����ϴ�."
//		}
//		break;
//	default:
//		{
//			PARTYENEMYINFO* pPartyEnemyInfo = NULL;
//			int i=0;
//			vector<PARTYENEMYINFO*>::iterator itParty = g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.begin();
//			while(itParty != g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.end())
//			{
//					i ++;
//					if(i == m_nPartySelectMember)
//					{
//					pPartyEnemyInfo = *itParty;
//						break;
//					}
//				itParty++;
//			}
//			
//			if(pPartyEnemyInfo)
//			{
//				if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_PARTY_BAN_MEMBER))
//				{
//					g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_PARTY_BAN_MEMBER);
//				}
//				char buf[128];
////				m_nPartyBanCharacterUniqueNumber = pParty->m_infoCharacter.CharacterUniqueNumber;
////				wsprintf(buf, "%s���� �߹��Ͻðڽ��ϱ�?", pParty->m_infoCharacter.CharacterName);
//				m_nPartyBanCharacterUniqueNumber = pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterUniqueNumber;
//
//				
//				// 2008-02-26 by bhsohn ���� �Ʒ��� ���� ���� ����
//				//wsprintf(buf, STRMSG_C_PARTY_0029, pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterName);//"%s���� �߹��Ͻðڽ��ϱ�?"	
//				char szCharName[SIZE_MAX_ARENA_FULL_NAME];
//				util::strncpy(szCharName, pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterName, SIZE_MAX_ARENA_FULL_NAME);	
//				g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharName);
//				wsprintf(buf, STRMSG_C_PARTY_0029, szCharName);//"%s���� �߹��Ͻðڽ��ϱ�?"
//
//				g_pGameMain->m_pInfWindow->AddMsgBox( buf,_Q_PARTY_BAN_MEMBER);
//			}
//			else
//			{
//				g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_PARTY_0027, _MESSAGE);//"����Ʈ���� �߹��� ����� �����ϼž� �մϴ�."
//			}
//		}
//		break;
//	}
//}


//void CINFCommunityParty::PartySendBanUser()
//{
//	FLOG( "CINFCommunityParty::PartySendBanUser()" );
//	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().PartyID != 0)
//	{
////		MSG_IC_PARTY_BAN_MEMBER sMsg;
////		memset(&sMsg,0x00,sizeof(sMsg));
////		char buffer[SIZE_MAX_PACKET];
////		int nType = T_IC_PARTY_BAN_MEMBER;
////		sMsg.PartyID = g_pD3dApp->m_pShuttleChild->m_infoParty.PartyID;
////		sMsg.CharacterUniqueNumber = m_nPartyBanCharacterUniqueNumber;
////		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
////		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
////		g_pD3dApp->m_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
//		g_pShuttleChild->m_pClientParty->ISendPartyBanMember(m_nPartyBanCharacterUniqueNumber);
//		m_nPartyBanCharacterUniqueNumber = 0;
//	}
//}

//void CINFCommunityParty::PartyQuestionTransferMaster()
//{
//	FLOG( "CINFCommunityParty::PartyQuestionTransferMaster()" );
//	switch(m_nPartySelectMember)
//	{
//	case -1:
//	case 0:
//		{
//			g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_PARTY_0030, _MESSAGE);//"����Ʈ���� ������� �� ����� �����ϼž� �մϴ�."
//		}
//		break;
//	default:
//		{
//			PARTYENEMYINFO* pPartyEnemyInfo = NULL;
//			int i=0;
//			vector<PARTYENEMYINFO*>::iterator itParty = g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.begin();
//			while(itParty != g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.end())
//			{
//				i ++;
//				if((*itParty)->m_pEnemyData && i == m_nPartySelectMember)
//				{
//					pPartyEnemyInfo = *itParty;
//					break;
//				}
//				itParty++;
//			}
//			
//			if(pPartyEnemyInfo)
//			{
//				if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_PARTY_TRANSFER_MASTER))
//				{
//					g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_PARTY_TRANSFER_MASTER);
//				}
//				char buf[128];
//				m_nPartyTransferMasterCharacterUniqueNumber = pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterUniqueNumber;
//		
//				// 2008-02-26 by bhsohn ���� �Ʒ��� ���� ���� ����
//				//wsprintf(buf, STRMSG_C_PARTY_0031, pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterName);//"%s�Կ��� ������� �����Ͻðڽ��ϱ�?"
//				char szCharName[SIZE_MAX_ARENA_FULL_NAME];
//				util::strncpy(szCharName, pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterName, SIZE_MAX_ARENA_FULL_NAME);	
//				g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharName);
//				wsprintf(buf, STRMSG_C_PARTY_0031, szCharName);//"%s�Կ��� ������� �����Ͻðڽ��ϱ�?"
//
//				
//				g_pGameMain->m_pInfWindow->AddMsgBox( buf,_Q_PARTY_TRANSFER_MASTER);
//			}
//			else
//			{
//				g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_PARTY_0032, _MESSAGE);//"���� �ʿ� �ִ� �������� ������ �����մϴ�."
//			}
//		}
//		break;
//	}
//}

void CINFCommunityParty::PartySendTransferMaster()
{
	FLOG( "CINFCommunityParty::PartySendTransferMaster()" );
	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().PartyID != 0)
	{
//		MSG_IC_PARTY_TRANSFER_MASTER sMsg;
//		memset(&sMsg,0x00,sizeof(sMsg));
//		char buffer[SIZE_MAX_PACKET];
//		int nType = T_IC_PARTY_TRANSFER_MASTER;
//		sMsg.PartyID = g_pD3dApp->m_pShuttleChild->m_infoParty.PartyID;
//		sMsg.NewMasterCharacterUniqueNumber = m_nPartyTransferMasterCharacterUniqueNumber;
//		sMsg.OldMasterCharacterUniqueNumber = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
//		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//		g_pD3dApp->m_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
		g_pShuttleChild->m_pClientParty->ISendPartyTransferMaster(m_nPartyTransferMasterCharacterUniqueNumber);
		m_nPartyTransferMasterCharacterUniqueNumber = 0;
	}
}

void CINFCommunityParty::ChagePartyFormation(int nFormation)
{
	FLOG( "CINFCommunityParty::ChagePartyFormation(int nFormation)" );
	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)
	{
		g_pShuttleChild->m_pClientParty->ChagePartyFormation(nFormation);
		//g_pShuttleChild->m_pClientParty->ISendPartyChangeFlightFormation(nFormation);
	}

}

void CINFCommunityParty::SetEnablePartyMenu(BOOL bEnable)
{ 
	FLOG( "CINFCommunityParty::SetEnablePartyMenu(BOOL bEnable)" );
	if(!bEnable)
	{
		if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_PARTY_INVITE_USER))
		{
			g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_PARTY_INVITE_USER);
		}
		if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_PARTY_TRANSFER_MASTER))
		{
			g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_PARTY_TRANSFER_MASTER);
		}
		if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_PARTY_BAN_MEMBER))
		{
			g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_PARTY_BAN_MEMBER);
		}
		if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_PARTY_SECEDE))
		{
			g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_PARTY_SECEDE);
		}
	}
	m_bEnablePartyMenu = bEnable; 
}


void CINFCommunityParty::RefreshSearchInfo()
{	
	BOOL bPartyBtn, bCreateBtn, bJoinBtn;
	bPartyBtn = bCreateBtn = bJoinBtn = FALSE;

	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)
	{
		bPartyBtn = TRUE;
		bCreateBtn = bJoinBtn = FALSE;
	}
	else if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMEMBER)
	{
		bPartyBtn = TRUE;
		bCreateBtn = bJoinBtn = FALSE;
	}
	else if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _NOPARTY)
	{
		bCreateBtn = bJoinBtn = TRUE;
		bPartyBtn = FALSE;
	}

	m_bPartyTypeBtn = g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType;

	{
		// ��ư����		
		m_pSCrateBtn->EnableBtn(bCreateBtn);
		m_pSJoinBtn->EnableBtn(bJoinBtn);
	}	
}
void CINFCommunityParty::RefreshOptionInfo()
{		
	BOOL bJoinBtn, bLeaveBtn, bPurgeBtn, bChargeBtn, bAppOk,bBattle;
	bJoinBtn = bLeaveBtn = bPurgeBtn = bChargeBtn = bAppOk = bBattle = FALSE;

	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)
	{
		bJoinBtn = bLeaveBtn = bPurgeBtn = bChargeBtn = bAppOk = bBattle = TRUE;
	}
	else if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMEMBER)
	{
		bLeaveBtn = TRUE;
		bJoinBtn = bPurgeBtn = bChargeBtn = bAppOk= FALSE;
	}
	else if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _NOPARTY)
	{
		bJoinBtn = bLeaveBtn = bPurgeBtn = bChargeBtn = bAppOk= FALSE;
	}

	// ����� ����
	m_bPartyTypeBtn = g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType;

	{
		m_pOJoinBtn->EnableBtn(bJoinBtn);			// �ʴ�
		m_pOLeaveBtn->EnableBtn(bLeaveBtn);			// Ż��
		m_pOPurgeBtn->EnableBtn(bPurgeBtn);			// �߹�
		m_pOChargeBtn->EnableBtn(bChargeBtn);		// ����		
		m_pOOkBtn->EnableBtn(bAppOk);				// Ȯ��		
		m_pOPartyBattle->EnableBtn(bBattle); 
	}
	//m_nPosVolum = (g_pGameMain->GetVoiceVolum()/100)*VOICECHAT_SPK_W;
	UpdateRadioBtn();
		
}

void CINFCommunityParty::RqPartyList()
{
	ATUM_DATE_TIME curServerTime = GetServerDateTime();
	m_curRqPartyListTime = curServerTime;

	// Lock
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_IC_PARTY_LIST_INFO);		
		vecUnLockMsg.push_back(T_IC_PARTY_LIST_INFO_OK);		
		// �ϳ��� ��Ī �Ǿ ���� Ǯ����. 
		g_pD3dApp->EnterMultiLock(FALSE, T_IC_PARTY_LIST_INFO, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}

	MSG_IC_PARTY_LIST_INFO sMsg;
	memset(&sMsg, 0x00, sizeof(MSG_IC_PARTY_LIST_INFO));

	int nListLen = 	(int)m_vecPartyListInfo.size();	
	if(nListLen > 0)
	{
		int nIdx = (nListLen-1);
		structPartyListInfo* pInfo = m_vecPartyListInfo[nIdx];
		sMsg.StartNum = pInfo->nPartyId;
	}
	else
	{
		// ���ۿ� ����. ���� ó���κ� ��û
		sMsg.StartNum = 0;
	}
	g_pIMSocket->SendMsg( T_IC_PARTY_LIST_INFO, (char*)&sMsg, sizeof(sMsg));		

}

void CINFCommunityParty::AddPartyList(SPARTY_LIST_INFO* pInfo)
{	
	structPartyListInfo* pPartyInfo = new structPartyListInfo;
	memset(pPartyInfo, 0x00, sizeof(structPartyListInfo));

	pPartyInfo->nIdx = (int)m_vecPartyListInfo.size();	// 
	pPartyInfo->nPartyId = pInfo->PartyNum;			// ��Ƽ ��ȣ
	pPartyInfo->bPartyLock = pInfo->PartyLock;		// ���� ����
	strncpy(pPartyInfo->chPartyName, pInfo->PartyName, SIZE_MAX_PARTY_NAME);	// ����
	strncpy(pPartyInfo->chPartyMasterName, pInfo->PartyMasterName, SIZE_MAX_CHARACTER_NAME);	// �����
	pPartyInfo->nMinLV = pInfo->MinLevel;		// �ּ� ����
	pPartyInfo->nMaxLV = pInfo->MaxLevel;		// �ִ� ����
	pPartyInfo->nPartyMem = pInfo->Membercount;	// ����
	pPartyInfo->nMaxPartyMem = SIZE_MAX_PARTY_MEMBER;	// �ִ� ��Ƽ����
	pPartyInfo->ExpDistributeType = pInfo->ExpDistributeType;
	pPartyInfo->ItemDistributeType = pInfo->ItemDistributeType;

	m_vecPartyListInfo.push_back(pPartyInfo);

	if(m_pScrollPartyList)
	{
		int nLienCnt = (int)m_vecPartyListInfo.size();
		int nMaxStep = max(nLienCnt, MAX_PARTY_LIST_LINE);			
		int nOldScrollStep = m_pScrollPartyList->GetScrollStep() + MAX_PARTY_LIST_LINE;
		
		// �ִ� �������� �����ְ�
		m_pScrollPartyList->SetMaxItem(nMaxStep);
		if(nOldScrollStep > nMaxStep)
		{
			nOldScrollStep = nMaxStep;
		}
		
		int nScrollStep = nOldScrollStep - MAX_PARTY_LIST_LINE;
		if(nScrollStep < 0)
		{
			nScrollStep = 0;
		}
		
		// ���� ��ũ�� ��ġ��
		m_pScrollPartyList->SetScrollStep(nScrollStep);
	}

}
void CINFCommunityParty::DeletePartyList()
{
	vector<structPartyListInfo*>::iterator itParty = m_vecPartyListInfo.begin();
	while(itParty != m_vecPartyListInfo.end())
	{
		util::del(*itParty);
		itParty++;
	}
	m_vecPartyListInfo.clear();
	if(m_pScrollPartyList)
	{
		m_pScrollPartyList->SetMaxItem((int)m_vecPartyListInfo.size());
	}

}
void CINFCommunityParty::SetTotalPartyCnt(int nTotalPartyCnt)
{
	m_nTotalPartyCnt = nTotalPartyCnt;
}

void CINFCommunityParty::CheckPartyListInfo()
{
	if(g_pD3dApp->IsLockMode() == FALSE)
	{
		// Lock���¸� ��û���� �ʴ´�.
		return;
	}
	int nScrollStep = m_pScrollPartyList->GetScrollStep();
		
	// �� ��� ����Ʈ�� ���� �޾ƾ� �� ��츦 üũ�Ѵ�.
	int nListSize = (int)m_vecPartyListInfo.size();
	if((m_nTotalPartyCnt > MAX_PARTY_LIST_LINE)
		&& (nListSize < m_nTotalPartyCnt ))
	{
		if((nScrollStep+MAX_PARTY_LIST_LINE) >= nListSize)
		{
			// ��ũ���� �ִ�� ��������.
			RqPartyList();
		}		
	}
}

void CINFCommunityParty::RenderParyInfo(POINT ptPos)
{
	int nPosX,nPosY;
	nPosX = nPosY = 0;
	int nSelRenderItem = m_nSelParty- m_pScrollPartyList->GetScrollStep();

	// ����Ʈ �̹���
	if(nSelRenderItem >= 0 && nSelRenderItem< MAX_PARTY_LIST_LINE)
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		nPosX = ptPos.x + SEL_IMAGE_POS.x - 4;
		nPosY = ptPos.y + SEL_IMAGE_POS.y - 4 + (SEL_IMAGE_POS_CAP_Y * nSelRenderItem);			  
#else  
		nPosX = ptPos.x + SEL_IMAGE_POS_X - 22;
		nPosY = ptPos.y + SEL_IMAGE_POS_Y + (SEL_IMAGE_POS_CAP_Y *nSelRenderItem) - 2;
#endif
		m_pImgSelect->Move(nPosX, nPosY);
		m_pImgSelect->Render();
	}
	int nLine = 0;
	int nCnt =0;
	char chBuffTmp[256];
	memset(chBuffTmp, 0x00, 256);
	vector<structPartyListInfo*>::iterator itParty = m_vecPartyListInfo.begin();
	for(nCnt =0;nCnt < m_pScrollPartyList->GetScrollStep();nCnt++)
	{
		if(itParty == m_vecPartyListInfo.end())
		{
			break;
		}
		itParty++;
	}
	
	for(nCnt =0;nCnt < MAX_PARTY_LIST_LINE;nCnt++)
	{
		if(itParty == m_vecPartyListInfo.end())
		{
			break;
		}
		structPartyListInfo* pListInfo = (*itParty);


		wsprintf(chBuffTmp, "%d", pListInfo->nIdx+1);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		nPosX = ptPos.x + SEL_IMAGE_POS.x;
		
		// ��ȣ
		nPosX += 43;
		SIZE tempSize = m_pPartyList[0]->GetStringSize( chBuffTmp );
		nPosY = ptPos.y + SEL_IMAGE_POS.y + (SEL_IMAGE_POS_CAP_Y *nLine) + ( 16 ) - ( tempSize.cy / 2 );
		
		m_pPartyList[0]->DrawText( nPosX - 22 - tempSize.cx / 2, nPosY, GUI_FONT_COLOR, chBuffTmp,0L);

		// ������
		nPosX += 41;
		if(pListInfo->bPartyLock)
		{
			m_pLockImage->Move( nPosX - 22 - ( m_pLockImage->GetImgSize().x / 2 ), nPosY );
			m_pLockImage->Render();
		}
		else
		{
			m_pUnLockImage->Move(nPosX - 22 - ( m_pUnLockImage->GetImgSize().x / 2 ), nPosY );
			m_pUnLockImage->Render();
		}

		// ����
		nPosX += 145;
		char chBuf[512], chResultBuf[512];
		if( strlen( pListInfo->chPartyName ) > 20 )
		{
			char chTemp[512];
			memset(chTemp,0x00,512);
			memset( chResultBuf,0x00,512);
			strncpy( chResultBuf, pListInfo->chPartyName, 20 );
			chResultBuf[ GetStringBuffPos(chResultBuf, GetStringBuffLen( chResultBuf ) - 1) ]='\0';
			sprintf(chBuf,"%s..", chResultBuf);
		}
		else
		{
			strcpy( chBuf, pListInfo->chPartyName );
		}
		tempSize = m_pPartyList[0]->GetStringSize( chBuf );
		m_pPartyList[0]->DrawText(nPosX - 73 - tempSize.cx / 2, nPosY, GUI_FONT_COLOR, chBuf, 0L);
 
		// ������
		nPosX += 75;
		if( strlen( pListInfo->chPartyMasterName ) > 14 )
		{
			char chTemp[512];
			memset( chTemp,0x00,512 );
			memset( chResultBuf,0x00,512 );
			strncpy( chResultBuf, pListInfo->chPartyMasterName, 14 );
			chResultBuf[ GetStringBuffPos(chResultBuf, GetStringBuffLen( chResultBuf ) - 1) ]='\0';
			sprintf( chBuf, "%s..", chResultBuf );
		}
		else
		{
			strcpy( chBuf, pListInfo->chPartyMasterName );
		}
		tempSize = m_pPartyList[0]->GetStringSize(chBuf);
		m_pPartyList[0]->DrawText(nPosX - 38 - tempSize.cx / 2, nPosY, GUI_FONT_COLOR, chBuf, 0L);

		// ����
		nPosX += 56;
		wsprintf(chBuffTmp, "%d~%d", pListInfo->nMinLV, pListInfo->nMaxLV);
		tempSize = m_pPartyList[0]->GetStringSize(chBuffTmp);
		m_pPartyList[0]->DrawText( nPosX - 28 - tempSize.cx / 2, nPosY, GUI_FONT_COLOR, chBuffTmp, 0L );

		// ����
		nPosX += 46;

		//�ο�
		nPosX += 45;
		wsprintf(chBuffTmp, "%d/%d", pListInfo->nPartyMem, pListInfo->nMaxPartyMem);
		tempSize = m_pPartyList[0]->GetStringSize(chBuffTmp);
		m_pPartyList[0]->DrawText(nPosX - 26 - tempSize.cx / 2, nPosY, GUI_FONT_COLOR, chBuffTmp, 0L);
#else	  
		nPosX = ptPos.x + SEL_IMAGE_POS_X;
		nPosY = ptPos.y + SEL_IMAGE_POS_Y + (SEL_IMAGE_POS_CAP_Y *nLine);

		// ��ȣ
		wsprintf(chBuffTmp, "%d", pListInfo->nIdx+1);
		m_pPartyList[0]->DrawText(nPosX,nPosY,GUI_FONT_COLOR,chBuffTmp,0L);

		// ������
		nPosX += 30;
		if(pListInfo->bPartyLock)
		{
			m_pLockImage->Move(nPosX, nPosY);
			m_pLockImage->Render();
		}
		else
		{
			m_pUnLockImage->Move(nPosX, nPosY);
			m_pUnLockImage->Render();
		}

		// ����
		nPosX += 35;
		m_pPartyList[0]->DrawText(nPosX,nPosY,GUI_FONT_COLOR,pListInfo->chPartyName,0L);

		// ������
		nPosX += 145;
		m_pPartyList[0]->DrawText(nPosX,nPosY,GUI_FONT_COLOR,pListInfo->chPartyMasterName,0L);

		// ����
		nPosX += 85;
		wsprintf(chBuffTmp, "%d~%d", pListInfo->nMinLV, pListInfo->nMaxLV);
		m_pPartyList[0]->DrawText(nPosX,nPosY,GUI_FONT_COLOR,chBuffTmp,0L);

		// ����
		nPosX += 55;
		if(pListInfo->bUseVOIP)
		{
			m_pVOIPImage->Move(nPosX, nPosY);
			m_pVOIPImage->Render();			
		}

		//�ο�
		nPosX += 50;
		wsprintf(chBuffTmp, "%d/%d", pListInfo->nPartyMem, pListInfo->nMaxPartyMem);
		m_pPartyList[0]->DrawText(nPosX,nPosY,GUI_FONT_COLOR,chBuffTmp,0L);
#endif		
		itParty++;
		nLine++;
	}

}

void CINFCommunityParty::ChangePartySubMode(int nSubWndMode)
{
	if((WND_PARTYMODE_OPTION == nSubWndMode) 
		&& (g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _NOPARTY))
	{
		// ��Ƽ�� ������ �ɼ�â�� �����.
		nSubWndMode = WND_PARTYMODE_SEARCH;		
	}
	m_nSubWndMode = nSubWndMode;
	// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
	// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	switch(nSubWndMode)
	{
	case WND_PARTYMODE_OPTION:	// ��Ƽ �ɼ�
		{
			// ��� ���� ����			
			SPARTY_INFO* pPartyInfo = g_pShuttleChild->m_pClientParty->GetSPartyInfo();
			memcpy(&m_pSPartyInfoSetup, pPartyInfo, sizeof(SPARTY_INFO));

			RefreshOptionInfo();	// �ɼ� UI����
			UpdateUIPos();			
			InitControl();
		}
		break;
	case WND_PARTYMODE_SEARCH:	// ��Ƽ �˻�
		{
			BOOL bRqPartyList = FALSE;
			ATUM_DATE_TIME curServerTime = GetServerDateTime();

			if(0 == m_curRqPartyListTime.Year)
			{
				bRqPartyList = TRUE;
			}
			else 
			{				
				int nSecondRemainMotherShip = curServerTime.GetTimeInSeconds() - m_curRqPartyListTime.GetTimeInSeconds();
				if(nSecondRemainMotherShip > RQ_PARTYLIST_TIME)
				{
					bRqPartyList = TRUE;
				}
			}
			if(bRqPartyList)
			{				
				// ó�� ����� Ȥ�� ��Ƽ����Ʈ ��û���� 15��(900��)��, ��Ƽ���� ��û
				RqPartyList();
			}			
			RefreshSearchInfo();
		}
		break;		
	}

}

void CINFCommunityParty::RenderOption(POINT ptPos)
{
	int nCnt = 0;
	POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();

	m_pOJoinBtn->Render();	
	m_pOLeaveBtn->Render();	
	m_pOPurgeBtn->Render();	
	m_pOChargeBtn->Render();		
	m_pOOkBtn->Render();	
	m_pOPartyBattle->Render();

	m_pINFSecuEditBox->Tick();
	m_pINFSecuEditBox->Render();
	
	m_pNumMinEditBox->Render();
	m_pNumMaxEditBox->Render();
	
	// 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
	m_pEditPartyName->Tick();
	//m_pEditPartyName->Render();	
	m_pEditPartyName->Render(0, 1);	
	// end 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
	
	{
		char chBuff[512];
		memset(chBuff, 0x00, 512);
		
		

		// 2009. 08. 11 by ckPark ��� ���� ��ų
// 		((CINFCommunity*)m_pParent)->GetFormatString(m_pComboForMation->GetSelect(), chBuff);
// 		vector<string> vecTmp;
// 
// 		g_pGameMain->StringCulling(chBuff, COMMUNITY_FORMATTXT_WIDTH, &vecTmp, m_pOPartyList);
// 		vector<string>::iterator itTmp = vecTmp.begin();
// 		for(nCnt = 0; nCnt < COMMUNITY_FORMATTXT_LINE;nCnt++)
// 		{
// 			if(itTmp == vecTmp.end())
// 			{
// 				break;
// 			}
// 			char* pTxt = (char*)(*itTmp).c_str();
// 
// 			if(pTxt)
// 			{
// 				m_pOPartyList->DrawText(ptBkPos.x+COMMUNITY_FORMATTXT_X, 
// 					ptBkPos.y+COMMUNITY_FORMATTXT_Y + (COMMUNITY_FORMATTXT_CAP_Y*nCnt), 
// 					GUI_FONT_COLOR, 
// 					pTxt);
// 			}
// 			itTmp++;
// 		}

		// ��� ���� ��ų ����
		int nSkillNum = CAtumSJ::FindPartyFormationSkillNum( m_pComboForMation->GetSelect() );
		if( nSkillNum )
		{
			ITEM* pSkillItem = g_pDatabase->GetServerItemInfo( nSkillNum );
			if( pSkillItem )
			{
				strcpy( chBuff, pSkillItem->Description );
				
				vector<string> vecTmp;

				g_pGameMain->StringCulling(chBuff, COMMUNITY_FORMATION_SKILL_DESC_WIDTH, &vecTmp, m_pOPartyList);
				vector<string>::iterator itTmp = vecTmp.begin();
				for(nCnt = 0; nCnt < CCOMMUNITY_FORMATION_SKILL_DESC_LINE;nCnt++)
				{
					if(itTmp == vecTmp.end())
						break;

					char* pTxt = (char*)(*itTmp).c_str();
					
					if(pTxt)
					{
						m_pOPartyList->DrawText(ptBkPos.x+COMMUNITY_FORMATION_SKILL_DESC.x, 
							ptBkPos.y+COMMUNITY_FORMATION_SKILL_DESC.y + (CCOMMUNITY_FORMATION_SKILL_DESC_CAP_Y*nCnt), 
							GUI_FONT_COLOR, 
							pTxt);
					}
					itTmp++;
				}
			}
		}

		// end 2009. 08. 11 by ckPark ��� ���� ��ų

		int nSelImage = m_pComboForMation->GetSelect() - 1;
		if(nSelImage >= 0 && nSelImage< MAX_COMBO_FLIGHT_FORM)
		{		
			m_pFormatImage[nSelImage]->Move(ptBkPos.x+COMMUNITY_FORMAT.x,ptBkPos.y+COMMUNITY_FORMAT.y);				
			m_pFormatImage[nSelImage]->Render();
		}
		m_pComboForMation->Render();
	}
	//warrior disable voip
	{
		for(nCnt =0; nCnt < MAX_RADIO_JOINT; nCnt++)
		{		
			m_pPartyJoint[nCnt]->Render();
		}
	/*	
		for(nCnt = 0;nCnt < MAX_RADIO_AUDIO;nCnt++)
		{			
			m_pAudioOp[nCnt]->Render();							
		}
		m_pImgSpkVolBar->Move(VOICECHAT_SPK_POS_X + m_nPosVolum,VOICECHAT_SPK_POS_Y);
		m_pImgSpkVolBar->Render();
*/
	}
	
	// �޺��ڽ��� ���� �������� ������ 	
	{
		int nSelCombo = -1;
		for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
		{
			if(m_pComboDivision[nCnt]->IsShowItem())
			{
				nSelCombo = nCnt;
				break;
			}
		}
		for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
		{	
			if(nCnt != nSelCombo)
			{
				m_pComboDivision[nCnt]->Render();
			}
		}
		if(nSelCombo != -1)
		{
			m_pComboDivision[nSelCombo]->Render();
		}
	}

	RenderPartyNumInfo();
}
void CINFCommunityParty::RenderSeach(POINT ptPos)
{	
	m_pSCrateBtn->Render();
	m_pSJoinBtn->Render();
	m_pSRefreshBtn->Render();	
	m_pScrollPartyList->Render();		
	RenderParyInfo(ptPos);
}



void CINFCommunityParty::RenderPartyNumInfo()
{
	POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();

	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _NOPARTY)
	{
		return;
	}
	DWORD dwColor = GUI_FONT_COLOR;
	int nIdx =0;
	int nPosX, nPosY;
	char chBuff[256];	
	memset(chBuff, 0x00, 256);
	nPosX = nPosY = 0;

	if(m_nPartyOPSel != -1)
	{		
		// ������ ������
		POINT ptSelPos;
		ptSelPos.x = PARTYOP_LIST_START.x+ptBkPos.x-3;
		ptSelPos.y = PARTYOP_LIST_START.y+ptBkPos.y + (m_nPartyOPSel*PARTYOP_LIST_HEIGHT) ;

		m_pSelImage->Move(ptSelPos.x, ptSelPos.y);
		m_pSelImage->Render() ;
	}

	{
		// ���� ������
		strncpy(chBuff, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, SIZE_MAX_CHARACTER_NAME);
		g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), chBuff);	// �Ʒ��� �̸� ���� 
		if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)
		{
			strcat(chBuff," @");//�����
		}
		
		if(m_nPartyOPSel == nIdx)
		{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			dwColor = GUI_FONT_COLOR_GR;
#else
			dwColor = RGB(1,1,1);
#endif
		}
		else
		{
			dwColor = GUI_FONT_COLOR;
		}

		nPosX = PARTYOP_LIST_START.x+ptBkPos.x;
		nPosY = PARTYOP_LIST_START.y+ptBkPos.y + (nIdx*PARTYOP_LIST_HEIGHT);

		m_pOPartyList->DrawText(nPosX,nPosY,dwColor,chBuff,0L);

		// 2008-12-02 by bhsohn ���� �¿������� ����üũ
		{
			wsprintf(chBuff, "ON");
			nPosX = PARTYOP_LIST_ON_START.x+ptBkPos.x;
			nPosY = PARTYOP_LIST_ON_START.y+ptBkPos.y + (nIdx*PARTYOP_LIST_HEIGHT);
			m_pOPartyList->DrawText(nPosX,nPosY,dwColor,chBuff,0L);
		}
		// end 2008-12-02 by bhsohn ���� �¿������� ����üũ
		
		nIdx++;		
	}
	
	vector<PARTYENEMYINFO*>::iterator itParty = g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.begin();
	while(itParty != g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.end())
	{
		strncpy(chBuff, (*itParty)->m_ImPartyMemberInfo.CharacterName, SIZE_MAX_CHARACTER_NAME);
		g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), chBuff);	// �Ʒ��� �̸� ���� 
		dwColor = GetParyInfoColor(nIdx, (*itParty)->m_bPartyType, chBuff);		

		nPosX = PARTYOP_LIST_START.x+ptBkPos.x;
		nPosY = PARTYOP_LIST_START.y+ptBkPos.y + (nIdx*PARTYOP_LIST_HEIGHT);

		m_pOPartyList->DrawText(nPosX,nPosY,dwColor,chBuff,0L);

		// 2008-12-02 by bhsohn ���� �¿������� ����üũ
		{
			if((*itParty)->m_bUserLogOn)
			{
				wsprintf(chBuff, "ON");
			}
			else
			{
				wsprintf(chBuff, "OFF");
			}
			nPosX = PARTYOP_LIST_ON_START.x+ptBkPos.x;
			nPosY = PARTYOP_LIST_ON_START.y+ptBkPos.y + (nIdx*PARTYOP_LIST_HEIGHT);
			m_pOPartyList->DrawText(nPosX,nPosY,dwColor,chBuff,0L);
		}
		// end 2008-12-02 by bhsohn ���� �¿������� ����üũ

		nIdx++;
		itParty++;
	}
	
}
DWORD CINFCommunityParty::GetParyInfoColor(int nIdx, PARTY_TYPE PartyType, char* pUserName)
{
	int nPartyOPSel = m_nPartyOPSel;
	DWORD dwColor = GUI_FONT_COLOR;
	switch(PartyType)
	{
	case _PARTYMASTER:
		{
			strcat(pUserName," @");
			if(nPartyOPSel == nIdx)
			{
				dwColor = GUI_SELECT_FONT_COLOR;
			}
			else
			{
				dwColor = GUI_FONT_COLOR;
			}			
		}
		break;
	case _PARTYMEMBER:
		{
			if(nPartyOPSel == nIdx)
			{
				dwColor = GUI_SELECT_FONT_COLOR;
			}
			else
			{
				dwColor = GUI_FONT_COLOR;
			}			
		}
		break;
	case _PARTYOTHERMAPMASTER:
		{
			strcat(pUserName," @");
			if(nPartyOPSel == nIdx)
			{
				dwColor = RGB(128,128,128);
			}
			else
			{
				dwColor = RGB(192,192,192);
			}			
		}
		break;
	case _PARTYOTHERMAPMEMBER:
		{
			if(nPartyOPSel == nIdx)
			{
				dwColor = RGB(128,128,128);
			}
			else
			{
				dwColor = RGB(192,192,192);
			}			
		}
		break;
	case _PARTYRESTARTMEMBER:
		{
			if(nPartyOPSel == nIdx)
			{
				dwColor = RGB(64,64,64);
			}
			else
			{
				dwColor = RGB(128,128,128);
			}			
		}
		break;
	default:
		{
			if(nPartyOPSel == nIdx)
			{
				dwColor = RGB(128,128,128);
			}
			else
			{
				dwColor = RGB(192,192,192);
			}			
		}
		break;
	}
	return dwColor;
}

void CINFCommunityParty::AllEditBoxDisable()
{
	if(m_pEditPartyName->IsEditMode())
	{
		m_pEditPartyName->BackupTxtString();
	}
		
	m_pINFSecuEditBox->EnableEdit(FALSE);
	m_pNumMinEditBox->EnableEdit(FALSE);
	m_pNumMaxEditBox->EnableEdit(FALSE);
	m_pEditPartyName->EnableEdit(FALSE, FALSE);
	
}

void CINFCommunityParty::OnClickJoint(int nId)
{
	m_pSPartyInfoSetup.PartyJoinType = nId;
	switch(nId)
	{
	case RADIO_JOINT_COMMAND:	// ����� �ʴ�
		{
			char chBlank[SIZE_MAX_TEAM_PW+1];
			memset(chBlank, 0x00, SIZE_MAX_TEAM_PW+1);
			
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			m_pINFSecuEditBox->SetString(chBlank, SIZE_MAX_COM_TEAM_PW );	// ��й�ȣ			  
#else																						
			m_pINFSecuEditBox->SetString(chBlank, SIZE_MAX_TEAM_PW);	// ��й�ȣ
#endif
			m_pINFSecuEditBox->BackupTxtString();
		}
		break;
	case RADIO_JOINT_FREE:		// ���� ����
		{			
		}
		break;
	}	
	UpdateRadioBtn();
}
void CINFCommunityParty::OnClickAudioOp(int nId)
{
	//m_nAduioIdx = nId;
	switch(nId)
	{
	case RADIO_AUDIO_CHAT:	// ���� ä�ù��
		{
		}
		break;
	case RADIO_AUDIO_KEY:	// Ű �Է¹��
		{
		}
		break;
	case RADIO_AUDIO_FREE:	// �������
		{
		}
		break;
	}	
	UpdateRadioBtn();
}
void CINFCommunityParty::UpdateRadioBtn()
{
	// �ʴ���
	switch(m_pSPartyInfoSetup.PartyJoinType)
	{
	case RADIO_JOINT_COMMAND:	// ����� �ʴ�
		{
			m_pPartyJoint[RADIO_JOINT_COMMAND]->SetRadioBtn(TRUE);
			m_pPartyJoint[RADIO_JOINT_FREE]->SetRadioBtn(FALSE);
		}
		break;
	case RADIO_JOINT_FREE:		// ���� ����
		{
			m_pPartyJoint[RADIO_JOINT_COMMAND]->SetRadioBtn(FALSE);
			m_pPartyJoint[RADIO_JOINT_FREE]->SetRadioBtn(TRUE);
		}
		break;
	}	 
	
	// ���� ��ư
	//m_pAudioOp[RADIO_AUDIO_CHAT]->SetRadioBtn(TRUE);
	switch(m_nAduioIdx)
	{
	case RADIO_AUDIO_CHAT:	// ���� ä�ù��
		{			
			m_pAudioOp[RADIO_AUDIO_KEY]->SetRadioBtn(FALSE);
			m_pAudioOp[RADIO_AUDIO_FREE]->SetRadioBtn(FALSE);
		}
		break;
	case RADIO_AUDIO_KEY:	// Ű �Է¹��
		{			
			m_pAudioOp[RADIO_AUDIO_KEY]->SetRadioBtn(TRUE);
			m_pAudioOp[RADIO_AUDIO_FREE]->SetRadioBtn(FALSE);
		}
		break;
	case RADIO_AUDIO_FREE:	// �������
		{			
			m_pAudioOp[RADIO_AUDIO_KEY]->SetRadioBtn(FALSE);
			m_pAudioOp[RADIO_AUDIO_FREE]->SetRadioBtn(TRUE);
		}
		break;
	}	
}

void CINFCommunityParty::InitComboInfo()
{
	char chTmpBuff[256];
	memset(chTmpBuff, 0x00, 256);
	{
		// ������
		sprintf(chTmpBuff, STRMSG_C_PARTY_0033);	// ��� ���� �� ��
		m_pComboForMation->AddElement(chTmpBuff);
		
		sprintf(chTmpBuff, STRMSG_C_PARTY_0034);	// �̷� ����, �̷� ���� ������� �� �ٷ� ������ �� ����̴�
		m_pComboForMation->AddElement(chTmpBuff);	
		
		sprintf(chTmpBuff, STRMSG_C_PARTY_0035);	// �̷� Ⱦ��, �̷� Ⱦ�� ������� �� �ٷ� ������ �� ����̴�
		m_pComboForMation->AddElement(chTmpBuff);	
		
		sprintf(chTmpBuff, STRMSG_C_PARTY_0036);	// �ﰢ ���, �ﰢ�� ������� ��ܺ��� 1, 2, 3���� ������ ��ġ�Ѵ�
		m_pComboForMation->AddElement(chTmpBuff);	
		
		sprintf(chTmpBuff, STRMSG_C_PARTY_0037);	// ���ﰢ ���, �� �ﰢ�� ������� ��ܺ��� 3, 2, 1���� ������ ��ġ�Ѵ�
		m_pComboForMation->AddElement(chTmpBuff);
		
		sprintf(chTmpBuff, STRMSG_C_PARTY_0038);	// �� ����, �� ������� ��ܺ��� 1, 3, 2���� ������ ��ġ�Ѵ�
		m_pComboForMation->AddElement(chTmpBuff);	
		
		sprintf(chTmpBuff, STRMSG_C_PARTY_0039);	// ���� ����, ���� ������� ��ܺ��� 2, 3, 1���� ������ ��ġ�Ѵ�
		m_pComboForMation->AddElement(chTmpBuff);	
		
		sprintf(chTmpBuff, STRMSG_C_PARTY_0040);	// X�� ����
		m_pComboForMation->AddElement(chTmpBuff);	
		
		sprintf(chTmpBuff, STRMSG_C_PARTY_0041);	// �� ����
		m_pComboForMation->AddElement(chTmpBuff);		

		m_pComboForMation->UpdateItem();
	}
}

// ��Ʈ�� �ʱ�ȭ
void CINFCommunityParty::InitControl()
{
	char chTmp[256];
	memset(chTmp, 0x00, 256);
	// 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
	//m_pEditPartyName->SetString(m_pSPartyInfoSetup.PartyName, SIZE_MAX_PARTY_NAME);	// ��Ƽ��
	m_pEditPartyName->SetString(m_pSPartyInfoSetup.PartyName, SIZE_MAX_PARTY_NAME, 0, TRUE);	// ��Ƽ��
	// end 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
	m_pEditPartyName->BackupTxtString();	// ���?

	m_pINFSecuEditBox->SetString(m_pSPartyInfoSetup.PartyPW, SIZE_MAX_TEAM_PW);	// ��й�ȣ
	
	wsprintf(chTmp, "%d", m_pSPartyInfoSetup.MinLevel);
	m_pNumMinEditBox->SetString(chTmp, SIZE_MAX_TEAM_PW);	// �ּ� ����
	
	wsprintf(chTmp, "%d", m_pSPartyInfoSetup.MaxLevel);
	m_pNumMaxEditBox->SetString(chTmp, SIZE_MAX_TEAM_PW);	// �ִ� ����
	
	// ��� ����
	m_pComboForMation->SetSelectItem(m_pSPartyInfoSetup.FormationType);

	// ����ġ
	m_pComboDivision[COMBO_DIVISION_EXP]->SetSelectItem(m_pSPartyInfoSetup.ExpDistributeType);
	// ������ �й�
	m_pComboDivision[COMBO_DIVISION_ITEM]->SetSelectItem(m_pSPartyInfoSetup.ItemDistributeType);

	if (false) // g_pGameMain->GetVoiceType() == VOICE_PARTY)
	{
		m_pAudioOp[RADIO_AUDIO_CHAT]->SetRadioBtn(TRUE);
	}
	else
	{
		m_pAudioOp[RADIO_AUDIO_CHAT]->SetRadioBtn(FALSE);
	}
}

void CINFCommunityParty::LoadControlInfo(SPARTY_INFO* pSPartyInfo)
{
	char chTmp[256];
	memset(chTmp, 0x00, 256);
	m_pEditPartyName->GetString(pSPartyInfo->PartyName, SIZE_MAX_PARTY_NAME);	// ��Ƽ��	

	// ����� �ʴ� ����� ��� ��ȣ�� ����.
	switch(m_pSPartyInfoSetup.PartyJoinType)
	{
	case RADIO_JOINT_COMMAND:	// ����� �ʴ�
		{
			memset(pSPartyInfo->PartyPW, 0x00, SIZE_MAX_TEAM_PW);
		}
		break;
	case RADIO_JOINT_FREE:		// ���� ����
		{
			m_pINFSecuEditBox->GetString(pSPartyInfo->PartyPW, SIZE_MAX_TEAM_PW);	// ��й�ȣ
		}
		break;
	}
		
	m_pNumMinEditBox->GetString(chTmp, SIZE_MAX_TEAM_PW);	// �ּ� ����
	pSPartyInfo->MinLevel = atoi(chTmp);
		
	m_pNumMaxEditBox->GetString(chTmp, SIZE_MAX_TEAM_PW);	// �ִ� ����
	pSPartyInfo->MaxLevel= atoi(chTmp);
	
	// ��� ����
	pSPartyInfo->FormationType = m_pComboForMation->GetSelect();

	// ����ġ
	pSPartyInfo->ExpDistributeType = m_pComboDivision[COMBO_DIVISION_EXP]->GetSelect();
	// ������ �й�
	pSPartyInfo->ItemDistributeType = m_pComboDivision[COMBO_DIVISION_ITEM]->GetSelect();

	//��й�ȣ Lock����
	if(strlen(pSPartyInfo->PartyPW) > 0)
	{
		pSPartyInfo->PartyLock = TRUE;
	}
	else
	{
		pSPartyInfo->PartyLock = FALSE;
	}
}

void CINFCommunityParty::ShowPartyFrame()
{	
	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _NOPARTY)
	{
		// ��Ƽ�� ������ ������ �˻�â�� �߰��Ѵ�.
		ChangePartySubMode(WND_PARTYMODE_SEARCH);
	}
	else
	{
		// ��Ƽ�� ������ �ɼ� â�� �߰��Ѵ�.
		ChangePartySubMode(WND_PARTYMODE_OPTION);
	}
	switch(m_nSubWndMode)
	{
	case WND_PARTYMODE_OPTION:	// ��Ƽ �ɼ�
		{
			RefreshOptionInfo();	// �ɼ� UI����			
		}
		break;
	case WND_PARTYMODE_SEARCH:	// ��Ƽ �˻�
		{			
			RefreshSearchInfo();
		}
		break;		
	}
}

void CINFCommunityParty::PartyQuestionBanUser()//�߹��� �����.
{
	FLOG( "CINFCommunityParty::PartyQuestionBanUser()" );
	int nPartySelectMember = m_nPartyOPSel;
	switch(nPartySelectMember)
	{
	case -1:
		{
			g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_PARTY_0027, _MESSAGE);//"����Ʈ���� �߹��� ����� �����ϼž� �մϴ�."
		}
		break;
	case 0:
		{
			g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_PARTY_0028, _MESSAGE);//"�ڽ��� �߹��� �� �����ϴ�."
		}
		break;
	default:
		{
			PARTYENEMYINFO* pPartyEnemyInfo = NULL;
			int i=0;
			vector<PARTYENEMYINFO*>::iterator itParty = g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.begin();
			while(itParty != g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.end())
			{
					i ++;
					if(i == nPartySelectMember)
					{
						pPartyEnemyInfo = *itParty;
						break;
					}
				itParty++;
			}
			
			if(pPartyEnemyInfo)
			{
				if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_PARTY_BAN_MEMBER))
				{
					g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_PARTY_BAN_MEMBER);
				}
				char buf[128];
				SetPartyBanCharacterUniqueNumber(pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterUniqueNumber);				
				
				char szCharName[SIZE_MAX_ARENA_FULL_NAME];
				util::strncpy(szCharName, pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterName, SIZE_MAX_ARENA_FULL_NAME);	
				g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharName);
				wsprintf(buf, STRMSG_C_PARTY_0029, szCharName);//"%s���� �߹��Ͻðڽ��ϱ�?"

				g_pGameMain->m_pInfWindow->AddMsgBox( buf,_Q_PARTY_BAN_MEMBER);
			}
			else
			{
				g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_PARTY_0027, _MESSAGE);//"����Ʈ���� �߹��� ����� �����ϼž� �մϴ�."
			}
		}
		break;
	}
}

void CINFCommunityParty::PartyQuestionTransferMaster()
{
	FLOG( "CINFCommunityParty::PartyQuestionTransferMaster()" );
	int nPartySelectMember = m_nPartyOPSel;
	switch(nPartySelectMember)
	{
	case -1:
	case 0:
		{
			g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_PARTY_0030, _MESSAGE);//"����Ʈ���� ������� �� ����� �����ϼž� �մϴ�."
		}
		break;
	default:
		{
			PARTYENEMYINFO* pPartyEnemyInfo = NULL;
			int i=0;
			vector<PARTYENEMYINFO*>::iterator itParty = g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.begin();
			while(itParty != g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.end())
			{
				i ++;
				// 2008-06-03 by bhsohn EP3 ��� ���� ó��
				//if((*itParty)->m_pEnemyData && i == nPartySelectMember)
				if(i == nPartySelectMember)
				{
					pPartyEnemyInfo = *itParty;
					break;
				}
				itParty++;
			}
			
			// 2008-06-03 by bhsohn EP3 ��� ���� ó��
			if(pPartyEnemyInfo)
			{
				if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_PARTY_TRANSFER_MASTER))
				{
					g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_PARTY_TRANSFER_MASTER);
				}
				char buf[128];
				m_nPartyTransferMasterCharacterUniqueNumber = pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterUniqueNumber;
		
				char szCharName[SIZE_MAX_ARENA_FULL_NAME];
				util::strncpy(szCharName, pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterName, SIZE_MAX_ARENA_FULL_NAME);	
				g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharName);
				wsprintf(buf, STRMSG_C_PARTY_0031, szCharName);//"%s�Կ��� ������� �����Ͻðڽ��ϱ�?"

				
				g_pGameMain->m_pInfWindow->AddMsgBox( buf,_Q_PARTY_TRANSFER_MASTER);
			}
//			else
//			{
//				g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_PARTY_0032, _MESSAGE);//"���� �ʿ� �ִ� �������� ������ �����մϴ�."
//			}
		}
		break;
	}
}
void CINFCommunityParty::OnClickOkBtn()
{
	LoadControlInfo(&m_pSPartyInfoSetup);
	RqChangePartyInfo();	
	ChagePartyFormation(m_pSPartyInfoSetup.FormationType);	// ��� ��������
}
void CINFCommunityParty::RqChangePartyInfo()
{
	// Lock
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_IC_PARTY_CHANGE_INFO);		
		vecUnLockMsg.push_back(T_IC_PARTY_CHANGE_INFO_OK);		
		// �ϳ��� ��Ī �Ǿ ���� Ǯ����. 
		g_pD3dApp->EnterMultiLock(FALSE, T_IC_PARTY_CHANGE_INFO, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}	

	MSG_IC_PARTY_CHANGE_INFO sMsg;	
	memset(&sMsg, 0x00, sizeof(MSG_IC_PARTY_LIST_INFO));	
	memcpy(&sMsg.PartyInfo, &m_pSPartyInfoSetup, sizeof(SPARTY_INFO));		 	

	g_pIMSocket->SendMsg( T_IC_PARTY_CHANGE_INFO, (char*)&sMsg, sizeof(sMsg));		
	
}
// ������ ��Ƽ��
UID32_t CINFCommunityParty::GetPartyBanCharacterUniqueNumber()
{
	return m_nPartyBanCharacterUniqueNumber;
}

void CINFCommunityParty::SetPartyBanCharacterUniqueNumber(UID32_t nPartyBanCharacterUniqueNumber)
{
	m_nPartyBanCharacterUniqueNumber = nPartyBanCharacterUniqueNumber;
}
// ������ ��Ƽ��
UID32_t CINFCommunityParty::GetPartyTransferMasterCharacterUniqueNumber()
{
	return m_nPartyTransferMasterCharacterUniqueNumber;
}

void CINFCommunityParty::SetPartyTransferMasterCharacterUniqueNumber(UID32_t nPartyTransferMasterCharacterUniqueNumber)
{
	m_nPartyTransferMasterCharacterUniqueNumber = nPartyTransferMasterCharacterUniqueNumber;
}

SPARTY_INFO* CINFCommunityParty::GetSPartyInfoSetup()
{
	return &m_pSPartyInfoSetup;
}

// ���ΰ�ħ Ŭ��
void CINFCommunityParty::OnClickRefresh()
{
	SetRefreshClickState(TRUE);		// ��ư�� ������ ��û
	DeletePartyList();		// ��ü�� �� �����.
	RqPartyList();	// ���� ��ħ
}
void CINFCommunityParty::OnClickJoinBtn()
{
	int nSelItem = m_nSelParty;
	int nMaxSize = (int)m_vecPartyListInfo.size();
	if(nSelItem < 0 
		|| nSelItem >= nMaxSize)
	{
		return;
	}

	structPartyListInfo* itListInfo = m_vecPartyListInfo[nSelItem];
	if(itListInfo->bPartyLock)
	{	
		// ��й�ȣ�� �Է��Ͻʽÿ�.
		g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_070430_0100, _Q_PARTY_JOIN_PASSWORD);
	}
	else
	{
		RqClickJoinBtn(NULL);
	}

}
void CINFCommunityParty::RqClickJoinBtn(char* pPass)
{
	int nSelItem = m_nSelParty;
	int nMaxSize = (int)m_vecPartyListInfo.size();
	if(nSelItem < 0 || 
		nSelItem >= nMaxSize)
	{
		return;
	}
	structPartyListInfo* itListInfo = m_vecPartyListInfo[nSelItem];
	g_pShuttleChild->m_pClientParty->FSendPartyJoinFree(itListInfo->nPartyId, pPass);	

}

void CINFCommunityParty::TestList()
{
	vector<structPartyListInfo*>::iterator itList = m_vecPartyListInfo.begin();
	if(itList == m_vecPartyListInfo.end())
	{
		return;
	}
	SPARTY_LIST_INFO stTest;
	structPartyListInfo* pList = (*itList);
	memset(&stTest, 0x00, sizeof(SPARTY_LIST_INFO));
	
	{
		stTest.PartyNum = pList->nPartyId;
		strncpy(stTest.PartyName,pList->chPartyName,SIZE_MAX_PARTY_NAME);
		stTest.ExpDistributeType = pList->ExpDistributeType;
		stTest.ItemDistributeType = pList->ItemDistributeType;
		stTest.PartyLock = pList->bPartyLock;		
		stTest.MinLevel = pList->nMinLV;
		stTest.MaxLevel = pList->nMaxLV;
		strncpy(stTest.PartyMasterName, pList->chPartyMasterName, SIZE_MAX_CHARACTER_NAME);
		stTest.Membercount = pList->nPartyMem;		

	}

	//AddPartyList(&stTest);
}

void CINFCommunityParty::UpdateUIPos()
{
	POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();
	int nCnt = 0;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	int nPosX, nPosY;

	nPosX = nPosY = 0;
	{		
		POINT pPos = m_pPartyControlBtn[ WND_PARTYMODE_OPTION ]->GetFindControlTargetofMinPos("invs00");
		nPosX = ptBkPos.x + CURRENT_PARTY_OPTION_BTN_START.x + pPos.x;
		nPosY = ptBkPos.y + CURRENT_PARTY_OPTION_BTN_START.y;
		m_pOJoinBtn->SetBtnPosition(nPosX, nPosY);	
	}
	{		
		POINT pPos = m_pPartyControlBtn[ WND_PARTYMODE_OPTION ]->GetFindControlTargetofMinPos("outs00");
		nPosX = ptBkPos.x + CURRENT_PARTY_OPTION_BTN_START.x + pPos.x;
		nPosY = ptBkPos.y + CURRENT_PARTY_OPTION_BTN_START.y;
		m_pOLeaveBtn->SetBtnPosition(nPosX, nPosY);	
	}
	
	{		
		POINT pPos = m_pPartyControlBtn[ WND_PARTYMODE_OPTION ]->GetFindControlTargetofMinPos("p_bt20");
		nPosX = ptBkPos.x + CURRENT_PARTY_OPTION_BTN_START.x + pPos.x;
		nPosY = ptBkPos.y + CURRENT_PARTY_OPTION_BTN_START.y;		
		m_pOPurgeBtn->SetBtnPosition(nPosX, nPosY);	
	}
	
	{		
		POINT pPos = m_pPartyControlBtn[ WND_PARTYMODE_OPTION ]->GetFindControlTargetofMinPos("coms00");
		nPosX = ptBkPos.x + CURRENT_PARTY_OPTION_BTN_START.x + pPos.x;
		nPosY = ptBkPos.y + CURRENT_PARTY_OPTION_BTN_START.y;		
		m_pOChargeBtn->SetBtnPosition(nPosX, nPosY);	
	} 
		
	{
		POINT pPos = m_pPartyControlBtn[ WND_PARTYMODE_OPTION ]->GetFindControlTargetofMinPos("apps00");
		nPosX = ptBkPos.x + CURRENT_PARTY_OPTION_BTN_START.x + pPos.x;
		nPosY = ptBkPos.y + CURRENT_PARTY_OPTION_BTN_START.y;
		m_pOOkBtn->SetBtnPosition(nPosX, nPosY);	
	}	

	{
		POINT pPos = m_pPartyControlBtn[ WND_PARTYMODE_OPTION ]->GetFindControlTargetofMinPos("p_pwar0");
		nPosX = ptBkPos.x + CURRENT_PARTY_OPTION_BTN_START.x + pPos.x;
		nPosY = ptBkPos.y + CURRENT_PARTY_OPTION_BTN_START.y;
		m_pOPartyBattle->SetBtnPosition(nPosX, nPosY);	
	}	

	{
		nPosX = ptBkPos.x + SECURITY_WND_PASS_EDIT.x;
		nPosY = ptBkPos.y + SECURITY_WND_PASS_EDIT.y;
		m_pINFSecuEditBox->SetPos(nPosX, nPosY);
	}

	{
		nPosX = ptBkPos.x + MIN_NUM_EDIT.x;
		nPosY = ptBkPos.y + MIN_NUM_EDIT.y;		
		m_pNumMinEditBox->SetPos(nPosX, nPosY);
	}

	{
		nPosX = ptBkPos.x + MAX_NUM_EDIT.x;
		nPosY = ptBkPos.y + MAX_NUM_EDIT.y;		
		m_pNumMaxEditBox->SetPos(nPosX, nPosY);
	}
	
	{
		nPosX = ptBkPos.x + PARTY_NAME_EDIT.x;
		nPosY = ptBkPos.y + PARTY_NAME_EDIT.y;		
		m_pEditPartyName->SetPos(nPosX, nPosY);	
	}
	{		
		// ���� ��� ����		
		for(nCnt = 0;nCnt < MAX_RADIO_JOINT;nCnt++)
		{
			nPosX = ptBkPos.x + PARTYCRE_JOIN.x;
			nPosY = ptBkPos.y + PARTYCRE_JOIN.y + (nCnt*PARTYCRE_JOIN_CAP_Y);
			m_pPartyJoint[nCnt]->SetPosition(nPosX, nPosY, 60);		
		}		
	}
	
	{		
		nPosX = ptBkPos.x + PARTYCRE_AUDIO.x;
		nPosY = ptBkPos.y + PARTYCRE_AUDIO.y;

		m_pAudioOp[RADIO_AUDIO_CHAT]->SetPosition(nPosX, nPosY, 60);		
		// ���� ��� ����		
		for(nCnt = RADIO_AUDIO_KEY;nCnt < MAX_RADIO_AUDIO;nCnt++)
		{
			nPosX = ptBkPos.x + PARTYCRE_AUDIO.x;
			nPosY = ptBkPos.y + PARTYCRE_AUDIO.y+ (nCnt*PARTYCRE_AUDIO_CAP_Y);		
			m_pAudioOp[nCnt]->SetPosition(nPosX, nPosY, 60);		
		}		
	}
	{
		int nMainWidth, nMainHeight;
		int nEleWidth, nEleHeight;
		int nElePosX, nElePosY;
		nElePosX = nElePosY =0;
		nMainWidth = PARTY_FORMAT_COMBO_MAIN_WIDTH;
		nMainHeight = PARTY_FORMAT_COMBO_MAIN_HEIGHT;
		nEleWidth = PARTY_FORMAT_COMBO_ELE_WIDTH;
		nEleHeight = PARTY_FORMAT_COMBO_ELE_HEIGHT;		
	
		nPosX = ptBkPos.x + PARTY_FORMAT_COMBO_MAIN_X;
		nPosY = ptBkPos.y + PARTY_FORMAT_COMBO_MAIN_Y;		
		
		nElePosX = nPosX;
		nElePosY = nPosY + nMainHeight + 2;
		
		m_pComboForMation->SetMainArea(nPosX, nPosY,nMainWidth, nMainHeight);
		m_pComboForMation->SetElementArea(nElePosX, nElePosY, nEleWidth, nEleHeight);
		m_pComboForMation->SetBGPos(nElePosX+6, nElePosY + 1,
												nEleWidth, nEleHeight);
	}
	{
		int nMainWidth, nMainHeight;
		int nEleWidth, nEleHeight;
		nMainWidth = DIVISION_COMBO_MAIN_WIDTH;
		nMainHeight = DIVISION_COMBO_MAIN_HEIGHT;
		nEleWidth = DIVISION_COMBO_ELE_WIDTH;
		nEleHeight = DIVISION_COMBO_ELE_HEIGHT;
		
		POINT	ptMainArena[MAX_COMBO_DIVISION] =
		{ 
			{ DIVISION_COMBO_MAIN_X, DIVISION_COMBO_MAIN_Y },
			{ DIVISION_COMBO_ELE_X, DIVISION_COMBO_ELE_Y },
		};
		
		POINT	ptElementArena[MAX_COMBO_DIVISION] =
		{
			{ptMainArena[0].x, ptMainArena[0].y + nMainHeight},
			{ptMainArena[1].x, ptMainArena[1].y + nMainHeight}			
		};
		
		
		for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
		{
			m_pComboDivision[nCnt]->SetMainArea(ptMainArena[nCnt].x, 
												ptMainArena[nCnt].y,
											nMainWidth, nMainHeight);
			m_pComboDivision[nCnt]->SetElementArea(ptElementArena[nCnt].x,
													ptElementArena[nCnt].y + 3,
													nEleWidth, nEleHeight);
			m_pComboDivision[nCnt]->SetBGPos(ptElementArena[nCnt].x+6, ptElementArena[nCnt].y + 3,
												nEleWidth, nEleHeight);
		}
	}
	
	if(m_pSCrateBtn)
	{		
		POINT pPos = m_pPartyControlBtn[ WND_PARTYMODE_SEARCH ]->GetFindControlTargetofMinPos("cress00");
		nPosX = ptBkPos.x + CURRENT_PARTY_BTN_START.x + pPos.x;
		nPosY = ptBkPos.y + CURRENT_PARTY_BTN_START.y + COMMU_BK_START.y;		
		m_pSCrateBtn->SetBtnPosition(nPosX, nPosY);	
	}
	
	if(m_pSJoinBtn)
	{	
		POINT pPos = m_pPartyControlBtn[ WND_PARTYMODE_SEARCH ]->GetFindControlTargetofMinPos("pars00");
		nPosX = ptBkPos.x + CURRENT_PARTY_BTN_START.x + pPos.x;
		nPosY = ptBkPos.y + CURRENT_PARTY_BTN_START.y + COMMU_BK_START.y;		
		m_pSJoinBtn->SetBtnPosition(nPosX, nPosY);	
	}	
	if(m_pSRefreshBtn)
	{		
		POINT pPos = m_pPartyControlBtn[ WND_PARTYMODE_SEARCH ]->GetFindControlTargetofMinPos("refs00");
		nPosX = ptBkPos.x + CURRENT_PARTY_BTN_START.x + pPos.x;
		nPosY = ptBkPos.y + CURRENT_PARTY_BTN_START.y + COMMU_BK_START.y;		
		m_pSRefreshBtn->SetBtnPosition(nPosX, nPosY);	
	}
	
	if(m_pScrollPartyList)
	{		

		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;
		
		ptScrollPos.x += 486;
		ptScrollPos.y += 86;
		
		// ��ũ�� x = ��ġ�� -5
		// ��ũ�� height = �̹��� ������ - 34
		m_pScrollPartyList->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,223);
		rcMouseWhell.left		= ptScrollPos.x - COMMU_BK_SEARCH_WIDTH;
		rcMouseWhell.top		= ptScrollPos.y;
		rcMouseWhell.right		= ptScrollPos.x - 15;
		rcMouseWhell.bottom		= ptScrollPos.y + 250;
		m_pScrollPartyList->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= ptScrollPos.x + 11;
		rcMousePos.bottom		= ptScrollPos.y + 223;
		m_pScrollPartyList->SetMouseBallRect(rcMousePos);
	}  
#else
	int nPosX, nPosY;
	nPosX = nPosY = 0;
	{		
		nPosX = ptBkPos.x + COMMUNITY_OP_INVITE_X;
		nPosY = ptBkPos.y + COMMUNITY_OP_INVITE_Y;		
		m_pOJoinBtn->SetBtnPosition(nPosX, nPosY);	
	}
	{		
		nPosX = ptBkPos.x + COMMUNITY_OP_LEAVE_X;
		nPosY = ptBkPos.y + COMMUNITY_OP_LEAVE_Y;		
		m_pOLeaveBtn->SetBtnPosition(nPosX, nPosY);	
	}
	
	{		
		nPosX = ptBkPos.x + COMMUNITY_OP_PURGE_X;
		nPosY = ptBkPos.y + COMMUNITY_OP_PURGE_Y;		
		m_pOPurgeBtn->SetBtnPosition(nPosX, nPosY);	
	}
	
	{		
		nPosX = ptBkPos.x + COMMUNITY_OP_CHARE_X;
		nPosY = ptBkPos.y + COMMUNITY_OP_CHARE_Y;		
		m_pOChargeBtn->SetBtnPosition(nPosX, nPosY);	
	} 
		
	{
		nPosX = ptBkPos.x + COMMUNITY_OP_OK_X;
		nPosY = ptBkPos.y + COMMUNITY_OP_OK_Y;		
		m_pOOkBtn->SetBtnPosition(nPosX, nPosY);	
	}	

	{
		nPosX = ptBkPos.x + COMMUNITY_OP_PARTY_BATTLE_X;
		nPosY = ptBkPos.y + COMMUNITY_OP_PARTY_BATTLE_Y;		
		m_pOPartyBattle->SetBtnPosition(nPosX, nPosY);	
	}	

	{
		nPosX = ptBkPos.x + SECURITY_WND_PASS_EDIT_X;
		nPosY = ptBkPos.y + SECURITY_WND_PASS_EDIT_Y;
		m_pINFSecuEditBox->SetPos(nPosX, nPosY);
	}

	{
		nPosX = ptBkPos.x + MIN_NUM_EDIT_X;
		nPosY = ptBkPos.y + MIN_NUM_EDIT_Y;		
		m_pNumMinEditBox->SetPos(nPosX, nPosY);
	}

	{
		nPosX = ptBkPos.x + MAX_NUM_EDIT_X;
		nPosY = ptBkPos.y + MAX_NUM_EDIT_Y;		
		m_pNumMaxEditBox->SetPos(nPosX, nPosY);
	}
	
	{
		nPosX = ptBkPos.x + PARTY_NAME_EDIT_X;
		nPosY = ptBkPos.y + PARTY_NAME_EDIT_Y;		
		m_pEditPartyName->SetPos(nPosX, nPosY);	
	}
	{		
		// ���� ��� ����		
		for(nCnt = 0;nCnt < MAX_RADIO_JOINT;nCnt++)
		{
			nPosX = ptBkPos.x + PARTYCRE_JOIN_X;
			nPosY = ptBkPos.y + PARTYCRE_JOIN_Y+ (nCnt*PARTYCRE_JOIN_CAP_Y);		
			m_pPartyJoint[nCnt]->SetPosition(nPosX, nPosY, 60);		
		}		
	}
	
	{		
		nPosX = ptBkPos.x + PARTYCRE_AUDIO_X+2;
		nPosY = ptBkPos.y + PARTYCRE_AUDIO_Y+5;		

		m_pAudioOp[RADIO_AUDIO_CHAT]->SetPosition(nPosX, nPosY, 60);		
		// ���� ��� ����		
		for(nCnt = RADIO_AUDIO_KEY;nCnt < MAX_RADIO_AUDIO;nCnt++)
		{
			nPosX = ptBkPos.x + PARTYCRE_AUDIO_X;
			nPosY = ptBkPos.y + PARTYCRE_AUDIO_Y+ (nCnt*PARTYCRE_AUDIO_CAP_Y);		
			m_pAudioOp[nCnt]->SetPosition(nPosX, nPosY, 60);		
		}		
	}
	{
		int nMainWidth, nMainHeight;
		int nEleWidth, nEleHeight;
		int nElePosX, nElePosY;
		nElePosX = nElePosY =0;
		nMainWidth = PARTY_FORMAT_COMBO_MAIN_WIDTH;
		nMainHeight = PARTY_FORMAT_COMBO_MAIN_HEIGHT;
		nEleWidth = PARTY_FORMAT_COMBO_ELE_WIDTH;
		nEleHeight = PARTY_FORMAT_COMBO_ELE_HEIGHT;		
	
		nPosX = ptBkPos.x + PARTY_FORMAT_COMBO_MAIN_X;
		nPosY = ptBkPos.y + PARTY_FORMAT_COMBO_MAIN_Y;		
		
		nElePosX = nPosX;
		nElePosY = nPosY + nMainHeight;
		
		m_pComboForMation->SetMainArea(nPosX, nPosY,nMainWidth, nMainHeight);
		m_pComboForMation->SetElementArea(nElePosX, nElePosY, nEleWidth, nEleHeight);
		m_pComboForMation->SetBGPos(nElePosX+6, nElePosY,
												nEleWidth, nEleHeight);
	}
	{
		int nMainWidth, nMainHeight;
		int nEleWidth, nEleHeight;
		nMainWidth = DIVISION_COMBO_MAIN_WIDTH;
		nMainHeight = DIVISION_COMBO_MAIN_HEIGHT;
		nEleWidth = DIVISION_COMBO_ELE_WIDTH;
		nEleHeight = DIVISION_COMBO_ELE_HEIGHT;
		
		POINT	ptMainArena[MAX_COMBO_DIVISION] =
		{
			{ptBkPos.x+138, ptBkPos.y+224},
			{ptBkPos.x+138, ptBkPos.y+242},			
		};
		
		POINT	ptElementArena[MAX_COMBO_DIVISION] =
		{
			{ptMainArena[0].x, ptMainArena[0].y + nMainHeight},
			{ptMainArena[1].x, ptMainArena[1].y + nMainHeight}			
		};
		
		
		for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
		{
			m_pComboDivision[nCnt]->SetMainArea(ptMainArena[nCnt].x, 
												ptMainArena[nCnt].y,
											nMainWidth, nMainHeight);
			m_pComboDivision[nCnt]->SetElementArea(ptElementArena[nCnt].x,
													ptElementArena[nCnt].y,
													nEleWidth, nEleHeight);
			m_pComboDivision[nCnt]->SetBGPos(ptElementArena[nCnt].x+6, ptElementArena[nCnt].y,
												nEleWidth, nEleHeight);
		}
	}
	
	if(m_pSCrateBtn)
	{		
		nPosX = ptBkPos.x + CURRENT_PARTY_CREATE_BTN_X;
		nPosY = ptBkPos.y + CURRENT_PARTY_CREATE_BTN_Y;		
		m_pSCrateBtn->SetBtnPosition(nPosX, nPosY);	
	}
	
	if(m_pSJoinBtn)
	{		
		nPosX = ptBkPos.x + CURRENT_PARTY_JOIN_BTN_X;
		nPosY = ptBkPos.y + CURRENT_PARTY_JOIN_BTN_Y;		
		m_pSJoinBtn->SetBtnPosition(nPosX, nPosY);	
	}	
	if(m_pSRefreshBtn)
	{		
		nPosX = ptBkPos.x + CURRENT_PARTY_REFRESH_BTN_X;
		nPosY = ptBkPos.y + CURRENT_PARTY_REFRESH_BTN_Y;		
		m_pSRefreshBtn->SetBtnPosition(nPosX, nPosY);	
	}
	
	if(m_pScrollPartyList)
	{		

		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;
		
		ptScrollPos.x += 485;
		ptScrollPos.y += 98;
		
		// ��ũ�� x = ��ġ�� -5
		// ��ũ�� height = �̹��� ������ - 34
		m_pScrollPartyList->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,217);
		rcMouseWhell.left		= ptScrollPos.x - COMMU_BK_SEARCH_WIDTH;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 227;
		m_pScrollPartyList->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 237;
		m_pScrollPartyList->SetMouseBallRect(rcMousePos);
	}
#endif
}

void CINFCommunityParty::UpdateSetupInfo(SPARTY_INFO* pInfo)
{
	// ��� ���� ����				
	memcpy(&m_pSPartyInfoSetup, pInfo, sizeof(SPARTY_INFO));

	RefreshOptionInfo();	// �ɼ� UI����
	UpdateUIPos();			
	InitControl();
}
void CINFCommunityParty::SetRefreshClickState(BOOL bClickRefresh)
{
	m_bClickRefresh = bClickRefresh;		// ��ư�� ������ ��û
}
BOOL CINFCommunityParty::GetRefreshClickState()
{
	return m_bClickRefresh ;		// ��ư�� ������ ��û
}

void CINFCommunityParty::UpdateBtnPos()
{
	UpdateUIPos();	// Ui��ġ ����
}
void CINFCommunityParty::SetVoiceChatVolum(DWORD i_nVolum)
{// ���� �����ϴ� �Լ��� ��� ȣ���ϸ� �ӵ��� ���� ������  �����Ҽ��ֵ��� ����.
	if(m_fVolumConTimeGap > VOICECHAT_SPK_CONTROL_TIME_GAP)
	{
		m_fVolumConTimeGap = 0;
		// 2008-10-20 by bhsohn VOIP ���� ����
		// �������� ����
		//g_pGameMain->SetVoiceVolum(i_nVolum);
	}
	m_fVolumConTimeGap += g_pD3dApp->GetElapsedTime();
	m_nVolum = i_nVolum;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
/// \date		2009-04-23 ~ 2009-04-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFCommunityParty::WndProcEdit(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nLienCnt = -1;
	int nArrowScroll = 0;
	BOOL bRtn = FALSE;	
	bRtn = m_pEditPartyName->WndProc(uMsg, wParam, lParam);
	
	if(bRtn)
	{
		return INF_MSGPROC_BREAK;
	}		
	return INF_MSGPROC_NORMAL;
}