// CINFMp3Player.cpp: implementation of the CINFMp3Player class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFMp3Player.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "D3DHanFont.h"
#include "INFScrollBar.h"
#include "INFGameMain.h"
#include "INFWindow.h"
// 2009-01-20 by bhsohn ���� �ý��� ����
//#include "MusicMP3.h"
#include "MusicMP3Ex.h"
// end 2009-01-20 by bhsohn ���� �ý��� ����
#include "AtumSound.h"
#include "Interface.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "dxutil.h"
#include "INFToolTip.h"
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	// �⺻ ������ DEFINE
	#define MP3PLAYER_BASE_POS_X		63
	#define MP3PLAYER_BASE_POS_Y		198

	#define MP3PLAYER_PLAYTIME			0.01f
	#define MP3PLAYER_PLAYTIMEGAB		2

	// �⺻ ��ư ����
	#define MP3PLAYER_BUTTON_STATE_NOR		0
	#define MP3PLAYER_BUTTON_STATE_OVE		1
	#define MP3PLAYER_BUTTON_STATE_PUH		2

	// ��ư ����
	#define MP3PLAYER_BUTTON_PRVE			0
	#define MP3PLAYER_BUTTON_PLAY			1
	#define MP3PLAYER_BUTTON_NEXT			2
	#define MP3PLAYER_BUTTON_STOP			3
	#define MP3PLAYER_BUTTON_FULL			4
	#define MP3PLAYER_BUTTON_MINI			5

	// ��ư ������
	#define MP3PLAYER_BUTTON_SIZE_W			18
	#define MP3PLAYER_BUTTON_SIZE_H			18

	#define MP3PLAYER_BUTTON_START_Y		201

	#define MP3PLAYER_BUTTON_BACK_X			72
	#define MP3PLAYER_BUTTON_PLAY_X			152
	#define MP3PLAYER_BUTTON_PP_X			197
	#define MP3PLAYER_BUTTON_STOP_X			114
	#define MP3PLAYER_BUTTON_PLAYLIST		374
	#define MP3PLAYER_BUTTON_FILEOPEN_X		385
	#define MP3PLAYER_BUTTON_FILEOPEN_Y		166

	#define MP3PLAYER_PLATLIST_SIZE			294
	#define MP3PLAYER_PLATLIST_SIZE_Y		153
	#define MP3PLAYER_PLATLIST_START_X			69
	#define MP3PLAYER_PLATLIST_START_Y			227

	#define MP3PLAYER_DIR_POS_X				64
	#define MP3PLAYER_DIR_POS_Y				167

	// ��ũ�� ��ġ
	#define MP3PLAYER_POS_X				254 
	#define MP3PLAYER_POS_Y				210
	#define MP3PLAYER_LINE_LENGTH		110
	#define MP3PLAYER_VOLUM_STEP		20
	#define MP3PLAYER_VOLUM_LEVEL		50
	#define MP3PLAYER_LINE_GAB			12

	#define MP3PLAYER_LIST_POS_X		354
	#define MP3PLAYER_LIST_POS_Y		248
	#define MP3PLAYER_LIST_LINE_LENGTH	130
	#define MP3PLAYER_LIST_LINE_GAB		10

	#define MP3PLAYER_LOOP				100
	#define MP3PLAYER_END				0

	// �Ǽ���
	#define MP3PLAYER_TAB_DEFAULT		0	// �⺻ ������ (����Ʈ ������ ������ ��)
	#define MP3PLAYER_TAB_REPEAT		1	// �⺻ ������ (����Ʈ ������ ������ ó������ �ٽ� ���),
	#define MP3PLAYER_TAB_REPEAT1		2	// �Ѱ �ݺ�
	#define MP3PLAYER_TAB_RANDOM		3	// ������ ��� ���

	#define MP3PLAYER_TAB_WIDTH			61
	#define MP3PLAYER_TAB_HEIGHT		17

	#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM) || defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn �±� ���� �߰�
	#define MP3PLAYER_BASE_POS_MINUS		130
	#else
	#define MP3PLAYER_BASE_POS_MINUS		132
	#endif

#else
// �⺻ ������ DEFINE
#define MP3PLAYER_BASE_POS_X		(g_pD3dApp->GetBackBufferDesc().Width-251)
#define MP3PLAYER_BASE_POS_Y		(g_pD3dApp->GetBackBufferDesc().Height-37)

// �⺻ ��ư ����
#define MP3PLAYER_BUTTON_STATE_NOR		0
#define MP3PLAYER_BUTTON_STATE_OVE		1
#define MP3PLAYER_BUTTON_STATE_PUH		2

// ��ư ����
#define MP3PLAYER_BUTTON_PRVE			0
#define MP3PLAYER_BUTTON_PLAY			1
#define MP3PLAYER_BUTTON_NEXT			2
#define MP3PLAYER_BUTTON_STOP			3
#define MP3PLAYER_BUTTON_FULL			4
#define MP3PLAYER_BUTTON_MINI			5

// ��ư ������
#define MP3PLAYER_BUTTON_SIZE_W			9
#define MP3PLAYER_BUTTON_SIZE_H			13

// ��ũ�� ��ġ
#define MP3PLAYER_POS_X				(g_pD3dApp->GetBackBufferDesc().Width-126)
#define MP3PLAYER_POS_Y				(g_pD3dApp->GetBackBufferDesc().Height-31)
#define MP3PLAYER_LINE_LENGTH		72
#define MP3PLAYER_VOLUM_STEP		20
#define MP3PLAYER_VOLUM_LEVEL		50
#define MP3PLAYER_LINE_GAB			12

#define MP3PLAYER_LIST_POS_X		(g_pD3dApp->GetBackBufferDesc().Width-14)
#define MP3PLAYER_LIST_POS_Y		(g_pD3dApp->GetBackBufferDesc().Height-164)
#define MP3PLAYER_LIST_LINE_LENGTH	104
#define MP3PLAYER_LIST_LINE_GAB		9

#define MP3PLAYER_LOOP				100
#define MP3PLAYER_END				0

// �Ǽ���
#define MP3PLAYER_TAB_DEFAULT		0	// �⺻ ������ (����Ʈ ������ ������ ��)
#define MP3PLAYER_TAB_REPEAT		1	// �⺻ ������ (����Ʈ ������ ������ ó������ �ٽ� ���),
#define MP3PLAYER_TAB_REPEAT1		2	// �Ѱ �ݺ�
#define MP3PLAYER_TAB_RANDOM		3	// ������ ��� ���

#define MP3PLAYER_TAB_WIDTH			61
#define MP3PLAYER_TAB_HEIGHT		17

#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM) || defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn �±� ���� �߰�
#define MP3PLAYER_BASE_POS_MINUS		130
#else
#define MP3PLAYER_BASE_POS_MINUS		132
#endif

#endif
//#define MP3PLAYER_
//#define MP3PLAYER_
//#define MP3PLAYER_

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFMp3Player::CINFMp3Player()
{
	FLOG( "CINFMp3Player()" );	

	g_pGameMain->m_bMp3PlayFlag = g_pSOption->sMp3Player;
	m_pImgBack		= NULL;
	int i;
	for(i=0; i<MP3PLAYER_BASE_TAB; i++)
		m_pImgTab[i] = NULL;
	for(i=0; i<MP3PLAYER_BUTTON_STATE; i++)
	{
		m_pImgHidenButton[i]=NULL;
		m_pImgMiniButton[i]=NULL;
		m_pImgNextButton[i]=NULL;
		m_pImgPlayButton[i]=NULL;
		m_pImgPrvButton[i]=NULL;
		m_pImgShowListButton[i]=NULL;
		m_pImgStopButton[i]=NULL;
	}
	m_pImgVolum = NULL;
	m_pImgListBox = NULL;
	m_pImgMiniBase = NULL;
	m_pImgSelectMp3 = NULL;
	
	for(i=0; i<MP3PLAYER_MAX_LIST; i++)
		m_pFontTitle[i]	= NULL;
	m_bMp3PlayListShow = FALSE;				// ���۽� �޴�����Ʈ ���̱�
	m_bBaseSizeFull = TRUE;					// ���۽� �����÷��̾� �̴ϻ������ �ϰų� �ƴ� �ƴϰų�
	for(i=0; i<MP3PLAYER_BUTTON_MAIN_STATE; i++)
	{
		m_nButtonStateMain[i] = MP3PLAYER_BUTTON_STATE_NOR;
		m_bButtonStateMain[i] = FALSE;
	}
	m_pScroll = NULL;
	m_pScrollVolum = NULL;
	memset(m_strDirectory, 0x00, MAX_PATH);
	m_vecMp3FileNames.clear();
	m_nCurrentRenderIndex = 0;
	m_nCurrentRenderSelect = 0;
	m_nSelectTab = MP3PLAYER_TAB_REPEAT;
	m_bPlay_Mute = TRUE;
	m_LnMutePos = 0;
	m_bStop_Flag = FALSE;
	memset(m_strMp3Name, 0x00, MAX_PATH);
	m_bMp3Play = FALSE;	
	m_bOriBGSound = TRUE;
	
	// ������, ���, ��������� ���丮 ���氡��
	if (COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_DEMO | RACE_MONITOR))
	{
		m_bSetMp3Dir = TRUE;
	}
	else
	{
		m_bSetMp3Dir = FALSE;
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_fPlaytime = NULL;
	m_nPlaytimeGab = NULL;
	m_nMoveFontPos = 0;
	m_pImgPlayImage = NULL;
	m_pImgStopImage = NULL;
#endif
}

CINFMp3Player::~CINFMp3Player()
{
	FLOG( "~CINFMp3Player()" );
	
	util::del(m_pImgBack);
	int i;
	for(i=0; i<MP3PLAYER_BASE_TAB; i++)
		util::del(m_pImgTab[i]);
	for(i=0; i<MP3PLAYER_BUTTON_STATE; i++)
	{
		util::del(m_pImgHidenButton[i]);
		util::del(m_pImgMiniButton[i]);
		util::del(m_pImgNextButton[i]);
		util::del(m_pImgPlayButton[i]);
		util::del(m_pImgPrvButton[i]);
		util::del(m_pImgShowListButton[i]);
		util::del(m_pImgStopButton[i]);
	}
	util::del(m_pImgVolum);
	util::del(m_pImgListBox);
	util::del(m_pImgMiniBase);
	util::del(m_pImgSelectMp3);

	for(i=0; i<MP3PLAYER_MAX_LIST; i++)		
		util::del(m_pFontTitle[i]);
	util::del(m_pScroll);
	util::del(m_pScrollVolum);
//	util::del(m_pMp3);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	util::del(m_pImgPlayImage);
	util::del(m_pImgStopImage);
#endif
}

HRESULT CINFMp3Player::InitDeviceObjects()
{
	FLOG( "CINFMp3Player::InitDeviceObjects(char* pData, int nSize)" );

	DataHeader	* pDataHeader ;
	char buf[32];
	memset(buf, 0x00, 32);
	
	// �̹��� ����
	wsprintf( buf, "mp_bas");	
	m_pImgBack = new CINFImageEx;
	pDataHeader = FindResource(buf);
	m_pImgBack->InitDeviceObjects(pDataHeader ) ;

	int i;
	for(i=0; i<MP3PLAYER_BASE_TAB; i++)
	{
		wsprintf( buf, "mp_tab%d", i);
		m_pImgTab[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pImgTab[i]->InitDeviceObjects(pDataHeader ) ;
	}
	
	for(i=0; i<MP3PLAYER_BUTTON_STATE; i++)
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		wsprintf( buf, "mp_sh%d", i);
		m_pImgHidenButton[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pImgHidenButton[i]->InitDeviceObjects(pDataHeader) ;
#else
		wsprintf( buf, "mp_sh%d", i);
		m_pImgHidenButton[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pImgHidenButton[i]->InitDeviceObjects(pDataHeader) ;
#endif

		wsprintf( buf, "mp_mi%d", i);
		m_pImgMiniButton[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pImgMiniButton[i]->InitDeviceObjects(pDataHeader ) ;

		wsprintf( buf, "mp_ne%d", i);
		m_pImgNextButton[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pImgNextButton[i]->InitDeviceObjects(pDataHeader ) ;

		wsprintf( buf, "mp_pl%d", i);
		m_pImgPlayButton[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pImgPlayButton[i]->InitDeviceObjects(pDataHeader) ;

		wsprintf( buf, "mp_pr%d", i);
		m_pImgPrvButton[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pImgPrvButton[i]->InitDeviceObjects(pDataHeader ) ;

		wsprintf( buf, "mp_sh%d", i);
		m_pImgShowListButton[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pImgShowListButton[i]->InitDeviceObjects(pDataHeader ) ;

		wsprintf( buf, "mp_st%d", i);
		m_pImgStopButton[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pImgStopButton[i]->InitDeviceObjects(pDataHeader ) ;
	}
	m_pImgVolum = new CINFImageEx;
	pDataHeader = FindResource("mp_vol");
	m_pImgVolum->InitDeviceObjects(pDataHeader ) ;
	
	m_pImgListBox = new CINFImageEx;
	pDataHeader = FindResource("mp_lis");
	m_pImgListBox->InitDeviceObjects(pDataHeader ) ;
	
	m_pImgMiniBase = new CINFImageEx;
	pDataHeader = FindResource("mp_min");
	m_pImgMiniBase->InitDeviceObjects(pDataHeader ) ;
	
	m_pImgSelectMp3 = new CINFImageEx;
	pDataHeader = FindResource("mp_sel");
	m_pImgSelectMp3->InitDeviceObjects(pDataHeader ) ;
	
	// ��Ʈ ����
	for(i=0; i<MP3PLAYER_MAX_LIST; i++)
	{
		m_pFontTitle[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),8, D3DFONT_ZENABLE, FALSE, 512,32);
		m_pFontTitle[i]->InitDeviceObjects(g_pD3dDev) ;
	}	


#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pScroll = new CINFScrollBar(this,
								MP3PLAYER_LIST_POS_X, 
								MP3PLAYER_LIST_POS_Y, 
								MP3PLAYER_LIST_LINE_LENGTH,
								MP3PLAYER_LIST_LINE_GAB,
								11,
								25,
								"c_scrlb",
								INFSCROLL_TYPE_VERTICAL);
#else
	// ��ũ�� ����
	m_pScroll = new CINFScrollBar(this,
								MP3PLAYER_LIST_POS_X, 
								MP3PLAYER_LIST_POS_Y, 
								MP3PLAYER_LIST_LINE_LENGTH,
								MP3PLAYER_LIST_LINE_GAB);
#endif
	m_pScroll->SetGameData( m_pGameData );
	m_pScroll->InitDeviceObjects();

	m_pScrollVolum = new CINFScrollBar(this,
								MP3PLAYER_POS_X, 
								MP3PLAYER_POS_Y, 
								MP3PLAYER_LINE_LENGTH,
								1,
								6,	// ��Ÿ ��ũ�ѹ� ������
								11, // ��Ÿ ��ũ�ѹ� ������
								"mp_vol",
								INFSCROLL_TYPE_HORIZONTAL
								);
	m_pScrollVolum->SetGameData( m_pGameData );
	m_pScrollVolum->InitDeviceObjects();
	m_pScrollVolum->SetNumberOfData( MP3PLAYER_VOLUM_LEVEL );
	
//	int nVol = MP3PLAYER_VOLUM_LEVEL-((g_pSOption->sMusicVolume*-1)/200);
//	m_pScrollVolum->SetCurrentIndex(nVol);
	m_pScrollVolum->SetCurrentIndex(g_pSOption->sMusicVolume);
	// 2012-03-13 mspark, ���� ���� �ɼ� ���� �ذ� - �߰�
	//g_pSOption->sMusicVolume+=1;	
	if(g_pSOption->sMusicVolume>100)
	{
		g_pSOption->sMusicVolume = 100;
	}
	// end 2012-03-13 mspark, ���� ���� �ɼ� ���� �ذ� - �߰�
//	m_pMp3 = new CMusicMP3();
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_fPlaytime = MP3PLAYER_PLAYTIME;
	m_nPlaytimeGab = MP3PLAYER_PLAYTIMEGAB;

	m_pImgPlayImage = new CINFImageEx;
	pDataHeader = FindResource("mp_file0");
	m_pImgPlayImage->InitDeviceObjects( pDataHeader ) ;
	
	m_pImgStopImage = new CINFImageEx;
	pDataHeader = FindResource("mp_file1");
	m_pImgStopImage->InitDeviceObjects( pDataHeader ) ;
#endif
	return S_OK;
}

HRESULT CINFMp3Player::RestoreDeviceObjects()
{
	FLOG( "CINFMp3Player::RestoreDeviceObjects()" );

// 	m_pScroll->SetScrollLinePos( MP3PLAYER_LIST_POS_X,
// 		MP3PLAYER_LIST_POS_Y);
// 	m_pScroll->SetWheelRect(MP3PLAYER_BASE_POS_X-1, 
// 							MP3PLAYER_BASE_POS_Y-132,
// 							g_pD3dApp->GetBackBufferDesc().Width-4,
// 							g_pD3dApp->GetBackBufferDesc().Height-39);
// 	m_pScrollVolum->SetScrollLinePos( MP3PLAYER_POS_X,MP3PLAYER_POS_Y);
// 	m_pScrollVolum->SetWheelRect(g_pD3dApp->GetBackBufferDesc().Width-256, 
// 								g_pD3dApp->GetBackBufferDesc().Height-37,
// 								g_pD3dApp->GetBackBufferDesc().Width-9,
// 								g_pD3dApp->GetBackBufferDesc().Height-24+(2*MP3PLAYER_VOLUM_STEP));
	m_pImgBack->RestoreDeviceObjects();
	int i;
	for(i=0; i<MP3PLAYER_BASE_TAB; i++)
		m_pImgTab[i]->RestoreDeviceObjects();
	for(i=0; i<MP3PLAYER_BUTTON_STATE; i++)
	{
		m_pImgHidenButton[i]->RestoreDeviceObjects();
		m_pImgMiniButton[i]->RestoreDeviceObjects();
		m_pImgNextButton[i]->RestoreDeviceObjects();
		m_pImgPlayButton[i]->RestoreDeviceObjects();
		m_pImgPrvButton[i]->RestoreDeviceObjects();
		m_pImgShowListButton[i]->RestoreDeviceObjects();
		m_pImgStopButton[i]->RestoreDeviceObjects();
	}
	m_pImgVolum->RestoreDeviceObjects();
	m_pImgListBox->RestoreDeviceObjects();
	m_pImgMiniBase->RestoreDeviceObjects();
	m_pImgSelectMp3->RestoreDeviceObjects();	
	for(i=0; i<MP3PLAYER_MAX_LIST; i++)
		m_pFontTitle[i]->RestoreDeviceObjects();

	m_pScrollVolum->RestoreDeviceObjects();
	m_pScrollVolum->SetWheelRect(g_pD3dApp->GetBackBufferDesc().Width-256, 
		g_pD3dApp->GetBackBufferDesc().Height-37,
		g_pD3dApp->GetBackBufferDesc().Width-9,
		g_pD3dApp->GetBackBufferDesc().Height-24+(2*MP3PLAYER_VOLUM_STEP));
	
	m_pScroll->RestoreDeviceObjects();
	m_pScroll->SetWheelRect(MP3PLAYER_BASE_POS_X-1, 
							MP3PLAYER_BASE_POS_Y-132,
							g_pD3dApp->GetBackBufferDesc().Width-4,
							g_pD3dApp->GetBackBufferDesc().Height-39);
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pImgPlayImage->RestoreDeviceObjects();
	m_pImgStopImage->RestoreDeviceObjects();
#endif
	return S_OK;
}

HRESULT CINFMp3Player::InvalidateDeviceObjects()
{
	FLOG( "CINFMp3Player::InvalidateDeviceObjects()" );

	m_pImgBack->InvalidateDeviceObjects();
	int i;
	for(i=0; i<MP3PLAYER_BASE_TAB; i++)
		m_pImgTab[i]->InvalidateDeviceObjects();
	for(i=0; i<MP3PLAYER_BUTTON_STATE; i++)
	{
		m_pImgHidenButton[i]->InvalidateDeviceObjects();
		m_pImgMiniButton[i]->InvalidateDeviceObjects();
		m_pImgNextButton[i]->InvalidateDeviceObjects();
		m_pImgPlayButton[i]->InvalidateDeviceObjects();
		m_pImgPrvButton[i]->InvalidateDeviceObjects();
		m_pImgShowListButton[i]->InvalidateDeviceObjects();
		m_pImgStopButton[i]->InvalidateDeviceObjects();
	}
	m_pImgVolum->InvalidateDeviceObjects();
	m_pImgListBox->InvalidateDeviceObjects();
	m_pImgMiniBase->InvalidateDeviceObjects();
	m_pImgSelectMp3->InvalidateDeviceObjects();

	for(i=0; i<MP3PLAYER_MAX_LIST; i++)
		m_pFontTitle[i]->InvalidateDeviceObjects();
	m_pScrollVolum->InvalidateDeviceObjects();
	m_pScroll->InvalidateDeviceObjects();
	
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pImgPlayImage->InvalidateDeviceObjects();
	m_pImgStopImage->InvalidateDeviceObjects();
#endif
	
	return S_OK;
}

HRESULT CINFMp3Player::DeleteDeviceObjects()
{
	FLOG( "CINFMp3Player::DeleteDeviceObjects()" );
	
	m_pImgBack->DeleteDeviceObjects();
	m_pImgVolum->DeleteDeviceObjects();
	m_pImgListBox->DeleteDeviceObjects();
	m_pImgMiniBase->DeleteDeviceObjects();
	m_pImgSelectMp3->DeleteDeviceObjects();
	m_pScrollVolum->DeleteDeviceObjects();
	m_pScroll->DeleteDeviceObjects();

	util::del(m_pImgBack);
	util::del(m_pImgVolum);
	util::del(m_pImgListBox);
	util::del(m_pImgMiniBase);
	util::del(m_pImgSelectMp3);
	util::del(m_pScrollVolum);
	util::del(m_pScroll);
	
	int i;
	for(i=0; i<MP3PLAYER_BASE_TAB; i++)
	{
		m_pImgTab[i]->DeleteDeviceObjects();
		util::del(m_pImgTab[i]);
	}
	for(i=0; i<MP3PLAYER_BUTTON_STATE; i++)
	{
		m_pImgHidenButton[i]->DeleteDeviceObjects();
		util::del(m_pImgHidenButton[i]);
		
		m_pImgMiniButton[i]->DeleteDeviceObjects();
		util::del(m_pImgMiniButton[i]);
				
		m_pImgNextButton[i]->DeleteDeviceObjects();
		util::del(m_pImgNextButton[i]);
			
		m_pImgPlayButton[i]->DeleteDeviceObjects();
		util::del(m_pImgPlayButton[i]);
			
		m_pImgPrvButton[i]->DeleteDeviceObjects();
		util::del(m_pImgPrvButton[i]);
			
		m_pImgShowListButton[i]->DeleteDeviceObjects();
		util::del(m_pImgShowListButton[i]);
		m_pImgStopButton[i]->DeleteDeviceObjects();
		util::del(m_pImgStopButton[i]);
	}
	for(i=0; i<MP3PLAYER_MAX_LIST; i++)
	{
		m_pFontTitle[i]->DeleteDeviceObjects();
		util::del(m_pFontTitle[i]);
	}
//	if(m_pMp3)
//	{
//		m_pMp3->Atum_MusicStop();
//		util::del(m_pMp3);
//	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pImgPlayImage->DeleteDeviceObjects();
	util::del(m_pImgPlayImage);
	m_pImgStopImage->DeleteDeviceObjects();
	util::del(m_pImgStopImage);
#endif

	return S_OK;
}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
void CINFMp3Player::Render(int posX, int posY)
{
	FLOG( "CINFMp3Player::Render()" );	
	
	if(m_pImgBack && m_bBaseSizeFull == TRUE)
	{
		// Ǯ������ ����3
		m_nBkPosX = posX;
		m_nBkPosY = posY;
		m_pImgBack->Move( posX + MP3PLAYER_BASE_POS_X, posY + MP3PLAYER_BASE_POS_Y);
		m_pImgBack->Render();
		
// 		m_pScrollVolum->RestoreDeviceObjects();
// 		m_pScrollVolum->SetScrollLinePos( posX + MP3PLAYER_POS_X, posY + MP3PLAYER_POS_Y);
// 		m_pScrollVolum->SetWheelRect( posX + MP3PLAYER_POS_X, 
//  									  posY + MP3PLAYER_POS_Y,
// 									  posX + MP3PLAYER_POS_X,
// 									  posY + MP3PLAYER_POS_Y);
		RenderButtonState(posX, posY);
		// ��ư ���� ������
		
		//m_pScrollVolum->Render();
	}
	// ����Ʈ �ڽ� ���̱�
	if(m_bMp3PlayListShow)
	{
		RenderPlayListBox(posX, posY);
		m_pScroll->Render();
	}
	if(g_pD3dApp->m_pSound->m_bPlayMusic && m_bMp3Play == TRUE && m_bPlay_Mute == TRUE )
	{
		m_pImgPlayImage->Move(posX + MP3PLAYER_DIR_POS_X - 24, posY + MP3PLAYER_DIR_POS_Y - 1);
		m_pImgPlayImage->Render();
	}
	else
	{
		m_pImgStopImage->Move(posX + MP3PLAYER_DIR_POS_X - 24, posY + MP3PLAYER_DIR_POS_Y - 1);
		m_pImgStopImage->Render();
	}

	if(m_vecMp3FileNames.size())
	{
		vector<string> vecstr;

		char chrtmp[512];
		memset(chrtmp,0x00,512);
		int nFontLen = GetStringBuffLen( m_vecMp3FileNames[m_nCurrentRenderIndex].szFileNameString );
		int nFindPos = GetStringBuffPos( m_vecMp3FileNames[m_nCurrentRenderIndex].szFileNameString, m_nMoveFontPos);
		strcpy( chrtmp, &(m_vecMp3FileNames[m_nCurrentRenderIndex].szFileNameString)[nFindPos]);

		StringCullingUserDataEx( chrtmp, 330, &vecstr, m_pFontTitle[0] );
		if( vecstr.size() )
		{
			m_pFontTitle[0]->DrawText(posX + MP3PLAYER_DIR_POS_X, posY + MP3PLAYER_DIR_POS_Y, GUI_FONT_COLOR_W, (char*)vecstr[0].c_str() );
		}
	} 
}
#else
void CINFMp3Player::Render()
{
	FLOG( "CINFMp3Player::Render()" );	
	if(m_pImgBack && m_bBaseSizeFull == TRUE)
	{
		// Ǯ������ ����3
		m_pImgBack->Move(MP3PLAYER_BASE_POS_X, MP3PLAYER_BASE_POS_Y);
		m_pImgBack->Render();
		// ��ư ���� ������
		RenderButtonState();
		m_pScrollVolum->Render();
	}
	else
	{
		// �̴ϻ����� ����3
		m_pImgMiniBase->Move(MP3PLAYER_BASE_POS_X+200, MP3PLAYER_BASE_POS_Y);
		m_pImgMiniBase->Render();
	}

	// ����Ʈ �ڽ� ���̱�
	if(m_bMp3PlayListShow)
	{
		RenderPlayListBox();
		m_pScroll->Render();
	}	
}
#endif
void CINFMp3Player::Tick()
{
	FLOG( "CINFMp3Player::Tick()" );

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	
	if(g_pD3dApp->m_pSound->m_bPlayMusic && m_bMp3Play == TRUE && m_bPlay_Mute == TRUE )
	{
		m_fPlaytime -= g_pD3dApp->GetElapsedTime();

		if( m_fPlaytime < 0.0f )
		{
			if( GetStringBuffLen( m_vecMp3FileNames[m_nCurrentRenderIndex].szFileNameString ) > m_nMoveFontPos )
			{
				m_nMoveFontPos += 2;
			}
			else
			{
				m_nPlaytimeGab -= 1;
				if( m_nPlaytimeGab < 0 )
				{
					m_nPlaytimeGab = MP3PLAYER_PLAYTIMEGAB;
					m_nMoveFontPos = 0;
				}
			}
			m_fPlaytime = 1.0f;
		}
	}
#endif
//	m_pScroll->SetScrollLinePos( MP3PLAYER_LIST_POS_X,
//		MP3PLAYER_LIST_POS_Y);
//	m_pScroll->SetWheelRect(MP3PLAYER_BASE_POS_X-1, 
//							MP3PLAYER_BASE_POS_Y-132,
//							g_pD3dApp->GetBackBufferDesc().Width-4,
//							g_pD3dApp->GetBackBufferDesc().Height-39);
//	m_pScrollVolum->SetScrollLinePos( MP3PLAYER_POS_X,MP3PLAYER_POS_Y);
//	m_pScrollVolum->SetWheelRect(g_pD3dApp->GetBackBufferDesc().Width-256, 
//		g_pD3dApp->GetBackBufferDesc().Height-37,
//		g_pD3dApp->GetBackBufferDesc().Width-9,
//		g_pD3dApp->GetBackBufferDesc().Height-24+(2*MP3PLAYER_VOLUM_STEP));

	// 2006-09-15 by ispark
	if(g_pD3dApp->m_pSound->GetMusicMp3() == NULL)
	{
		return;
	}

	if(m_bBaseSizeFull)
	{		
		// 2006-03-15 by ispark, ���� ���� ����
		// 2006-03-22 by ispark
		//int nVol = m_pScrollVolum->GetCurrentScrollIndex();
		// 2012-03-13 by mspark, ���� ���� �ɼ� ���� �ذ�
		int nVol = g_pSOption->sMusicVolume;
		// end 2012-03-13 by mspark, ���� ���� �ɼ� ���� �ذ�
		if(g_pSOption->sMusicVolume != nVol)
		{
			int nNewVol = -10000;
			if(nVol > 0)
			{
				nNewVol = GetMusicVolume(nVol);
			}
//			nVol = ((nVol*200)-10000);		
			g_pSOption->sMusicVolume = nVol;
			// 2013-02-05 by bhsohn Sound ���� ��񿡼� Exception���� ���� ���� ����
			if(g_pD3dApp->m_pSound && g_pD3dApp->m_pSound->IsSoundDevice())
			{
				g_pD3dApp->m_pSound->GetMusicMp3()->SetAtumMusicVolume(nNewVol);
			}
		}
	}
	if(m_bOriBGSound)
	{// 2008-03-26 by dgwoo ��������� �÷����߿��� �ݺ��Ѵ�.
		return;
	}
	// ��Ÿ �ɼ� ����
	switch(m_nSelectTab) 
	{
	case MP3PLAYER_TAB_DEFAULT:
		{
			// 2013-02-05 by bhsohn Sound ���� ��񿡼� Exception���� ���� ���� ����			
//			if(g_pD3dApp->m_pSound->GetMusicMp3()->m_bNowPlay == FALSE)
			if((g_pD3dApp->m_pSound && g_pD3dApp->m_pSound->IsSoundDevice()) && g_pD3dApp->m_pSound->GetMusicMp3()->m_bNowPlay == FALSE)
			{
				vector<Mp3File_t>::iterator it = m_vecMp3FileNames.begin();
				while(it != m_vecMp3FileNames.end())
				{
					if(m_nCurrentRenderIndex == (*it).nIndex)
					{
						if(m_vecMp3FileNames.size()-1 > m_nCurrentRenderIndex)
						{
							m_nCurrentRenderIndex = (*it).nIndex + 1;
							Play_Mp3Music();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
							m_nMoveFontPos = 0;
#endif
							break;
						}
						else
						{// ����Ʈ�� ������ �뷡 ����.
						}
					}
					it++;
				}
				
			}
		}
		break;
	case MP3PLAYER_TAB_RANDOM:
		{			
			// 2013-02-05 by bhsohn Sound ���� ��񿡼� Exception���� ���� ���� ����						
//			if(g_pD3dApp->m_pSound->GetMusicMp3()->m_bNowPlay == FALSE)
			if((g_pD3dApp->m_pSound && g_pD3dApp->m_pSound->IsSoundDevice()) && g_pD3dApp->m_pSound->GetMusicMp3()->m_bNowPlay == FALSE)			
			{
				int nCnt = 0;
//				vector<Mp3File_t>::iterator it = m_vecMp3FileNames.begin();
//				while(it != m_vecMp3FileNames.end())
//				{
//					if(m_nCurrentRenderIndex == m_vecMp3FileNames[nCnt].nIndex)
//					{						
//						m_nCurrentRenderIndex = m_vecMp3FileNames[m_vecRandom[nCnt]].nIndex;
//						Play_Mp3Music();
//						break;
//					}
//					nCnt++;
//					it++;
//				}
//
				vector<int>::iterator it = m_vecRandom.begin();
				while(it != m_vecRandom.end())
				{
					if(m_nCurrentRenderIndex == m_vecMp3FileNames[m_vecRandom[nCnt]].nIndex)
					{
						nCnt++;
						if(nCnt>m_vecRandom.size()-1)
							nCnt = 0;
						m_nCurrentRenderIndex = m_vecMp3FileNames[m_vecRandom[nCnt]].nIndex;
						Play_Mp3Music();
						break;
					}					
					nCnt++;
					it++;
				}
				
			}
		}
		break;
	case MP3PLAYER_TAB_REPEAT:
		{
			// 2013-02-05 by bhsohn Sound ���� ��񿡼� Exception���� ���� ���� ����						
//			if(g_pD3dApp->m_pSound->GetMusicMp3()->m_bNowPlay == FALSE)
			if((g_pD3dApp->m_pSound && g_pD3dApp->m_pSound->IsSoundDevice()) && g_pD3dApp->m_pSound->GetMusicMp3()->m_bNowPlay == FALSE)
			{
				vector<Mp3File_t>::iterator it = m_vecMp3FileNames.begin();
				while(it != m_vecMp3FileNames.end())
				{
					if(m_nCurrentRenderIndex == (*it).nIndex)
					{
						if(m_vecMp3FileNames.size()-1 > m_nCurrentRenderIndex)
						{
							m_nCurrentRenderIndex = (*it).nIndex + 1;
							Play_Mp3Music();
							break;
						}
						else
						{
							m_nCurrentRenderIndex = m_vecMp3FileNames[0].nIndex;
							Play_Mp3Music();
							break;
						}
					}
					it++;
				}
				
			}
		}
		break;
	case MP3PLAYER_TAB_REPEAT1:
		{
			// 2013-02-05 by bhsohn Sound ���� ��񿡼� Exception���� ���� ���� ����
//			if(g_pD3dApp->m_pSound->GetMusicMp3()->m_bNowPlay == FALSE)			
			if(g_pD3dApp->m_pSound && g_pD3dApp->m_pSound->IsSoundDevice() && (g_pD3dApp->m_pSound->GetMusicMp3()->m_bNowPlay == FALSE))
			{
				vector<Mp3File_t>::iterator it = m_vecMp3FileNames.begin();
				while(it != m_vecMp3FileNames.end())
				{
					if(m_nCurrentRenderIndex == (*it).nIndex)
					{
						if(it != m_vecMp3FileNames.end())
						{
													
							Play_Mp3Music();
							//m_nCurrentRenderIndex = (*it).nIndex;	
							break;
						}						
					}
					it++;
				}
				
			}
		}
		break;
	}	
}

int CINFMp3Player::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFMp3Player::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	// 2012-03-20 by mspark, mp3 �߰� ���� - #ifdef C_EPSODE4_UI_CHANGE_JSKIM �κ� �ּ�
//#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
//#else	
	if(m_pScrollVolum && m_pScroll)
	{
		// ��������
		if(m_pScrollVolum->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
		{
			g_pInterface->SaveOptionFile();
			return INF_MSGPROC_BREAK;
		}
		// ��ũ�� ����
		if(m_pScroll->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
		{
			return INF_MSGPROC_BREAK;
		}
	}
//#endif
	// end 2012-03-20 by mspark, mp3 �߰� ���� - #ifdef C_EPSODE4_UI_CHANGE_JSKIM �κ� �ּ�
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����	switch(uMsg)
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			// �̴� ������ �϶��� �극��ũ 
			if(m_bBaseSizeFull == FALSE)
				break;				
			
			for(int i=0; i<MP3PLAYER_BUTTON_MAIN_STATE; i++)
				m_nButtonStateMain[i] = MP3PLAYER_BUTTON_STATE_NOR;

			// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
			g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
			// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
			//////////////////////////////////////////////////////////
			// ��ư ����
			if( pt.y > ( m_nBkPosY + MP3PLAYER_BUTTON_START_Y ) &&
				pt.y < m_nBkPosY + MP3PLAYER_BUTTON_START_Y + MP3PLAYER_BUTTON_SIZE_W )
			{
				if( pt.x>( m_nBkPosX + MP3PLAYER_BUTTON_BACK_X )
					&& pt.x<( m_nBkPosX+ MP3PLAYER_BUTTON_BACK_X + MP3PLAYER_BUTTON_SIZE_W))
				{	// �ڷ�
					m_nButtonStateMain[MP3PLAYER_BUTTON_PRVE] = MP3PLAYER_BUTTON_STATE_OVE;					
				}
				else if( pt.x>(m_nBkPosX + MP3PLAYER_BUTTON_PLAY_X )
					&& pt.x<(m_nBkPosX + MP3PLAYER_BUTTON_PLAY_X + MP3PLAYER_BUTTON_SIZE_W))
				{	// ���
					m_nButtonStateMain[MP3PLAYER_BUTTON_PLAY] = MP3PLAYER_BUTTON_STATE_OVE;					
				}
				else if( pt.x >( m_nBkPosX + MP3PLAYER_BUTTON_PP_X )
					&& pt.x<(m_nBkPosX + MP3PLAYER_BUTTON_PP_X + MP3PLAYER_BUTTON_SIZE_W))
				{	// ������
					m_nButtonStateMain[MP3PLAYER_BUTTON_NEXT] = MP3PLAYER_BUTTON_STATE_OVE;					
				}
				else if( pt.x>( m_nBkPosX + MP3PLAYER_BUTTON_STOP_X)
					&& pt.x<( m_nBkPosX + MP3PLAYER_BUTTON_STOP_X + MP3PLAYER_BUTTON_SIZE_W))
				{	// ����
					m_nButtonStateMain[MP3PLAYER_BUTTON_STOP] = MP3PLAYER_BUTTON_STATE_OVE;					
				}
				else if( pt.x>( m_nBkPosX + MP3PLAYER_BUTTON_PLAYLIST )
					&& pt.x<( m_nBkPosX + MP3PLAYER_BUTTON_PLAYLIST + MP3PLAYER_BUTTON_SIZE_W))
				{	// �÷��� ����Ʈ
					m_nButtonStateMain[MP3PLAYER_BUTTON_FULL] = MP3PLAYER_BUTTON_STATE_OVE;					
					// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
					g_pInterface->m_pToolTip->SetNameToolTip(pt,"STRTOOLTIP39");
					// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����	
				}
// 				else if( pt.x>(MP3PLAYER_BASE_POS_X+238)
// 					&& pt.x<(MP3PLAYER_BASE_POS_X+238+MP3PLAYER_BUTTON_SIZE_W))
// 				{	// Ǯ���
// 					m_nButtonStateMain[MP3PLAYER_BUTTON_MINI] = MP3PLAYER_BUTTON_STATE_OVE;					
// 				}
			}

			// ����Ʈ �ڽ� ��
			if(m_bMp3PlayListShow == TRUE)
			{
				if( pt.y>(m_nBkPosY + MP3PLAYER_PLATLIST_START_Y)
					&& pt.y<(m_nBkPosY + MP3PLAYER_PLATLIST_START_Y + 153))
				{
					if( pt.x>(m_nBkPosX + MP3PLAYER_PLATLIST_START_X)
						&& pt.x<(m_nBkPosX + MP3PLAYER_PLATLIST_START_X + 293))
					{
						return INF_MSGPROC_BREAK;
					}
				}
			}

			//
			/////////////////////////////////////////////////////////
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if(!g_pShuttleChild->GetIsUseInterface())
			{
				break;
			}

			// �̴� ������ �϶��� �극��ũ 
// 			if(m_bBaseSizeFull == FALSE)
// 			{
// 				if( pt.y>(MP3PLAYER_BASE_POS_Y)
// 					&& pt.y<(MP3PLAYER_BASE_POS_Y+MP3PLAYER_BUTTON_SIZE_H))
// 				{
// 					if( pt.x>(MP3PLAYER_BASE_POS_X+200)
// 						&& pt.x<MP3PLAYER_BASE_POS_X+247)
// 					{	// Ǯ������
// 						m_bBaseSizeFull = TRUE;
// 						return INF_MSGPROC_BREAK;
// 					}
// 				}
// 			}			
			
			for(int i=0; i<MP3PLAYER_BUTTON_MAIN_STATE; i++)
				m_bButtonStateMain[i] = FALSE;
			//////////////////////////////////////////////////////////
			// ��ư ����
			if( pt.y>( m_nBkPosY + MP3PLAYER_BUTTON_START_Y)
			 && pt.y<( m_nBkPosY + MP3PLAYER_BUTTON_START_Y+MP3PLAYER_BUTTON_SIZE_H))
			{
				if( pt.x>( m_nBkPosX + MP3PLAYER_BUTTON_BACK_X )
					&& pt.x< ( m_nBkPosX + MP3PLAYER_BUTTON_BACK_X + MP3PLAYER_BUTTON_SIZE_W ))
				{	// �ڷ�					
					m_nButtonStateMain[MP3PLAYER_BUTTON_PRVE] = MP3PLAYER_BUTTON_STATE_PUH;
					m_bButtonStateMain[MP3PLAYER_BUTTON_PRVE] = TRUE;
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x>( m_nBkPosX + MP3PLAYER_BUTTON_PLAY_X )
					&& pt.x<( m_nBkPosX + MP3PLAYER_BUTTON_PLAY_X + MP3PLAYER_BUTTON_SIZE_W))
				{	// ���
					m_nButtonStateMain[MP3PLAYER_BUTTON_PLAY] = MP3PLAYER_BUTTON_STATE_PUH;
					m_bButtonStateMain[MP3PLAYER_BUTTON_PLAY] = TRUE;
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x>( m_nBkPosX + MP3PLAYER_BUTTON_PP_X )
					&& pt.x<( m_nBkPosX + MP3PLAYER_BUTTON_PP_X + MP3PLAYER_BUTTON_SIZE_W))
				{	// ������
					m_nButtonStateMain[MP3PLAYER_BUTTON_NEXT] = MP3PLAYER_BUTTON_STATE_PUH;
					m_bButtonStateMain[MP3PLAYER_BUTTON_NEXT] = TRUE;
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x>( m_nBkPosX + MP3PLAYER_BUTTON_STOP_X )
					&& pt.x<( m_nBkPosX + MP3PLAYER_BUTTON_STOP_X + MP3PLAYER_BUTTON_SIZE_W))
				{	// ����
					m_nButtonStateMain[MP3PLAYER_BUTTON_STOP] = MP3PLAYER_BUTTON_STATE_PUH;
					m_bButtonStateMain[MP3PLAYER_BUTTON_STOP] = TRUE;
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x>( m_nBkPosX + MP3PLAYER_BUTTON_PLAYLIST )
					&& pt.x<( m_nBkPosX + MP3PLAYER_BUTTON_PLAYLIST + MP3PLAYER_BUTTON_SIZE_W))
				{	// �÷��� ����Ʈ
					m_nButtonStateMain[MP3PLAYER_BUTTON_FULL] = MP3PLAYER_BUTTON_STATE_PUH;
					m_bButtonStateMain[MP3PLAYER_BUTTON_FULL] = TRUE;
					return INF_MSGPROC_BREAK;
				}
// 				else if( pt.x>(MP3PLAYER_BASE_POS_X+238)
// 					&& pt.x<(MP3PLAYER_BASE_POS_X+238+MP3PLAYER_BUTTON_SIZE_W))
// 				{	// Ǯ���
// 					m_nButtonStateMain[MP3PLAYER_BUTTON_MINI] = MP3PLAYER_BUTTON_STATE_PUH;
// 					m_bButtonStateMain[MP3PLAYER_BUTTON_MINI] = TRUE;
// 					return INF_MSGPROC_BREAK;
// 				}
			}
			if(pt.x>(m_nBkPosX + MP3PLAYER_BUTTON_FILEOPEN_X)
				&& pt.x<(m_nBkPosX + MP3PLAYER_BUTTON_FILEOPEN_X + MP3PLAYER_BUTTON_SIZE_W )
				&& pt.y>( m_nBkPosY + MP3PLAYER_BUTTON_FILEOPEN_Y)
				&& pt.y<( m_nBkPosY + MP3PLAYER_BUTTON_FILEOPEN_Y + MP3PLAYER_BUTTON_SIZE_H)
				&& TRUE == m_bSetMp3Dir)
			{// ���丮 ��������					
				g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_ETC_0002,
					_Q_MP3_DIRECTORY);
			}
			
			// ����Ʈ �ڽ� ��
			if(m_bMp3PlayListShow == TRUE)
			{
				// ��
				if( pt.y>(m_nBkPosY + MP3PLAYER_PLATLIST_START_Y)
					&& pt.y<m_nBkPosY + MP3PLAYER_PLATLIST_START_Y + 25)
				{
					for(int i=0; i<MP3PLAYER_BASE_TAB; i++)
					{
						if( pt.x> m_nBkPosX + MP3PLAYER_PLATLIST_START_X + (i* (MP3PLAYER_PLATLIST_SIZE / 4 ) )
							&& pt.x< m_nBkPosX + MP3PLAYER_PLATLIST_START_X + (( i + 1 ) * (MP3PLAYER_PLATLIST_SIZE / 4 ) ) )
						{
							m_nSelectTab = i;
							if(m_nSelectTab == MP3PLAYER_TAB_RANDOM)
							{
								RandomPlayList();
							}
							g_pInterface->SetWindowOrder(WNDMp3Player);
							return INF_MSGPROC_BREAK;
						}
					}
				}

				// ����Ʈ
				if( pt.y>(m_nBkPosY + MP3PLAYER_PLATLIST_START_Y + 25 )
					&& pt.y<m_nBkPosY + MP3PLAYER_PLATLIST_START_Y + MP3PLAYER_PLATLIST_SIZE_Y + 25 )
				{
					if( pt.x>(m_nBkPosX + MP3PLAYER_PLATLIST_START_X)
						&& pt.x<m_nBkPosX + MP3PLAYER_PLATLIST_START_X + MP3PLAYER_PLATLIST_SIZE - 15)		// 2012-03-20 by mspark, mp3 �߰� ����
					{
						// 2008-03-04 by dgwoo ������� ���� ����.
						for(int i = 0 ; i < MP3PLAYER_MAX_LIST ; i++)
						{
							if((m_nBkPosY + MP3PLAYER_PLATLIST_START_Y + 25 + (i*MP3PLAYER_LINE_GAB)) < pt.y
								&& (m_nBkPosY + MP3PLAYER_PLATLIST_START_Y + 25 + ((i+1) * MP3PLAYER_LINE_GAB)) > pt.y)
							{
								if(m_vecMp3FileNames.size()>m_pScroll->GetCurrentScrollIndex()+i)
									m_nCurrentRenderSelect = m_vecMp3FileNames[m_pScroll->GetCurrentScrollIndex()+i].nIndex;
							}
						}
						g_pInterface->SetWindowOrder(WNDMp3Player);
						return INF_MSGPROC_BREAK;
					}
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			// �̴� ������ �϶��� �극��ũ 
			if(m_bBaseSizeFull == FALSE)
				break;
			
			//////////////////////////////////////////////////////////
			// ��ư ����
			if( pt.y>( m_nBkPosY + MP3PLAYER_BUTTON_START_Y)
			 && pt.y<( m_nBkPosY + MP3PLAYER_BUTTON_START_Y+MP3PLAYER_BUTTON_SIZE_H))
			{
				if( pt.x>( m_nBkPosX + MP3PLAYER_BUTTON_BACK_X )
					&& pt.x<( m_nBkPosX + MP3PLAYER_BUTTON_BACK_X + MP3PLAYER_BUTTON_SIZE_W))
				{	// �ڷ�
					if(m_bButtonStateMain[MP3PLAYER_BUTTON_PRVE] == TRUE)
					{
						BOOL bFalse = FALSE;
						
						// 2012-03-20 by mspark, mp3 �߰� ���� - ���� ���� ��쿡�� �ڷ� ��ư ���� ���
						if(m_nSelectTab == MP3PLAYER_TAB_RANDOM)
						{
							int nCnt = 0;
							vector<int>::iterator it = m_vecRandom.begin();
							while(it != m_vecRandom.end())
							{
								if(m_nCurrentRenderIndex == m_vecMp3FileNames[m_vecRandom[nCnt]].nIndex)
								{
									nCnt--;
									if(nCnt<0)
										nCnt = m_vecRandom.size()-1;
									m_nCurrentRenderIndex = m_vecMp3FileNames[m_vecRandom[nCnt]].nIndex;
									bFalse = TRUE;
									break;
								}					
								nCnt++;
								it++;
							}
						}
						else
						// 2012-03-20 by mspark, mp3 �߰� ���� - ���� ���� ��쿡�� �ڷ� ��ư ���� ���
						{
						if(m_vecMp3FileNames.size() <= 0)
							break;
						m_LnMutePos = 0;
						m_bPlay_Mute = TRUE;
						if(m_nCurrentRenderIndex <= m_vecMp3FileNames[0].nIndex)
						{
							m_nCurrentRenderIndex = m_vecMp3FileNames[m_vecMp3FileNames.size()-1].nIndex;
							Play_Mp3Music();
							break;
						}
						
						vector<Mp3File_t>::iterator it = m_vecMp3FileNames.begin();
						while(it != m_vecMp3FileNames.end())
						{
							if(m_nCurrentRenderIndex == (*it).nIndex)
							{
								it--;
								m_nCurrentRenderIndex = (*it).nIndex;
								bFalse = TRUE;
								break;
							}
							
							it++;
						}
						}
						
						if(FALSE == bFalse)
						{
							m_nCurrentRenderIndex = m_vecMp3FileNames[m_vecMp3FileNames.size()-1].nIndex; // 2016-01-08 exception at this point
						}
						Play_Mp3Music();

					}
				}
				else if( pt.x>( m_nBkPosX + MP3PLAYER_BUTTON_PLAY_X )
					&& pt.x<( m_nBkPosX + MP3PLAYER_BUTTON_PLAY_X + MP3PLAYER_BUTTON_SIZE_W))
				{	// ���, ����.
					if(m_bButtonStateMain[MP3PLAYER_BUTTON_PLAY] == TRUE)
					{
						if(m_vecMp3FileNames.size() <= 0)
						{
							g_pD3dApp->m_pSound->m_bPlayMusic = FALSE;
							m_bStop_Flag = FALSE;
							break;
						}
						if(m_bStop_Flag)
						{// ���� => ��� �ϰ�� ���õ� ������ �����Ų��.
							m_nCurrentRenderIndex = m_nCurrentRenderSelect;
							Play_Mp3Music();
							m_bStop_Flag = FALSE;
							break;
						}

						if(m_bPlay_Mute)
						{// ����.
							if(m_bOriBGSound)
							{
								Play_Mp3Music();
								m_bOriBGSound = FALSE;
							}
							else
							{
								// 2013-02-05 by bhsohn Sound ���� ��񿡼� Exception���� ���� ���� ����			
// 								g_pD3dApp->m_pSound->GetMusicMp3()->Atum_MusicStop();
// 								m_LnMutePos = g_pD3dApp->m_pSound->GetMusicMp3()->m_pLPos;
								if(g_pD3dApp->m_pSound && g_pD3dApp->m_pSound->IsSoundDevice())
								{
									g_pD3dApp->m_pSound->GetMusicMp3()->Atum_MusicStop();
									m_LnMutePos = g_pD3dApp->m_pSound->GetMusicMp3()->m_pLPos;
								}
								m_bPlay_Mute = FALSE;
							}
						}
						else
						{// ���.
							if(m_LnMutePos != 0)
							{
								Play_Mp3Music(TRUE);
								m_bPlay_Mute = 0;
							}
							else
							{
								Play_Mp3Music();
							}
							m_bPlay_Mute = TRUE;
						}	
					}
				}
				else if( pt.x>( m_nBkPosX + MP3PLAYER_BUTTON_PP_X )
					&& pt.x<( m_nBkPosX + MP3PLAYER_BUTTON_PP_X + MP3PLAYER_BUTTON_SIZE_W))
				{	// ������
					if(m_bButtonStateMain[MP3PLAYER_BUTTON_NEXT] == TRUE)
					{
						// 2012-03-20 by mspark, mp3 �߰� ���� - ���� �ǿ��� ������ ��ư ���� ���
						if(m_nSelectTab == MP3PLAYER_TAB_RANDOM)
						{
							int nCnt = 0;
							vector<int>::iterator it = m_vecRandom.begin();
							while(it != m_vecRandom.end())
							{
								if(m_nCurrentRenderIndex == m_vecMp3FileNames[m_vecRandom[nCnt]].nIndex)
								{
									nCnt++;
									if(nCnt>m_vecRandom.size()-1)
										nCnt = 0;
									m_nCurrentRenderIndex = m_vecMp3FileNames[m_vecRandom[nCnt]].nIndex;
									break;
								}					
								nCnt++;
								it++;
							}
						}
						else
						// end 2012-03-20 by mspark, mp3 �߰� ���� - ���� �ǿ��� ������ ��ư ���� ���
						{
						if(m_vecMp3FileNames.size() <= 0)
							break;
						m_LnMutePos = 0;
						m_bPlay_Mute = TRUE;
						if(m_vecMp3FileNames.size()-1 <= m_nCurrentRenderIndex)
						{
							m_nCurrentRenderIndex = m_vecMp3FileNames[0].nIndex;
							Play_Mp3Music();
							break;
						}
						
						vector<Mp3File_t>::iterator it = m_vecMp3FileNames.begin();
						while(it != m_vecMp3FileNames.end())
						{
							if(m_nCurrentRenderIndex == (*it).nIndex)
							{
								it++;
								m_nCurrentRenderIndex = (*it).nIndex;								
								break;
							}
							
							it++;
							}
						}
						
						Play_Mp3Music();
					}
				}
				else if( pt.x>( m_nBkPosX + MP3PLAYER_BUTTON_STOP_X )
					&& pt.x<( m_nBkPosX + MP3PLAYER_BUTTON_STOP_X + MP3PLAYER_BUTTON_SIZE_W))
				{	// ����
					if(m_bButtonStateMain[MP3PLAYER_BUTTON_STOP] == TRUE)
					{
						// 2013-02-05 by bhsohn Sound ���� ��񿡼� Exception���� ���� ���� ����			
						if(g_pD3dApp->m_pSound && g_pD3dApp->m_pSound->IsSoundDevice())
						{
							g_pD3dApp->m_pSound->GetMusicMp3()->Atum_MusicStop();
						}
						m_bMp3Play = FALSE;
						m_bStop_Flag = TRUE;
						m_bOriBGSound = FALSE;
					}
				}
				else if( pt.x>( m_nBkPosX + MP3PLAYER_BUTTON_PLAYLIST )
					&& pt.x<( m_nBkPosX + MP3PLAYER_BUTTON_PLAYLIST + MP3PLAYER_BUTTON_SIZE_W))
				{	// �÷��� ����Ʈ
					if(m_bButtonStateMain[MP3PLAYER_BUTTON_FULL] == TRUE)
					{
						m_bMp3PlayListShow = !m_bMp3PlayListShow;
						// ����Ʈ Ȱ���� �Ǹ� ������ �ڵ����� â �켱 ������ �־ ������ �ﵵ�� �Ѵ�.
						if(m_bMp3PlayListShow == TRUE)
							g_pInterface->SetWindowOrder(WNDMp3Player);
					}
				}
// 				else if( pt.x>(MP3PLAYER_BASE_POS_X+238)
// 					&& pt.x<(MP3PLAYER_BASE_POS_X+238+MP3PLAYER_BUTTON_SIZE_W))
// 				{	// Ǯ���
// 					if(m_bButtonStateMain[MP3PLAYER_BUTTON_MINI] == TRUE)
// 					{
// 						m_bBaseSizeFull = !m_bBaseSizeFull;
// 						if(m_bBaseSizeFull == FALSE)
// 							m_bMp3PlayListShow = FALSE;
// 					}
// 				}
			}
			
			for(int i=0; i<MP3PLAYER_BUTTON_MAIN_STATE; i++)
			{	// �ʱ�ȭ
				m_nButtonStateMain[i] = MP3PLAYER_BUTTON_STATE_NOR;
				m_bButtonStateMain[i] = FALSE;
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			// ����Ʈ �ڽ� ��(������� ����)
			if(m_bMp3PlayListShow == TRUE)
			{
				if( pt.y>(m_nBkPosY + MP3PLAYER_PLATLIST_START_Y + 25)
					&& pt.y<(m_nBkPosY + MP3PLAYER_PLATLIST_START_Y + 25 + MP3PLAYER_PLATLIST_SIZE_Y) )
				{
					if( pt.x>( m_nBkPosX + MP3PLAYER_PLATLIST_START_X )
						&& pt.x<( m_nBkPosX + MP3PLAYER_PLATLIST_START_X + MP3PLAYER_PLATLIST_SIZE ) - 15)		// 2012-03-20 by mspark, mp3 �߰� ���� - (- 15) �߰�
					{
						int nTemp=0;
						for(int i=0; i<MP3PLAYER_MAX_LIST + 1; i++)
						{							
							if((m_nBkPosY + MP3PLAYER_PLATLIST_START_Y + 25 +nTemp)<pt.y 
							&& (m_nBkPosY + MP3PLAYER_PLATLIST_START_Y + 25 +nTemp+MP3PLAYER_LINE_GAB)>pt.y)
							{
								if(m_vecMp3FileNames.size()>m_pScroll->GetCurrentScrollIndex()+i-1)
									m_nCurrentRenderIndex = m_vecMp3FileNames[m_pScroll->GetCurrentScrollIndex()+i-1].nIndex;
								Play_Mp3Music();
							}

							nTemp = (i*MP3PLAYER_LINE_GAB);
						}
						return INF_MSGPROC_BREAK;
					}
				}
			}
		}
		break;
	case WM_KEYDOWN:
		{
			if(wParam == VK_DELETE)
			{
				if(m_bMp3PlayListShow == TRUE)
				{
					g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_ETC_0003,//"�����Ͻ� ���� �÷��� ����Ʈ���� �����Ͻðڽ��ϱ�?"
						_Q_MP3_PLAYLIST_DEL);
					return INF_MSGPROC_BREAK;					
				}
			}
		} 
	}
#else
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			// �̴� ������ �϶��� �극��ũ 
			if(m_bBaseSizeFull == FALSE)
				break;				
			
			for(int i=0; i<MP3PLAYER_BUTTON_MAIN_STATE; i++)
				m_nButtonStateMain[i] = MP3PLAYER_BUTTON_STATE_NOR;

			//////////////////////////////////////////////////////////
			// ��ư ����
			if( pt.y>(MP3PLAYER_BASE_POS_Y)
				&& pt.y<(MP3PLAYER_BASE_POS_Y+MP3PLAYER_BUTTON_SIZE_H))
			{
				if( pt.x>(MP3PLAYER_BASE_POS_X+51)
					&& pt.x<(MP3PLAYER_BASE_POS_X+51+MP3PLAYER_BUTTON_SIZE_W))
				{	// �ڷ�
					m_nButtonStateMain[MP3PLAYER_BUTTON_PRVE] = MP3PLAYER_BUTTON_STATE_OVE;					
				}
				else if( pt.x>(MP3PLAYER_BASE_POS_X+67)
					&& pt.x<(MP3PLAYER_BASE_POS_X+67+MP3PLAYER_BUTTON_SIZE_W))
				{	// ���
					m_nButtonStateMain[MP3PLAYER_BUTTON_PLAY] = MP3PLAYER_BUTTON_STATE_OVE;					
				}
				else if( pt.x>(MP3PLAYER_BASE_POS_X+81)
					&& pt.x<(MP3PLAYER_BASE_POS_X+81+MP3PLAYER_BUTTON_SIZE_W))
				{	// ������
					m_nButtonStateMain[MP3PLAYER_BUTTON_NEXT] = MP3PLAYER_BUTTON_STATE_OVE;					
				}
				else if( pt.x>(MP3PLAYER_BASE_POS_X+95)
					&& pt.x<(MP3PLAYER_BASE_POS_X+95+MP3PLAYER_BUTTON_SIZE_W))
				{	// ����
					m_nButtonStateMain[MP3PLAYER_BUTTON_STOP] = MP3PLAYER_BUTTON_STATE_OVE;					
				}
				else if( pt.x>(MP3PLAYER_BASE_POS_X+225)
					&& pt.x<(MP3PLAYER_BASE_POS_X+225+MP3PLAYER_BUTTON_SIZE_W))
				{	// �÷��� ����Ʈ
					m_nButtonStateMain[MP3PLAYER_BUTTON_FULL] = MP3PLAYER_BUTTON_STATE_OVE;					
				}
				else if( pt.x>(MP3PLAYER_BASE_POS_X+238)
					&& pt.x<(MP3PLAYER_BASE_POS_X+238+MP3PLAYER_BUTTON_SIZE_W))
				{	// Ǯ���
					m_nButtonStateMain[MP3PLAYER_BUTTON_MINI] = MP3PLAYER_BUTTON_STATE_OVE;					
				}
			}

			// ����Ʈ �ڽ� ��
			if(m_bMp3PlayListShow == TRUE)
			{
				if( pt.y>(MP3PLAYER_BASE_POS_Y-132)
					&& pt.y<(MP3PLAYER_BASE_POS_Y+2))
				{
					if( pt.x>(MP3PLAYER_BASE_POS_X-1)
						&& pt.x<(MP3PLAYER_BASE_POS_X+233))
					{
						return INF_MSGPROC_BREAK;
					}
				}
			}

			//
			/////////////////////////////////////////////////////////
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			// 2005-10-05 by ispark
			// �����ÿ��� ����ϵ��� ����
//			if(g_pD3dApp->m_bCharacter == FALSE && (g_pShuttleChild->m_bUnitStop == FALSE && g_pShuttleChild->m_bLandedMove == FALSE))
			if(!g_pShuttleChild->GetIsUseInterface())
			{
				break;
			}

			// �̴� ������ �϶��� �극��ũ 
			if(m_bBaseSizeFull == FALSE)
			{
				if( pt.y>(MP3PLAYER_BASE_POS_Y)
					&& pt.y<(MP3PLAYER_BASE_POS_Y+MP3PLAYER_BUTTON_SIZE_H))
				{
					if( pt.x>(MP3PLAYER_BASE_POS_X+200)
						&& pt.x<MP3PLAYER_BASE_POS_X+247)
					{	// Ǯ������
						m_bBaseSizeFull = TRUE;
						return INF_MSGPROC_BREAK;
					}
				}
			}			
			
			for(int i=0; i<MP3PLAYER_BUTTON_MAIN_STATE; i++)
				m_bButtonStateMain[i] = FALSE;
			//////////////////////////////////////////////////////////
			// ��ư ����
			if( pt.y>(MP3PLAYER_BASE_POS_Y)
			 && pt.y<(MP3PLAYER_BASE_POS_Y+MP3PLAYER_BUTTON_SIZE_H))
			{
				if( pt.x>(MP3PLAYER_BASE_POS_X+51)
					&& pt.x<(MP3PLAYER_BASE_POS_X+51+MP3PLAYER_BUTTON_SIZE_W))
				{	// �ڷ�					
					m_nButtonStateMain[MP3PLAYER_BUTTON_PRVE] = MP3PLAYER_BUTTON_STATE_PUH;
					m_bButtonStateMain[MP3PLAYER_BUTTON_PRVE] = TRUE;
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x>(MP3PLAYER_BASE_POS_X+67)
					&& pt.x<(MP3PLAYER_BASE_POS_X+67+MP3PLAYER_BUTTON_SIZE_W))
				{	// ���
					m_nButtonStateMain[MP3PLAYER_BUTTON_PLAY] = MP3PLAYER_BUTTON_STATE_PUH;
					m_bButtonStateMain[MP3PLAYER_BUTTON_PLAY] = TRUE;
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x>(MP3PLAYER_BASE_POS_X+81)
					&& pt.x<(MP3PLAYER_BASE_POS_X+81+MP3PLAYER_BUTTON_SIZE_W))
				{	// ������
					m_nButtonStateMain[MP3PLAYER_BUTTON_NEXT] = MP3PLAYER_BUTTON_STATE_PUH;
					m_bButtonStateMain[MP3PLAYER_BUTTON_NEXT] = TRUE;
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x>(MP3PLAYER_BASE_POS_X+95)
					&& pt.x<(MP3PLAYER_BASE_POS_X+95+MP3PLAYER_BUTTON_SIZE_W))
				{	// ����
					m_nButtonStateMain[MP3PLAYER_BUTTON_STOP] = MP3PLAYER_BUTTON_STATE_PUH;
					m_bButtonStateMain[MP3PLAYER_BUTTON_STOP] = TRUE;
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x>(MP3PLAYER_BASE_POS_X+225)
					&& pt.x<(MP3PLAYER_BASE_POS_X+225+MP3PLAYER_BUTTON_SIZE_W))
				{	// �÷��� ����Ʈ
					m_nButtonStateMain[MP3PLAYER_BUTTON_FULL] = MP3PLAYER_BUTTON_STATE_PUH;
					m_bButtonStateMain[MP3PLAYER_BUTTON_FULL] = TRUE;
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x>(MP3PLAYER_BASE_POS_X+238)
					&& pt.x<(MP3PLAYER_BASE_POS_X+238+MP3PLAYER_BUTTON_SIZE_W))
				{	// Ǯ���
					m_nButtonStateMain[MP3PLAYER_BUTTON_MINI] = MP3PLAYER_BUTTON_STATE_PUH;
					m_bButtonStateMain[MP3PLAYER_BUTTON_MINI] = TRUE;
					return INF_MSGPROC_BREAK;
				}
				else if(pt.x>(MP3PLAYER_BASE_POS_X)
					&& pt.x<(MP3PLAYER_BASE_POS_X+43)
					&& TRUE == m_bSetMp3Dir)
				{// ���丮 ��������					
					g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_ETC_0002,
								_Q_MP3_DIRECTORY);
				}
			}
			
			// ����Ʈ �ڽ� ��
			if(m_bMp3PlayListShow == TRUE)
			{
				// ��
				if( pt.y>(MP3PLAYER_BASE_POS_Y-19)
					&& pt.y<(MP3PLAYER_BASE_POS_Y-2))
				{
					for(int i=0; i<MP3PLAYER_BASE_TAB; i++)
					{
						if( pt.x>(MP3PLAYER_BASE_POS_X-1-(i*MP3PLAYER_TAB_WIDTH))
							&& pt.x<(MP3PLAYER_BASE_POS_X+MP3PLAYER_TAB_WIDTH+(i*MP3PLAYER_TAB_WIDTH)))
						{
							m_nSelectTab = i;
							if(m_nSelectTab == MP3PLAYER_TAB_RANDOM)
							{
								RandomPlayList();
							}
							g_pInterface->SetWindowOrder(WNDMp3Player);
							return INF_MSGPROC_BREAK;
						}
					}
				}

				// ����Ʈ
				if( pt.y>(MP3PLAYER_BASE_POS_Y-132)
					&& pt.y<(MP3PLAYER_BASE_POS_Y+2))
				{
					if( pt.x>(MP3PLAYER_BASE_POS_X-1)
						&& pt.x<(MP3PLAYER_BASE_POS_X+233))
					{
						// 2008-03-04 by dgwoo ������� ���� ����.
						for(int i = 0 ; i < MP3PLAYER_MAX_LIST ; i++)
						{
							if((MP3PLAYER_BASE_POS_Y - 130 + (i*MP3PLAYER_LINE_GAB)) < pt.y
								&& (MP3PLAYER_BASE_POS_Y-130+(i*MP3PLAYER_LINE_GAB) + MP3PLAYER_LINE_GAB) > pt.y)
							{
								if(m_vecMp3FileNames.size()>m_pScroll->GetCurrentScrollIndex()+i)
									m_nCurrentRenderSelect = m_vecMp3FileNames[m_pScroll->GetCurrentScrollIndex()+i].nIndex;
							}
						}
						g_pInterface->SetWindowOrder(WNDMp3Player);
						return INF_MSGPROC_BREAK;
					}
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			// �̴� ������ �϶��� �극��ũ 
			if(m_bBaseSizeFull == FALSE)
				break;
			
			//////////////////////////////////////////////////////////
			// ��ư ����
			if( pt.y>(MP3PLAYER_BASE_POS_Y)
			 && pt.y<(MP3PLAYER_BASE_POS_Y+MP3PLAYER_BUTTON_SIZE_H))
			{
				if( pt.x>(MP3PLAYER_BASE_POS_X+51)
					&& pt.x<(MP3PLAYER_BASE_POS_X+51+MP3PLAYER_BUTTON_SIZE_W))
				{	// �ڷ�
					if(m_bButtonStateMain[MP3PLAYER_BUTTON_PRVE] == TRUE)
					{
						//
						if(m_vecMp3FileNames.size() <= 0)
							break;
						m_LnMutePos = 0;
						m_bPlay_Mute = TRUE;
						if(m_nCurrentRenderIndex <= m_vecMp3FileNames[0].nIndex)
						{
							m_nCurrentRenderIndex = m_vecMp3FileNames[m_vecMp3FileNames.size()-1].nIndex;
							Play_Mp3Music();
							break;
						}
						
						BOOL bFalse = FALSE;
						vector<Mp3File_t>::iterator it = m_vecMp3FileNames.begin();
						while(it != m_vecMp3FileNames.end())
						{
							if(m_nCurrentRenderIndex == (*it).nIndex)
							{
								it--;
								m_nCurrentRenderIndex = (*it).nIndex;
								bFalse = TRUE;
								break;
							}
							
							it++;
						}
						if(FALSE == bFalse)
						{
							m_nCurrentRenderIndex = m_vecMp3FileNames[m_vecMp3FileNames.size()-1].nIndex;
						}
						Play_Mp3Music();

					}
				}
				else if( pt.x>(MP3PLAYER_BASE_POS_X+67)
					&& pt.x<(MP3PLAYER_BASE_POS_X+67+MP3PLAYER_BUTTON_SIZE_W))
				{	// ���, ����.
					if(m_bButtonStateMain[MP3PLAYER_BUTTON_PLAY] == TRUE)
					{
						if(m_vecMp3FileNames.size() <= 0)
						{
							g_pD3dApp->m_pSound->m_bPlayMusic = FALSE;
							m_bStop_Flag = FALSE;
							break;
						}
						if(m_bStop_Flag)
						{// ���� => ��� �ϰ�� ���õ� ������ �����Ų��.
							m_nCurrentRenderIndex = m_nCurrentRenderSelect;
							Play_Mp3Music();
							m_bStop_Flag = FALSE;
							break;
						}

						if(m_bPlay_Mute)
						{// ����.
							if(m_bOriBGSound)
							{
								Play_Mp3Music();
								m_bOriBGSound = FALSE;
							}
							else
							{
								// 2013-02-05 by bhsohn Sound ���� ��񿡼� Exception���� ���� ���� ����			
								if(g_pD3dApp->m_pSound && g_pD3dApp->m_pSound->IsSoundDevice())
								{
									g_pD3dApp->m_pSound->GetMusicMp3()->Atum_MusicStop();
									m_LnMutePos = g_pD3dApp->m_pSound->GetMusicMp3()->m_pLPos;
								}
								m_bPlay_Mute = FALSE;
							}
						}
						else
						{// ���.
							if(m_LnMutePos != 0)
							{
								Play_Mp3Music(TRUE);
								m_bPlay_Mute = 0;
							}
							else
							{
								Play_Mp3Music();
							}

//							if(m_vecMp3FileNames.size() <= 0)
//							{//
//								g_pD3dApp->m_pSound->GetMusicMp3()->Atum_MusicStop();
//								m_LnMutePos = g_pD3dApp->m_pSound->GetMusicMp3()->m_pLPos;
//								m_bStop_Flag = FALSE;
//								break;
//							}
//							else
//							{// mp3���� ����Ʈ�� �������.
//								m_nCurrentRenderIndex = m_nCurrentRenderSelect;
//								Play_Mp3Music();
//								m_bStop_Flag = FALSE;
//								break;
//							}
							m_bPlay_Mute = TRUE;
						}
						

//						if(m_bStop_Flag == FALSE)
//						{
//							m_nCurrentRenderIndex = m_nCurrentRenderSelect;
//							Play_Mp3Music();
//							m_bStop_Flag = FALSE;
//							break;
//						}
//						if(m_bPlay_Mute)
//						{
//							g_pD3dApp->m_pSound->GetMusicMp3()->Atum_MusicStop();
//							m_LnMutePos = g_pD3dApp->m_pSound->GetMusicMp3()->m_pLPos;
//							m_bPlay_Mute = FALSE;
//						}
//						else
//						{
//							if(m_LnMutePos != 0)
//							{
//								Play_Mp3Music(TRUE);
//								m_bPlay_Mute = 0;
//							}
//							else
//							{
//								Play_Mp3Music();
//							}
//							
//							m_bPlay_Mute = TRUE;
//						}
						
					}
				}
				else if( pt.x>(MP3PLAYER_BASE_POS_X+81)
					&& pt.x<(MP3PLAYER_BASE_POS_X+81+MP3PLAYER_BUTTON_SIZE_W))
				{	// ������
					if(m_bButtonStateMain[MP3PLAYER_BUTTON_NEXT] == TRUE)
					{
						if(m_vecMp3FileNames.size() <= 0)
							break;
						m_LnMutePos = 0;
						m_bPlay_Mute = TRUE;
						if(m_vecMp3FileNames.size()-1 <= m_nCurrentRenderIndex)
						{
							m_nCurrentRenderIndex = m_vecMp3FileNames[0].nIndex;
							Play_Mp3Music();
							break;
						}
						
						vector<Mp3File_t>::iterator it = m_vecMp3FileNames.begin();
						while(it != m_vecMp3FileNames.end())
						{
							if(m_nCurrentRenderIndex == (*it).nIndex)
							{
								it++;
								m_nCurrentRenderIndex = (*it).nIndex;								
								break;
							}
							
							it++;
						}
						
						Play_Mp3Music();
					}
				}
				else if( pt.x>(MP3PLAYER_BASE_POS_X+95)
					&& pt.x<(MP3PLAYER_BASE_POS_X+95+MP3PLAYER_BUTTON_SIZE_W))
				{	// ����
					if(m_bButtonStateMain[MP3PLAYER_BUTTON_STOP] == TRUE)
					{
						// 2013-02-05 by bhsohn Sound ���� ��񿡼� Exception���� ���� ���� ����			
						if(g_pD3dApp->m_pSound && g_pD3dApp->m_pSound->IsSoundDevice())
						{
							g_pD3dApp->m_pSound->GetMusicMp3()->Atum_MusicStop();
						}
						m_bMp3Play = FALSE;
						m_bStop_Flag = TRUE;
						m_bOriBGSound = FALSE;
					}
				}
				else if( pt.x>(MP3PLAYER_BASE_POS_X+225)
					&& pt.x<(MP3PLAYER_BASE_POS_X+225+MP3PLAYER_BUTTON_SIZE_W))
				{	// �÷��� ����Ʈ
					if(m_bButtonStateMain[MP3PLAYER_BUTTON_FULL] == TRUE)
					{
						m_bMp3PlayListShow = !m_bMp3PlayListShow;
						// ����Ʈ Ȱ���� �Ǹ� ������ �ڵ����� â �켱 ������ �־ ������ �ﵵ�� �Ѵ�.
						if(m_bMp3PlayListShow == TRUE)
							g_pInterface->SetWindowOrder(WNDMp3Player);
					}
				}
				else if( pt.x>(MP3PLAYER_BASE_POS_X+238)
					&& pt.x<(MP3PLAYER_BASE_POS_X+238+MP3PLAYER_BUTTON_SIZE_W))
				{	// Ǯ���
					if(m_bButtonStateMain[MP3PLAYER_BUTTON_MINI] == TRUE)
					{
						m_bBaseSizeFull = !m_bBaseSizeFull;
						if(m_bBaseSizeFull == FALSE)
							m_bMp3PlayListShow = FALSE;
					}
				}
			}
			
			for(int i=0; i<MP3PLAYER_BUTTON_MAIN_STATE; i++)
			{	// �ʱ�ȭ
				m_nButtonStateMain[i] = MP3PLAYER_BUTTON_STATE_NOR;
				m_bButtonStateMain[i] = FALSE;
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			// ����Ʈ �ڽ� ��(������� ����)
			if(m_bMp3PlayListShow == TRUE)
			{
				if( pt.y>(MP3PLAYER_BASE_POS_Y-132)
					&& pt.y<(MP3PLAYER_BASE_POS_Y+2))
				{
					if( pt.x>(MP3PLAYER_BASE_POS_X-1)
						&& pt.x<(MP3PLAYER_BASE_POS_X+233))
					{
						int nTemp=0;
						for(int i=0; i<MP3PLAYER_MAX_LIST+1; i++)
						{							
							if((MP3PLAYER_BASE_POS_Y-130+nTemp)<pt.y 
							&& (MP3PLAYER_BASE_POS_Y-130+nTemp+MP3PLAYER_LINE_GAB)>pt.y)
							{
								if(m_vecMp3FileNames.size()>m_pScroll->GetCurrentScrollIndex()+i-1)
									m_nCurrentRenderIndex = m_vecMp3FileNames[m_pScroll->GetCurrentScrollIndex()+i-1].nIndex;
								Play_Mp3Music();
							}

							nTemp = (i*MP3PLAYER_LINE_GAB);
						}
						return INF_MSGPROC_BREAK;
					}
				}
			}
		}
		break;
	case WM_KEYDOWN:
		{
			if(wParam == VK_DELETE)
			{
				if(m_bMp3PlayListShow == TRUE)
				{
					g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_ETC_0003,//"�����Ͻ� ���� �÷��� ����Ʈ���� �����Ͻðڽ��ϱ�?"
						_Q_MP3_PLAYLIST_DEL);
					return INF_MSGPROC_BREAK;					
				}
			}
		}
	}
#endif
	return INF_MSGPROC_NORMAL;
}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����

void CINFMp3Player::RenderButtonState(int posX, int posY)
{
	// ��ư ���� ������
	m_pImgPrvButton[m_nButtonStateMain[MP3PLAYER_BUTTON_PRVE]]->Move(posX + MP3PLAYER_BUTTON_BACK_X, posY + MP3PLAYER_BUTTON_START_Y);
	m_pImgPrvButton[m_nButtonStateMain[MP3PLAYER_BUTTON_PRVE]]->Render();
	m_pImgPlayButton[m_nButtonStateMain[MP3PLAYER_BUTTON_PLAY]]->Move(posX + MP3PLAYER_BUTTON_PLAY_X, posY + MP3PLAYER_BUTTON_START_Y);
	m_pImgPlayButton[m_nButtonStateMain[MP3PLAYER_BUTTON_PLAY]]->Render();
	m_pImgNextButton[m_nButtonStateMain[MP3PLAYER_BUTTON_NEXT]]->Move(posX + MP3PLAYER_BUTTON_PP_X, posY + MP3PLAYER_BUTTON_START_Y);
	m_pImgNextButton[m_nButtonStateMain[MP3PLAYER_BUTTON_NEXT]]->Render();
	m_pImgStopButton[m_nButtonStateMain[MP3PLAYER_BUTTON_STOP]]->Move(posX + MP3PLAYER_BUTTON_STOP_X, posY + MP3PLAYER_BUTTON_START_Y);
	m_pImgStopButton[m_nButtonStateMain[MP3PLAYER_BUTTON_STOP]]->Render();
	
	if(m_bMp3PlayListShow == FALSE)
	{
		m_pImgShowListButton[m_nButtonStateMain[MP3PLAYER_BUTTON_FULL]]->Move(posX + MP3PLAYER_BUTTON_PLAYLIST,posY + MP3PLAYER_BUTTON_START_Y);
		m_pImgShowListButton[m_nButtonStateMain[MP3PLAYER_BUTTON_FULL]]->Render();
	}
	else
	{
		m_pImgHidenButton[m_nButtonStateMain[MP3PLAYER_BUTTON_FULL]]->Move(posX + MP3PLAYER_BUTTON_PLAYLIST,posY + MP3PLAYER_BUTTON_START_Y);
		m_pImgHidenButton[m_nButtonStateMain[MP3PLAYER_BUTTON_FULL]]->Render();
	}
}
#else
void CINFMp3Player::RenderButtonState()
{
	// ��ư ���� ������
	m_pImgPrvButton[m_nButtonStateMain[MP3PLAYER_BUTTON_PRVE]]->Move(MP3PLAYER_BASE_POS_X+51,MP3PLAYER_BASE_POS_Y);
	m_pImgPrvButton[m_nButtonStateMain[MP3PLAYER_BUTTON_PRVE]]->Render();
	m_pImgPlayButton[m_nButtonStateMain[MP3PLAYER_BUTTON_PLAY]]->Move(MP3PLAYER_BASE_POS_X+67,MP3PLAYER_BASE_POS_Y);
	m_pImgPlayButton[m_nButtonStateMain[MP3PLAYER_BUTTON_PLAY]]->Render();
	m_pImgNextButton[m_nButtonStateMain[MP3PLAYER_BUTTON_NEXT]]->Move(MP3PLAYER_BASE_POS_X+81,MP3PLAYER_BASE_POS_Y);
	m_pImgNextButton[m_nButtonStateMain[MP3PLAYER_BUTTON_NEXT]]->Render();
	m_pImgStopButton[m_nButtonStateMain[MP3PLAYER_BUTTON_STOP]]->Move(MP3PLAYER_BASE_POS_X+95,MP3PLAYER_BASE_POS_Y);
	m_pImgStopButton[m_nButtonStateMain[MP3PLAYER_BUTTON_STOP]]->Render();
	
	if(m_bMp3PlayListShow == FALSE)
	{
		m_pImgShowListButton[m_nButtonStateMain[MP3PLAYER_BUTTON_FULL]]->Move(MP3PLAYER_BASE_POS_X+225,MP3PLAYER_BASE_POS_Y);
		m_pImgShowListButton[m_nButtonStateMain[MP3PLAYER_BUTTON_FULL]]->Render();
	}
	else
	{
		m_pImgHidenButton[m_nButtonStateMain[MP3PLAYER_BUTTON_FULL]]->Move(MP3PLAYER_BASE_POS_X+225,MP3PLAYER_BASE_POS_Y);
		m_pImgHidenButton[m_nButtonStateMain[MP3PLAYER_BUTTON_FULL]]->Render();
	}
	m_pImgMiniButton[m_nButtonStateMain[MP3PLAYER_BUTTON_MINI]]->Move(MP3PLAYER_BASE_POS_X+238,MP3PLAYER_BASE_POS_Y);
	m_pImgMiniButton[m_nButtonStateMain[MP3PLAYER_BUTTON_MINI]]->Render();
}
#endif

void CINFMp3Player::FindMp3File()
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	char fname[MAX_PATH];
	BOOL bResult=TRUE;
	char Path[MAX_PATH];
	int  nCnt=0;
	Mp3File_t tempMp3Name;

	// 2013-02-05 by bhsohn Sound ���� ��񿡼� Exception���� ���� ���� ����			
	if(g_pD3dApp->m_pSound && !g_pD3dApp->m_pSound->IsSoundDevice())
	{
		return;
	}
	// END 2013-02-05 by bhsohn Sound ���� ��񿡼� Exception���� ���� ���� ����

	// 2006-09-15 by ispark
	if(g_pD3dApp->m_pSound->GetMusicMp3() == NULL)
	{
		return;
	}

	g_pD3dApp->m_pSound->GetMusicMp3()->Atum_MusicStop();
	m_vecMp3FileNames.clear();
	if(strlen(m_strDirectory)>0)
	{
		memset(Path, 0x00, MAX_PATH);
		strncpy(Path, m_strDirectory, strlen(m_strDirectory));
	}
	else
	{
		GetCurrentDirectory( MAX_PATH, Path );
		strcat(Path, "\\Music\\*.mp3");
		strncpy(m_strDirectory,Path, strlen(Path));
	}

	hSrch = FindFirstFile(Path, &wfd);
	if(hSrch == INVALID_HANDLE_VALUE)
	{
		DBGOUT("FindFirstFile : INVALID_HANDLE_VALUE = %d\n",GetLastError());
		FindClose(hSrch);
		return ;
	}
	while(bResult)
	{
		memset(&tempMp3Name, 0x00, sizeof(Mp3File_t));
		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			wsprintf(fname, "[ %s ]", wfd.cFileName);
		}
		else
		{
			wsprintf(fname, "%s", wfd.cFileName);
			tempMp3Name.nIndex = nCnt;
			strncpy(tempMp3Name.szFileNameString, fname, strlen(fname));
			m_vecMp3FileNames.push_back(tempMp3Name);
		}
		nCnt++;
		bResult = FindNextFile(hSrch, &wfd);
	}
	m_pScroll->SetNumberOfData( m_vecMp3FileNames.size() );
	m_nCurrentRenderIndex = 0;
	m_nCurrentRenderSelect = 0;
	FindClose(hSrch);

	vector<Mp3File_t>::iterator it = m_vecMp3FileNames.begin();
	while(it != m_vecMp3FileNames.end())
	{
		DBGOUT("MP3 File : %s\n", (*it).szFileNameString);
		it++;
	}
}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
void CINFMp3Player::RenderPlayListBox(int posX, int posY)
{
	m_pImgListBox->Move( posX + MP3PLAYER_PLATLIST_START_X, posY + MP3PLAYER_PLATLIST_START_Y );
	m_pImgListBox->Render();
	
	// �� ǥ��
	m_pImgTab[m_nSelectTab]->Move( posX + MP3PLAYER_PLATLIST_START_X, posY + MP3PLAYER_PLATLIST_START_Y);
	m_pImgTab[m_nSelectTab]->Render();

	m_pScroll->SetScrollLinePos( posX + MP3PLAYER_LIST_POS_X, posY + MP3PLAYER_LIST_POS_Y );
	m_pScroll->SetWheelRect( posX + MP3PLAYER_PLATLIST_START_X, 
							 posY + MP3PLAYER_PLATLIST_START_Y,
							 posX + MP3PLAYER_PLATLIST_START_X + 290,
							 posY + MP3PLAYER_PLATLIST_START_Y + 133);
	m_pScroll->Render();
	
	int nLineGab = 0;
	for(int i=0; i<MP3PLAYER_MAX_LIST; i++)
	{
		if(m_vecMp3FileNames.size() > m_pScroll->GetCurrentScrollIndex() +i)
		{			
			// ���� ǥ��
			if(m_nCurrentRenderSelect == m_vecMp3FileNames[m_pScroll->GetCurrentScrollIndex()+i].nIndex)
			{
				m_pImgSelectMp3->Move( posX + MP3PLAYER_PLATLIST_START_X, posY + MP3PLAYER_PLATLIST_START_Y + 25 + nLineGab);
				m_pImgSelectMp3->Scaling(0.95f, 1.0f);		// 2012-03-20 by mspark, mp3 �߰� ����
				m_pImgSelectMp3->Render();
			}

			vector<string> vecstr;
			StringCullingUserDataEx(m_vecMp3FileNames[m_pScroll->GetCurrentScrollIndex()+i].szFileNameString, 283, &vecstr, m_pFontTitle[0]);
			strncpy(m_strMp3Name, vecstr[0].c_str(), MAX_PATH );

			// ��� ǥ��
			if(m_nCurrentRenderIndex == m_vecMp3FileNames[m_pScroll->GetCurrentScrollIndex()+i].nIndex)
			{
				if(m_nCurrentRenderSelect == m_vecMp3FileNames[m_pScroll->GetCurrentScrollIndex()+i].nIndex)
				{
					m_pFontTitle[0]->DrawText(posX + MP3PLAYER_PLATLIST_START_X + 5, posY + MP3PLAYER_PLATLIST_START_Y + 25 + nLineGab, GUI_FONT_COLOR_G,
						m_strMp3Name);
				}
				else
				{
					m_pFontTitle[0]->DrawText(posX + MP3PLAYER_PLATLIST_START_X + 5, posY + MP3PLAYER_PLATLIST_START_Y + 25 + nLineGab, GUI_FONT_COLOR_YM,
						m_strMp3Name);
				}
			}
			else
			{
				m_pFontTitle[0]->DrawText(posX + MP3PLAYER_PLATLIST_START_X + 5, posY + MP3PLAYER_PLATLIST_START_Y + 25 + nLineGab, GUI_SELECT_FONT_COLOR,	m_strMp3Name);		
			}
			
			nLineGab += MP3PLAYER_LINE_GAB;			
		}
	}
	
}
#else
void CINFMp3Player::RenderPlayListBox()
{
	m_pImgListBox->Move(MP3PLAYER_BASE_POS_X-1, MP3PLAYER_BASE_POS_Y-149);
	m_pImgListBox->Render();
	
	// �� ǥ��
	m_pImgTab[m_nSelectTab]->Move(MP3PLAYER_BASE_POS_X-1, MP3PLAYER_BASE_POS_Y-19);
	m_pImgTab[m_nSelectTab]->Render();
	
	int nLineGab = 0;
	for(int i=0; i<MP3PLAYER_MAX_LIST; i++)
	{
		if(m_vecMp3FileNames.size() > m_pScroll->GetCurrentScrollIndex() +i)
		{			
			// ���� ǥ��
			if(m_nCurrentRenderSelect == m_vecMp3FileNames[m_pScroll->GetCurrentScrollIndex()+i].nIndex)
			{
				m_pImgSelectMp3->Move(MP3PLAYER_BASE_POS_X+5, MP3PLAYER_BASE_POS_Y-130+nLineGab);
				m_pImgSelectMp3->Render();
			}
			if(strlen(m_vecMp3FileNames[m_pScroll->GetCurrentScrollIndex()+i].szFileNameString)>37)
			{
				FileNameCull(m_vecMp3FileNames[m_pScroll->GetCurrentScrollIndex()+i].szFileNameString);
			}
			else
			{
				memset(m_strMp3Name, 0x00, MAX_PATH);
				strncpy(m_strMp3Name, m_vecMp3FileNames[m_pScroll->GetCurrentScrollIndex()+i].szFileNameString, strlen(m_vecMp3FileNames[m_pScroll->GetCurrentScrollIndex()+i].szFileNameString));
			}
			// ��� ǥ��
			if(m_nCurrentRenderIndex == m_vecMp3FileNames[m_pScroll->GetCurrentScrollIndex()+i].nIndex)
			{
				if(m_nCurrentRenderSelect == m_vecMp3FileNames[m_pScroll->GetCurrentScrollIndex()+i].nIndex)
				{					
					m_pFontTitle[i]->DrawText(MP3PLAYER_BASE_POS_X+5, MP3PLAYER_BASE_POS_Y-MP3PLAYER_BASE_POS_MINUS+nLineGab, GUI_FONT_COLOR_G,
						m_strMp3Name);
				}
				else
				{
					m_pFontTitle[i]->DrawText(MP3PLAYER_BASE_POS_X+5, MP3PLAYER_BASE_POS_Y-MP3PLAYER_BASE_POS_MINUS+nLineGab, GUI_FONT_COLOR_YM,
						m_strMp3Name);
				}
			}
			else
				m_pFontTitle[i]->DrawText(MP3PLAYER_BASE_POS_X+5, MP3PLAYER_BASE_POS_Y-MP3PLAYER_BASE_POS_MINUS+nLineGab, GUI_SELECT_FONT_COLOR,
					m_strMp3Name);		
			
			nLineGab += MP3PLAYER_LINE_GAB;			
		}
	}

//	vector<Mp3File_t>::iterator it = m_vecMp3FileNames.begin();
//	while(it != m_vecMp3FileNames.end())
//	{
//		if(nBreakCnt>MP3PLAYER_MAX_LIST)
//			break;
//		it++;
//		nBreakCnt++;
//	}
}
#endif

void CINFMp3Player::DeleteSelectPlayList()
{
	vector<Mp3File_t>::iterator it = m_vecMp3FileNames.begin();
	while(it != m_vecMp3FileNames.end())
	{
		if(m_nCurrentRenderSelect == (*it).nIndex)
		{
			it = m_vecMp3FileNames.erase(it);
			continue;
		}
		
		it++;
	}	
}

void CINFMp3Player::Play_Mp3Music(BOOL bMute)
{
// 2012-10-15 by jhahn ��� ���� mp3�÷��� ���� ����
	if (FALSE == g_pGameMain->m_bMp3PlayFlag)
	{
		// 2013-02-05 by bhsohn Sound ���� ��񿡼� Exception���� ���� ���� ����			
		if(g_pD3dApp->m_pSound && g_pD3dApp->m_pSound->IsSoundDevice())
		{
			g_pD3dApp->m_pSound->m_bPlayMusic = FALSE;			
			m_bMp3Play = FALSE;
			m_bOriBGSound = TRUE;				
			return;
		}
		// END 2013-02-05 by bhsohn Sound ���� ��񿡼� Exception���� ���� ���� ����			
		g_pD3dApp->m_pSound->m_bPlayMusic = FALSE;
		g_pD3dApp->m_pSound->GetMusicMp3()->m_bNowPlay = FALSE;
		m_bMp3Play = FALSE;
		m_bOriBGSound = TRUE;		
		g_pD3dApp->m_pSound->PlayBackSound();
				
		return;	
	}
//end 2012-10-15 by jhahn ��� ���� mp3�÷��� ���� ����

	char buffDir[1024];
	memset(buffDir, 0x00, 1024);
	char buffDir2[1024];
	memset(buffDir2, 0x00, 1024);
	
	g_pD3dApp->m_pSound->SetDefaultMusicLoop(FALSE);
	vector<Mp3File_t>::iterator it = m_vecMp3FileNames.begin();
	while(it != m_vecMp3FileNames.end())
	{
		if(m_nCurrentRenderIndex == (*it).nIndex)
		{
			// ���� �ڿ����� (*.mp3) ���� 
			strncpy(buffDir, m_strDirectory, strlen(m_strDirectory)-5);
			strcat(buffDir, (*it).szFileNameString);
			
			break;
		}
		
		it++;
	}	
	// 2013-02-05 by bhsohn Sound ���� ��񿡼� Exception���� ���� ���� ����			
	if(g_pD3dApp->m_pSound && g_pD3dApp->m_pSound->IsSoundDevice())
	{
		g_pD3dApp->m_pSound->GetMusicMp3()->Atum_MusicStop();
	}
	if(buffDir != NULL)	
	{
		// 2012-11-15 by jhahn ���� ���ǵ� �� ���� ����
		// 2013-02-05 by bhsohn Sound ���� ��񿡼� Exception���� ���� ���� ����					
//		if (g_pD3dApp->m_pSound->GetMusicMp3()->Atum_MusicInit(buffDir) == E_FAIL)
		if ((g_pD3dApp->m_pSound && g_pD3dApp->m_pSound->IsSoundDevice()) && g_pD3dApp->m_pSound->GetMusicMp3()->Atum_MusicInit(buffDir) == E_FAIL)
		{	
			vector<Mp3File_t>::iterator it = m_vecMp3FileNames.begin();
			while(it != m_vecMp3FileNames.end())
			{
				memset(buffDir2, 0x00, 1024);
				strncpy(buffDir2, m_strDirectory, strlen(m_strDirectory)-5);
				strcat(buffDir2, (*it).szFileNameString);

				if(strcmp(buffDir,buffDir2) == false)
				{
					it = m_vecMp3FileNames.erase(it);				
					continue;
				}			
				it++;
				
			}	
		}
	}
	//end 2012-11-15 by jhahn ���� ���ǵ� �� ���� ����
		
	// 2006-03-22 by ispark
	int nVolume = GetMusicVolume(g_pSOption->sMusicVolume);
//	g_pD3dApp->m_pSound->GetMusicMp3()->Atum_PlayMP3(g_pSOption->sMusicVolume);
	// 2013-02-05 by bhsohn Sound ���� ��񿡼� Exception���� ���� ���� ����			
	if(g_pD3dApp->m_pSound && g_pD3dApp->m_pSound->IsSoundDevice())
	{
		g_pD3dApp->m_pSound->GetMusicMp3()->Atum_PlayMP3(nVolume);
	}
	if(g_pD3dApp->m_pSound->m_bPlayMusic && g_pD3dApp->m_pSound->GetMusicMp3())
	{
		// 2013-02-05 by bhsohn Sound ���� ��񿡼� Exception���� ���� ���� ����			
		if(g_pD3dApp->m_pSound && g_pD3dApp->m_pSound->IsSoundDevice())
		{
			if(bMute)
				g_pD3dApp->m_pSound->GetMusicMp3()->Atum_LoopMusic(m_LnMutePos);
			else
				g_pD3dApp->m_pSound->GetMusicMp3()->Atum_LoopMusic();
		}
	}
	g_pD3dApp->m_pSound->m_bPlayMusic = TRUE;
	// 2013-02-05 by bhsohn Sound ���� ��񿡼� Exception���� ���� ���� ����			
	if(g_pD3dApp->m_pSound && g_pD3dApp->m_pSound->IsSoundDevice())
	{
		g_pD3dApp->m_pSound->GetMusicMp3()->m_bNowPlay = TRUE;
	}
	m_bMp3Play = TRUE;
	m_bOriBGSound = FALSE;
}

void CINFMp3Player::FileNameCull(char *strName)
{
	vector<string> vecTemp;
	StringCullingUserData(strName, 35, &vecTemp);	// ��ü ��Ʈ�� ���ܳ���
	if(vecTemp.size()>0)
	{
		char buff[MAX_PATH];
		memset(buff, 0x00, MAX_PATH);
		
		strncpy(buff, vecTemp[0].c_str(), strlen(vecTemp[0].c_str()));
		strcat(buff, "..");
		memset(m_strMp3Name, 0x00, MAX_PATH);
		strncpy(m_strMp3Name, buff, strlen(buff));
	}
}

void CINFMp3Player::RandomPlayList()
{
	m_vecRandom.clear();
	int nNameSize = m_vecMp3FileNames.size();
	if(nNameSize <= 1)
		return;
	BOOL nLoop = TRUE;
	BOOL bFush = FALSE;

	int nVelRate = RANDI(0, nNameSize - 1);
	m_vecRandom.push_back(nVelRate);
	
	while(nLoop)
	{
		bFush = FALSE;
		nVelRate = RANDI(0, nNameSize - 1);
		vector<int>::iterator it = m_vecRandom.begin();
		while(it != m_vecRandom.end())
		{
			if(*it == nVelRate)
			{
				bFush = TRUE;
			}
			it++;
		}
		
		if(bFush == FALSE)
		{
			m_vecRandom.push_back(nVelRate);
			if(m_vecRandom.size() == nNameSize)
				nLoop = FALSE;
		}
	}	
}

