// INFLuckyMachine.cpp: implementation of the CINFLuckyMachine class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GameDataLast.h"
#include "INFImage.h"
#include "INFLuckyMachine.h"
#include "AtumDatabase.h"
#include "AtumApplication.h"
#include "INFGameMain.h"
#include "INFIcon.h"
#include "ItemInfo.h"
#include "FieldWinSocket.h"
#include "INFCityBase.h"
#include "StoreData.h"
#include "Chat.h"
#include "INFInvenExtend.h"
#include "ShuttleChild.h"
#include "AtumSound.h"
#include "INFImageEx.h"										   // 2011. 10. 10 by jskim UI�ý��� ����
	
#define LUCKYMACHINE_BLANK_ICON_NUMBER			7020630

#define LUCKYMACHINE_EFFECT_FRAME_CHANGE_TIME				0.5f

// 2010. 06. 03 by jskim �����ӽ�, ���Ǹӽ� ����
#define LUCKYMACHINE_MAIN_BG_X					(CITY_BASE_NPC_BOX_START_X + 420)
#define LUCKYMACHINE_MAIN_BG_Y					(CITY_BASE_NPC_BOX_START_Y - 440)

#define	LUCKYMACHINE_EFFECT_CH_BG_X				(CITY_BASE_NPC_BOX_START_X)
#define LUCKYMACHINE_EFFECT_CH_BG_Y				(LUCKYMACHINE_MAIN_BG_Y - 192)

#define	LUCKYMACHINE_EFFECT_BG_X				(CITY_BASE_NPC_BOX_START_X -7)
#define LUCKYMACHINE_EFFECT_BG_Y				(LUCKYMACHINE_MAIN_BG_Y - 148)

#define LUCKYMACHINE_SLOT_X						(LUCKYMACHINE_MAIN_BG_X + 43 + 30)
#define LUCKYMACHINE_SLOT_Y						(LUCKYMACHINE_MAIN_BG_Y + 34 + 120)

#define LUCKYMACHINE_SLOT_GAP_X					32
#define LUCKYMACHINE_SLOT_GAP_Y					52

#define	LUCKYMACHINE_COIN_X						(LUCKYMACHINE_MAIN_BG_X + 140 + 30)
#define	LUCKYMACHINE_COIN_Y						(LUCKYMACHINE_MAIN_BG_Y + 191 + 120)

#define ICON_SIZE_W								30
#define ICON_SIZE_H								30

#define LUCKYMACHINE_GAMBLE_BUTTON_X			(LUCKYMACHINE_MAIN_BG_X + 141)
#define LUCKYMACHINE_GAMBLE_BUTTON_Y			(LUCKYMACHINE_MAIN_BG_Y + 240 + 126)

#define LUCKYMACHINE_TAKE_BUTTON_X				(LUCKYMACHINE_MAIN_BG_X + 198)
#define LUCKYMACHINE_TAKE_BUTTON_Y				(LUCKYMACHINE_MAIN_BG_Y + 240 + 126)

// 2009-03-04 by bhsohn ��Ű �ӽ� ������
// �ڷ� ����
#define LUCKYMACHINE_PREV_X			(CITY_BASE_NPC_BOX_START_X+613)
#define LUCKYMACHINE_PREV_Y			(CITY_BASE_NPC_BOX_START_Y+79)

///////////// ��Ű �ӽ� ����
#define LUCKYMACHINE_SELECT_TYPE_BG_X					(CITY_BASE_NPC_BOX_START_X)
#define LUCKYMACHINE_SELECT_TYPE_BG_Y					(CITY_BASE_NPC_BOX_START_Y - 451)

#define LUCKYMACHINE_SELECT_TYPE_NORMAL_X			(LUCKYMACHINE_SELECT_TYPE_BG_X + 93)
#define LUCKYMACHINE_SELECT_TYPE_NORMAL_Y			(LUCKYMACHINE_SELECT_TYPE_BG_Y + 155)

#define LUCKYMACHINE_SELECT_TYPE_MINI_X				(LUCKYMACHINE_SELECT_TYPE_BG_X + 349)
#define LUCKYMACHINE_SELECT_TYPE_MINI_Y				(LUCKYMACHINE_SELECT_TYPE_BG_Y + 155)

///////////// ��Ű �ӽ� ���� ����

#define		MAX_ONEPAGE_NPCLIST_COUNT							5		// �������� �ִ� NPC��

#define LUCKYMACHINE_SELECT_SHOP_BG_X					(CITY_BASE_NPC_BOX_START_X)
#define LUCKYMACHINE_SELECT_SHOP_BG_Y					(CITY_BASE_NPC_BOX_START_Y - 276)

#define LUCKYMACHINE_SELECT_SHOP_PREV_X					(LUCKYMACHINE_SELECT_SHOP_BG_X + 44)
#define LUCKYMACHINE_SELECT_SHOP_PREV_Y					(LUCKYMACHINE_SELECT_SHOP_BG_Y + 120)

#define LUCKYMACHINE_SELECT_SHOP_NEXT_X					(LUCKYMACHINE_SELECT_SHOP_BG_X + 768)
#define LUCKYMACHINE_SELECT_SHOP_NEXT_Y					(LUCKYMACHINE_SELECT_SHOP_BG_Y + 120)

#define LUCKYMACHINE_SELECT_SHOP_NPC_X					(LUCKYMACHINE_SELECT_SHOP_BG_X + 148)
#define LUCKYMACHINE_SELECT_SHOP_NPC_Y					(LUCKYMACHINE_SELECT_SHOP_BG_Y + 87)
#define LUCKYMACHINE_SELECT_SHOP_NPC_WIDTH				(117)


// ���� ���� ���� ������ ���ؽ�
#define NOT_OPEN_LUCKY_MACHINE						(999)

// ���� Index
// #define LUCKY_MACHINE_BUILD_INDEX_BCU							(9348)		// ���������� ��Ű(2001)
// #define LUCKY_MACHINE_BUILD_INDEX_ANI							(9349)		// �˸��� ��Ű(2002)
// #define LUCKY_MACHINE_BUILD_INDEX_NOR							(9350)		// �Ϲ� ���� ��Ű(2003)


#define MAX_LUCKY_MACHINE_TIME							(9.0f)		// ��Ű�ӽ� �ִ� �ð�
#define OVERTIME_LUCKY_MACHINE_STEP						(LUCKY_ITEM_LIST_NUM+LUCKY_ITEM_POS+1)		// ��Ű�ӽ� �ð� �ʰ��� ����

// ���� ���ܵ�
#define FIRST_SLOT_MACHINE_SUCCESS_STEP					(22)		// ù��° ����
#define SECOND_SLOT_MACHINE_SUCCESS_STEP				(22)		
#define THIRD_SLOT_MACHINE_SUCCESS_STEP					(22)

#define		MAX_YOU_LUCKY_TIME							5.0f
#define		MAX_YOU_DELAY_LUCKY_TIME					4.0f

#define	PRIZE_LUCKY_ITEM_X								200
#define	PRIZE_LUCKY_ITEM_Y								200


// end 2009-03-04 by bhsohn ��Ű �ӽ� ������

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFLuckyMachine::CINFLuckyMachine(CAtumNode* pParent, BUILDINGNPC* pBuilding)
{
	m_pParent = pParent;	// CGameMain*
	m_pGameData = NULL;
	//m_pBuildingInfo = pBuilding;

	m_vecTakeItem.clear();
	m_pBtnGamble	= NULL;
	m_pBtnTake		= NULL;

	// 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
	m_pBtnGamble1	= NULL;
	m_pBtnTake1		= NULL;
	//end 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����

//	m_pImgArrow		= NULL;

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_LUCKY_TITLE_IMAGE;nCnt++)
	{
		m_pImgEffectBG[nCnt]	= NULL;
	}
	for(nCnt = 0;nCnt < MAX_LUCKY_YOULUCKY;nCnt++)
	{
		m_pImgYouLuckyBG[nCnt]	= NULL;
	}
	

	
	m_fTimeEffectBG		= 0.0f;
	m_nEffectBG			= 0;

	m_fLuckyTimeEffectBG = 0.0f;
	m_nLuckyEffectBG	= 0;

	for(nCnt = 0; nCnt < 3; nCnt++)
	{
		m_pImgMainBG[nCnt]	= NULL;
	}
	
	m_pImgQuestionMark	= NULL;

	m_nState			= LUCKY_STATE_READY;	
	///////////////////////////////////////
	InitRulType();	// �귵 ���� ���� ��Ģ�� ���Ѵ�.	
	///////////////////////////////////////

	// 2009-03-04 by bhsohn ��Ű �ӽ� ������
	m_nLuckyMechineStep = LUCKY_MAINSTEP_SELECT_SHOP;	
	m_nLuckySelectStartIdx = 0;	

	m_pImgSelctShopBK = NULL;
	m_pSelectShopPrevBtn = NULL;
	m_pSelectShopNextBtn = NULL;

	m_pPrevStepBtn = NULL;
	
	m_vecstruShopList.clear();				// NPC����Ʈ	
	memset(&m_struSelLuckyMachine, 0x00, sizeof(LUCKY_MACHINE_OMI));			// ������ ��Ű ����

	// 2009-05-13 by bhsohn ��Ű ���� ���� ����
	m_uLoadBuildingIndex=0;
	// end 2009-05-13 by bhsohn ��Ű ���� ���� ����

	m_nLoadNpcListInfluenceType = -1;
	int i =0;
	for(i = 0; i < LUCKY_ITEM_LIST_NUM_LINE; i++)
	{		
		m_fTimeHold[i]		= 0.0f;
		m_fTimeOneStepSlot[i]		= 0.0f;
		m_fTimeAllTimeStepSlot[i]		= 0.0f;
		m_nSuccessSlot[i]				= 0 ;		
		m_nSlotStartPosIdx[i]			= 0;
		m_nSlotType[i]			= 0;
		m_bSlotDone[i]		= FALSE;
		m_nSlotStep[i]		= 0;
		m_bCompleteStep[i]	=FALSE;
	}
	m_pCoinItemInfo = NULL;
	m_bInfluenceOneShopMode = FALSE;
	m_uBuildingIndex = 0;
	// end 2009-03-04 by bhsohn ��Ű �ӽ� ������

	// 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
	m_bYouLucky = FALSE;	
	m_fYouLuckyViewTime= 0.0f;
	m_dwYouLuckyAlpha= 0;
	// end 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��


	// 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����
	m_dwLastFrameTime	= 0;
	m_nCurFrame			= 0;
	// end 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����

	// 2009-05-18 by bhsohn ��Ű�������� �κ�Ǯ�ϋ��� ������ ó��
	m_pItemCoinItemTmp = NULL;	
	// end 2009-05-18 by bhsohn ��Ű�������� �κ�Ǯ�ϋ��� ������ ó��

	// 2010. 05. 12 by jskim �ű� ��Ű �ӽ� �߰���� - ��Ű�ӽſ��� �޴� �����۸� �����ֱ�
	m_pImgDisableItem = NULL;
	//end 2010. 05. 12 by jskim �ű� ��Ű �ӽ� �߰���� - ��Ű�ӽſ��� �޴� �����۸� �����ֱ�
}

CINFLuckyMachine::~CINFLuckyMachine()
{	
	ClearSearchItem();	
	util::del(m_pCoinItemInfo);
	// 2009-05-18 by bhsohn ��Ű�������� �κ�Ǯ�ϋ��� ������ ó��
	util::del(m_pItemCoinItemTmp);	
	// end 2009-05-18 by bhsohn ��Ű�������� �κ�Ǯ�ϋ��� ������ ó��
}

void CINFLuckyMachine::InitRulType()
{
	int nRulType = 0;
	int nSlot = 0;
	int nSlotStep = 0;

	for(nSlot = 0;nSlot < LUCKY_ITEM_LIST_NUM_LINE;nSlot++)
	{
		for(nRulType = 0;nRulType < MAX_SLOT_RULLET_TYPE;nRulType++)
		{
			for(nSlotStep = 0;nSlotStep < MAX_SLOT_RULLET_STEP;nSlotStep++)
			{			
				m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= -1.0f;				
				m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.0f;
				m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
				m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].bCompleteStep	= FALSE;
			}
		}
	}

	
	for(nSlot = 0;nSlot < LUCKY_ITEM_LIST_NUM_LINE;nSlot++)
	{
		nRulType = 0;				
		
		{			
			// �⺻ Ÿ��(1)
			nSlotStep = 0;

			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= 1.3f;				
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.0f;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
			nSlotStep++;

			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= 0.2f;				
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.02f;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
			nSlotStep++;			

			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= 0.5f;				
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.05f;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].bCompleteStep	= TRUE;
			nSlotStep++;						

			nRulType++;
		}		

		{
			// �⺻ Ÿ��(2)
			nSlotStep = 0;
			
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= 1.7f;				
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.0f;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
			nSlotStep++;
			
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= 0.3f;				
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.03f;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
			nSlotStep++;
			

			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= 0.6f;				
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.06f;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].bCompleteStep	= TRUE;
			nSlotStep++;
			
			nRulType++;
		}
		
		{
			// �⺻ Ÿ��(3)
			nSlotStep = 0;
			
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= 2.0f;				
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.0f;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
			nSlotStep++;
			
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= 0.4f;				
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.04f;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
			nSlotStep++;
			
			
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= 0.7f;				
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.07f;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].bCompleteStep	= TRUE;
			
			nRulType++;
		}		
	}
}
void CINFLuckyMachine::ResetData()
{
	int i;
	// 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
// 	if(m_pCoinItemInfo)
// 	{
// 		//m_pCoinItemInfo->CurrentCount = 1;				// ��ȭ �������� �Ѱ��� �ø����ִ�.
// 		//g_pStoreData->PutItem((char*)((ITEM_GENERAL*)m_pCoinItemInfo), FALSE);
// 		g_pStoreData->UpdateItemCount(m_pCoinItemInfo->UniqueNumber, m_pCoinItemInfo->CurrentCount + 1);		
// 		util::del(m_pCoinItemInfo);
// 	}
	ClearConinItemInfo(); // �������� �ʱ�ȭ
	// end 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
	for(i = 0; i < LUCKY_ITEM_LIST_NUM_LINE; i++)
	{		
		m_fTimeHold[i]		= 0.0f;
		m_fTimeOneStepSlot[i]		= 0.0f;
		m_fTimeAllTimeStepSlot[i]		= 0.0f;
		m_nSuccessSlot[i]				= 0;		
		m_nSlotStartPosIdx[i]			= 0;		
		m_bSlotDone[i]		= FALSE;
		m_nSlotStep[i]		= 0;
		m_bCompleteStep[i]	=FALSE;
	}	
	ClearSearchItem();
	m_nState = LUCKY_STATE_READY;
	Init();
	
}
HRESULT CINFLuckyMachine::InitDeviceObjects()
{
	DataHeader	* pDataHeader;

	m_pImgMainBG[2] = new CINFImageEx;
	//pDataHeader = FindResource("AM_BG");
	pDataHeader = FindResource("AME_BG_0");
	m_pImgMainBG[2]->InitDeviceObjects(pDataHeader);
	m_pImgMainBG[2]->SetUVAnimation(4,2,0.07f);
	m_pImgMainBG[2]->SetLoop(TRUE);

	m_pImgMainBG[1] = new CINFImageEx;
	//pDataHeader = FindResource("WM_BG");
	pDataHeader = FindResource("WME_BG_0");
	m_pImgMainBG[1]->InitDeviceObjects(pDataHeader);
	m_pImgMainBG[1]->SetUVAnimation(4,2,0.07f);
	m_pImgMainBG[1]->SetLoop(TRUE);


	m_pImgMainBG[0] = new CINFImageEx;
	//pDataHeader = FindResource("HM_BG");
	pDataHeader = FindResource("HME_BG_0");
	m_pImgMainBG[0]->InitDeviceObjects(pDataHeader);
	m_pImgMainBG[0]->SetUVAnimation(4,2,0.07f);
	m_pImgMainBG[0]->SetLoop(TRUE);

	m_pImgMainBG[3] = new CINFImageEx;
	//pDataHeader = FindResource("HM_BG");
	pDataHeader = FindResource("MME_BG_0");
	m_pImgMainBG[3]->InitDeviceObjects(pDataHeader);
	m_pImgMainBG[3]->SetUVAnimation(4,2,0.07f);
	m_pImgMainBG[3]->SetLoop(TRUE);




	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_LUCKY_TITLE_IMAGE;nCnt++)
	{	
		char chTmp[32];
		wsprintf(chTmp, "LMEBG%d", nCnt);
		pDataHeader = FindResource(chTmp);
		if(pDataHeader)
		{
			m_pImgEffectBG[nCnt] = new CINFImageEx;		
			m_pImgEffectBG[nCnt]->InitDeviceObjects(pDataHeader);
		}	
	}	

	int i;
	for(i=0; i< 4; i++)
	{
		char temp[256];
		m_pImgEffBG[2][i] = new CINFImageEx; 
		wsprintf(temp,"AME_BG_%d",i+1);		
		pDataHeader = FindResource(temp);
		m_pImgEffBG[2][i]->InitDeviceObjects(pDataHeader);

		m_pImgEffBG[1][i] = new CINFImageEx; 
		wsprintf(temp,"WME_BG_%d",i+1);		
		pDataHeader = FindResource(temp);
		m_pImgEffBG[1][i]->InitDeviceObjects(pDataHeader);

		m_pImgEffBG[0][i] = new CINFImageEx; 
		wsprintf(temp,"HME_BG_%d",i+1);		
		pDataHeader = FindResource(temp);
		m_pImgEffBG[0][i]->InitDeviceObjects(pDataHeader);
	}
	
	for(i=0; i< 2; i++)
	{
		char temp[256];
		m_pImgEffBG[2][i+4] = new CINFImageEx; 
		wsprintf(temp,"AME_BG_%d",i+3);		
		pDataHeader = FindResource(temp);
		m_pImgEffBG[2][i+4]->InitDeviceObjects(pDataHeader);

		m_pImgEffBG[1][i+4] = new CINFImageEx; 
		wsprintf(temp,"WME_BG_%d",i+3);		
		pDataHeader = FindResource(temp);
		m_pImgEffBG[1][i+4]->InitDeviceObjects(pDataHeader);

		m_pImgEffBG[0][i+4] = new CINFImageEx; 
		wsprintf(temp,"HME_BG_%d",i+3);		
		pDataHeader = FindResource(temp);
		m_pImgEffBG[0][i+4]->InitDeviceObjects(pDataHeader);
	}
	
	char temp1[256];
	m_pImgEffBG[2][6] = new CINFImageEx; 
	wsprintf(temp1,"AME_BG_%d",5);
	pDataHeader = FindResource(temp1);
	m_pImgEffBG[2][6]->InitDeviceObjects(pDataHeader);

	m_pImgEffBG[1][6] = new CINFImageEx; 
	wsprintf(temp1,"WME_BG_%d",5);
	pDataHeader = FindResource(temp1);
	m_pImgEffBG[1][6]->InitDeviceObjects(pDataHeader);

	m_pImgEffBG[0][6] = new CINFImageEx; 
	wsprintf(temp1,"HME_BG_%d",5);
	pDataHeader = FindResource(temp1);
	m_pImgEffBG[0][6]->InitDeviceObjects(pDataHeader);

	for(nCnt = 0;nCnt < MAX_LUCKY_YOULUCKY;nCnt++)
	{
		char chTmp[32];
		wsprintf(chTmp, "LMLBG%d", nCnt);
		pDataHeader = FindResource(chTmp);
		if(pDataHeader)
		{
			m_pImgYouLuckyBG[nCnt] = new CINFImageEx;		
			m_pImgYouLuckyBG[nCnt]->InitDeviceObjects(pDataHeader);
		}		
	}
	

//	m_pImgArrow = new CINFImage;
//	pDataHeader = FindResource("arrow");
//	m_pImgArrow->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize);
	
	m_pImgQuestionMark = new CINFImageEx;
	pDataHeader = FindResource("question");
	m_pImgQuestionMark->InitDeviceObjects(pDataHeader);
	

	if(m_pBtnGamble == NULL)
	{
		m_pBtnGamble = new CINFImageBtn;
	}
	//m_pBtnGamble->InitDeviceObjects("AM_Start");
	m_pBtnGamble->InitDeviceObjects("GameStart");
	
	if(m_pBtnTake == NULL)
	{
		m_pBtnTake = new CINFImageBtn;
	}
	//m_pBtnTake->InitDeviceObjects("AM_Prize");
	m_pBtnTake->InitDeviceObjects("PrizeItem");
	
	// 2009-03-04 by bhsohn ��Ű �ӽ� ������	
	
	// 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
	if(m_pBtnGamble1 == NULL)
	{
		m_pBtnGamble1 = new CINFImageBtn;
	}
	m_pBtnGamble1->InitDeviceObjects("GameStart");
	if(m_pBtnTake1 == NULL)
	{
		m_pBtnTake1 = new CINFImageBtn;
	}
	m_pBtnTake1->InitDeviceObjects("PrizeItem");
	//end 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
	// ��Ű�ӽ� ĳ����
	{
		if(NULL == m_pImgSelctShopBK)
		{
			m_pImgSelctShopBK = new CINFImageEx;
			pDataHeader = FindResource("lbk1");
			m_pImgSelctShopBK->InitDeviceObjects(pDataHeader);
		}
		if(NULL == m_pSelectShopPrevBtn)
		{
			m_pSelectShopPrevBtn = new CINFImageBtn;			
			m_pSelectShopPrevBtn->InitDeviceObjects("lspre");
		}
		if(NULL == m_pSelectShopNextBtn)
		{
			m_pSelectShopNextBtn = new CINFImageBtn;			
			m_pSelectShopNextBtn->InitDeviceObjects("lsnex");
		}		 
	}
	if(NULL == m_pPrevStepBtn)
	{
		m_pPrevStepBtn = new CINFImageBtn;			
		m_pPrevStepBtn->InitDeviceObjects("lpre");

	}
	InitDeviceObjectsNPCListBtn();
	// end 2009-03-04 by bhsohn ��Ű �ӽ� ������
	// 2010. 05. 12 by jskim �ű� ��Ű �ӽ� �߰���� - ��Ű�ӽſ��� �޴� �����۸� �����ֱ�
	if(NULL == m_pImgDisableItem)
	{
		m_pImgDisableItem = new CINFImageEx;
		pDataHeader = FindResource("LM_inven");
		m_pImgDisableItem->InitDeviceObjects(pDataHeader);
	}
	//end 2010. 05. 12 by jskim �ű� ��Ű �ӽ� �߰���� - ��Ű�ӽſ��� �޴� �����۸� �����ֱ�
	// 2010. 05. 13 by jskim �ű� ��Ű �ӽ� �߰���� - �޴� ������ ����Ʈ ����
	if(NULL == m_pAME_slot[0])
	{
		m_pAME_slot[0] = new CINFImageEx;
		pDataHeader = FindResource("AME_slot0");
		m_pAME_slot[0]->InitDeviceObjects(pDataHeader );
	}
	if(NULL == m_pAME_slot[1])
	{
		m_pAME_slot[1] = new CINFImageEx;
		pDataHeader = FindResource("AME_slot1");
		m_pAME_slot[1]->InitDeviceObjects(pDataHeader);
	}

	for(nCnt=0; nCnt < 3; nCnt++)
	{
		m_nAME_EffectNum[nCnt] = NULL;
	}	//end 2010. 05. 13 by jskim �ű� ��Ű �ӽ� �߰���� - �޴� ������ ����Ʈ ����
	return S_OK;
}
HRESULT CINFLuckyMachine::RestoreDeviceObjects()
{
//	m_pImgArrow->RestoreDeviceObjects();
	for(int i=0; i< 7; i++)
	{
		for(int j=0; j < 3; j++)
		{
			m_pImgEffBG[j][i]->RestoreDeviceObjects();
		}
	}

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_LUCKY_TITLE_IMAGE;nCnt++)
	{
		if(m_pImgEffectBG[nCnt])
		{
			m_pImgEffectBG[nCnt]->RestoreDeviceObjects();		
		}
	}
	for(nCnt = 0;nCnt < MAX_LUCKY_YOULUCKY;nCnt++)
	{	
		if(m_pImgYouLuckyBG[nCnt])
		{			
			m_pImgYouLuckyBG[nCnt]->RestoreDeviceObjects();
		}		
	}
	for(nCnt = 0;nCnt < LUCKY_SELECT_MAX;nCnt++)
	{	
		m_pImgMainBG[nCnt]->RestoreDeviceObjects();
	}
	
	m_pImgQuestionMark->RestoreDeviceObjects();

	m_pBtnGamble->RestoreDeviceObjects();
	m_pBtnTake->RestoreDeviceObjects();
	m_pBtnGamble->SetBtnPosition(LUCKYMACHINE_GAMBLE_BUTTON_X,LUCKYMACHINE_GAMBLE_BUTTON_Y);
	m_pBtnTake->SetBtnPosition(LUCKYMACHINE_TAKE_BUTTON_X,LUCKYMACHINE_TAKE_BUTTON_Y);

	// 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
	m_pBtnGamble1->RestoreDeviceObjects();
	m_pBtnTake1->RestoreDeviceObjects();
	m_pBtnGamble1->SetBtnPosition(LUCKYMACHINE_GAMBLE_BUTTON_X,LUCKYMACHINE_GAMBLE_BUTTON_Y);
	m_pBtnTake1->SetBtnPosition(LUCKYMACHINE_TAKE_BUTTON_X,LUCKYMACHINE_TAKE_BUTTON_Y);
	//end 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����

	// 2009-03-04 by bhsohn ��Ű �ӽ� ������	
	{
		m_pImgSelctShopBK->RestoreDeviceObjects();
		m_pSelectShopPrevBtn->RestoreDeviceObjects();		
		m_pSelectShopPrevBtn->SetBtnPosition(LUCKYMACHINE_SELECT_SHOP_PREV_X, LUCKYMACHINE_SELECT_SHOP_PREV_Y);
		
		m_pSelectShopNextBtn->RestoreDeviceObjects();		
		m_pSelectShopNextBtn->SetBtnPosition(LUCKYMACHINE_SELECT_SHOP_NEXT_X,LUCKYMACHINE_SELECT_SHOP_NEXT_Y);
		
	}
	{
		m_pPrevStepBtn->RestoreDeviceObjects();
		m_pPrevStepBtn->SetBtnPosition(LUCKYMACHINE_PREV_X, LUCKYMACHINE_PREV_Y);
	}
	RestoreDeviceObjectsNPCListBtn();


	RefreshNPCListBtnStat();
	// end 2009-03-04 by bhsohn ��Ű �ӽ� ������
	// 2010. 05. 12 by jskim �ű� ��Ű �ӽ� �߰���� - ��Ű�ӽſ��� �޴� �����۸� �����ֱ�
	m_pImgDisableItem->RestoreDeviceObjects();
	//end 2010. 05. 12 by jskim �ű� ��Ű �ӽ� �߰���� - ��Ű�ӽſ��� �޴� �����۸� �����ֱ�

	// 2010. 05. 13 by jskim �ű� ��Ű �ӽ� �߰���� - �޴� ������ ����Ʈ ����
	m_pAME_slot[0]->RestoreDeviceObjects();
	m_pAME_slot[1]->RestoreDeviceObjects();
	//end 2010. 05. 13 by jskim �ű� ��Ű �ӽ� �߰���� - �޴� ������ ����Ʈ ����
	return S_OK;
}

HRESULT CINFLuckyMachine::DeleteDeviceObjects()
{
//	m_pImgArrow->DeleteDeviceObjects();

	for(int i=0; i< 7; i++)
	{
		for(int j=0; j < 3; j++)
		{
			m_pImgEffBG[j][i]->DeleteDeviceObjects();
			util::del(m_pImgEffBG[j][i]);
		}
	}

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_LUCKY_TITLE_IMAGE;nCnt++)
	{
		if(m_pImgEffectBG[nCnt])
		{			
			m_pImgEffectBG[nCnt]->DeleteDeviceObjects();
			util::del(m_pImgEffectBG[nCnt]);			
		}
	}
	for(nCnt = 0;nCnt < MAX_LUCKY_YOULUCKY;nCnt++)
	{	
		if(m_pImgYouLuckyBG[nCnt])
		{			
			m_pImgYouLuckyBG[nCnt]->DeleteDeviceObjects();
			util::del(m_pImgYouLuckyBG[nCnt]);			
		}		
	}
	
	for(nCnt = 0;nCnt < LUCKY_SELECT_MAX;nCnt++)
	{
		m_pImgMainBG[nCnt]->DeleteDeviceObjects();
		util::del(m_pImgMainBG[nCnt]);
	}	

	m_pImgQuestionMark->DeleteDeviceObjects();
	util::del(m_pImgQuestionMark);

	m_pBtnGamble->DeleteDeviceObjects();
	util::del(m_pBtnGamble);

	m_pBtnTake->DeleteDeviceObjects();
	util::del(m_pBtnTake);

	// 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
	m_pBtnGamble1->DeleteDeviceObjects();
	util::del(m_pBtnGamble1);
	
	m_pBtnTake1->DeleteDeviceObjects();
	util::del(m_pBtnTake1);
	//end 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
	// 2009-03-04 by bhsohn ��Ű �ӽ� ������	
	{
		m_pImgSelctShopBK->DeleteDeviceObjects();
		util::del(m_pImgSelctShopBK);

		m_pSelectShopPrevBtn->DeleteDeviceObjects();		
		util::del(m_pSelectShopPrevBtn);

		m_pSelectShopNextBtn->DeleteDeviceObjects();		
		util::del(m_pSelectShopNextBtn);	
	}

	{
		m_pPrevStepBtn->DeleteDeviceObjects();
		util::del(m_pPrevStepBtn);
	}
	DeleteDeviceObjectsNPCListBtn();
	// end 2009-03-04 by bhsohn ��Ű �ӽ� ������

	// 2010. 05. 12 by jskim �ű� ��Ű �ӽ� �߰���� - ��Ű�ӽſ��� �޴� �����۸� �����ֱ�
	m_pImgDisableItem->DeleteDeviceObjects();
	util::del(m_pImgDisableItem);
	//end 2010. 05. 12 by jskim �ű� ��Ű �ӽ� �߰���� - ��Ű�ӽſ��� �޴� �����۸� �����ֱ�


	// 2010. 05. 13 by jskim �ű� ��Ű �ӽ� �߰���� - �޴� ������ ����Ʈ ����
	m_pAME_slot[0]->DeleteDeviceObjects();
	util::del(m_pAME_slot[0]);
	m_pAME_slot[1]->DeleteDeviceObjects();
	util::del(m_pAME_slot[1]);
	//end 2010. 05. 13 by jskim �ű� ��Ű �ӽ� �߰���� - �޴� ������ ����Ʈ ����

	return S_OK;
}

HRESULT CINFLuckyMachine::InvalidateDeviceObjects()
{
//	m_pImgArrow->InvalidateDeviceObjects();

	for(int i=0; i< 7; i++)
	{
		for(int j=0; j < 3; j++)
		{
			m_pImgEffBG[j][i]->InvalidateDeviceObjects();
		}
	}

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_LUCKY_TITLE_IMAGE;nCnt++)
	{
		if(m_pImgEffectBG[nCnt])
		{			
			m_pImgEffectBG[nCnt]->InvalidateDeviceObjects();
		}
	}
	for(nCnt = 0;nCnt < MAX_LUCKY_YOULUCKY;nCnt++)
	{	
		if(m_pImgYouLuckyBG[nCnt])
		{			
			m_pImgYouLuckyBG[nCnt]->InvalidateDeviceObjects();			
		}		
	}

	for(nCnt = 0;nCnt < LUCKY_SELECT_MAX;nCnt++)
	{	
		m_pImgMainBG[nCnt]->InvalidateDeviceObjects();
	}
	m_pImgQuestionMark->InvalidateDeviceObjects();

	m_pBtnGamble->InvalidateDeviceObjects();
	m_pBtnTake->InvalidateDeviceObjects();

	// 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
	m_pBtnGamble1->InvalidateDeviceObjects();
	m_pBtnTake1->InvalidateDeviceObjects();
	//end 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
	
	// 2009-03-04 by bhsohn ��Ű �ӽ� ������	
	{
		m_pImgSelctShopBK->InvalidateDeviceObjects();			

		m_pSelectShopPrevBtn->InvalidateDeviceObjects();		
		m_pSelectShopNextBtn->InvalidateDeviceObjects();
	}
	{
		m_pPrevStepBtn->InvalidateDeviceObjects();
	}
	InvalidateDeviceObjectsNPCListBtn();
	// end 2009-03-04 by bhsohn ��Ű �ӽ� ������

	// 2010. 05. 12 by jskim �ű� ��Ű �ӽ� �߰���� - ��Ű�ӽſ��� �޴� �����۸� �����ֱ�
	m_pImgDisableItem->InvalidateDeviceObjects();
	//end 2010. 05. 12 by jskim �ű� ��Ű �ӽ� �߰���� - ��Ű�ӽſ��� �޴� �����۸� �����ֱ�

	// 2010. 05. 13 by jskim �ű� ��Ű �ӽ� �߰���� - �޴� ������ ����Ʈ ����
	m_pAME_slot[0]->DeleteDeviceObjects();
	m_pAME_slot[1]->DeleteDeviceObjects();
	//end 2010. 05. 13 by jskim �ű� ��Ű �ӽ� �߰���� - �޴� ������ ����Ʈ ����
	return S_OK;
}

void CINFLuckyMachine::Render()
{
	switch(m_nLuckyMechineStep)
	{ 	
	case LUCKY_MAINSTEP_SELECT_SHOP:		// ��Ű �ӽ� ���� ����
		{
			RenderLuckyStepStepSelectShop();
		}
		break;
	case LUCKY_MAINSTEP_SELECT_GAMING:		// ��Ű �ӽ� ������
		{
			RenderLuckyStepStepSelectGaming();
		}
		break;
	}
	
	// �ڷ� ���� 
	m_pPrevStepBtn->Render();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::RenderLuckyStepStepSelectShop()
{
	m_pImgSelctShopBK->Move(LUCKYMACHINE_SELECT_SHOP_BG_X, LUCKYMACHINE_SELECT_SHOP_BG_Y);
	m_pImgSelctShopBK->Render();

	// ��Ű �ӽ� Ÿ�� �븻
	m_pSelectShopPrevBtn->Render();		
	
	// ��Ű �ӽ� Ÿ�� �̴�
	m_pSelectShopNextBtn->Render();			

	// NPC��ư	
	{
		vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
		while(itNpcBtn != m_vecstruShopList.end())
		{
			structNPCList* pstruNPCList = (*itNpcBtn);
			
			if(pstruNPCList->pInfImageBtn)
			{
				pstruNPCList->pInfImageBtn->Render();
			}		
			itNpcBtn++;
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::RenderLuckyStepStepSelectGaming()
{	
	// ��� �̹��� 
	int nImageIdx=0, nImageType=0;
	nImageType = m_struSelLuckyMachine.MachineKind - 1;
		nImageIdx = m_nEffectBG;
//	m_pImgEffBG[nImageType][nImageIdx]->Move(LUCKYMACHINE_EFFECT_BG_X,LUCKYMACHINE_EFFECT_BG_Y);
//	m_pImgEffBG[nImageType][nImageIdx]->Render();

	m_pImgMainBG[nImageType]->Move(LUCKYMACHINE_MAIN_BG_X,LUCKYMACHINE_MAIN_BG_Y);
	m_pImgMainBG[nImageType]->Render();

	int i = 0,j = 0;
	CINFIcon* pIcon = ((CINFGameMain*)m_pParent)->m_pIcon;
	switch(m_nState)
	{
	case LUCKY_STATE_READY:
		{
			// 2009-03-04 by bhsohn ��Ű �ӽ� ������
			for(j = 0 ; j < CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind) ; j++)
			{
				int nSlotPosIdx = j;
				if(LUCKY_SELECT_TYPE_MINI == CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind))//m_struSelLuckyMachine.MachineKind)
				{
					nSlotPosIdx = LUCKY_SELECT_TYPE_MINI;
				}
				for(i = 0 ; i < LUCKY_ITEM_LIST_NUM ; i++)
				{
					m_pImgQuestionMark->Move(LUCKYMACHINE_SLOT_X+(LUCKYMACHINE_SLOT_GAP_X*i),
						LUCKYMACHINE_SLOT_Y+(LUCKYMACHINE_SLOT_GAP_Y*nSlotPosIdx));
					m_pImgQuestionMark->Render();
				}
			}
		}
		break;
	case LUCKY_STATE_PRIZE:
	case LUCKY_STATE_RULLET:
		{					
			int nSlotNum=0;
			for(nSlotNum=0;nSlotNum < CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind); nSlotNum++)
			{
				RenderLuckyMachineSlot(nSlotNum);
			}
		}
		break;
	}

	// 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
// 	m_pBtnGamble->Render();
// 	m_pBtnTake->Render();
	if(m_struSelLuckyMachine.MachineKind == LUCKY_MACHINE_KIND_HAPPY_MACHINE)
	{
		m_pBtnGamble1->Render();
		m_pBtnTake1->Render();
	}
	else
	{
		m_pBtnGamble->Render();
		m_pBtnTake->Render();
	}
	//end 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
	
	if(m_pCoinItemInfo != NULL && 
		m_pCoinItemInfo->ItemInfo->SourceIndex != NULL)
	{// ��ȭ.
		CItemInfo* pItem = m_pCoinItemInfo;
		char buf[20];
		wsprintf(buf, "%08d",pItem->ItemInfo->SourceIndex);
		pIcon->SetIcon( buf, LUCKYMACHINE_COIN_X,LUCKYMACHINE_COIN_Y, 1.0f );
		pIcon->Render();
	}

	// 2010. 05. 12 by jskim ���� �ӽ� ���� ����Ʈ ���̴� �κ��� �κ� �ʿ��� ��Ű�ӽ� ������ ����
	RenderYouLucky();
	//end 2010. 05. 12 by jskim ���� �ӽ� ���� ����Ʈ ���̴� �κ��� �κ� �ʿ��� ��Ű�ӽ� ������ ����
}


void CINFLuckyMachine::Tick()
{
	switch(m_nLuckyMechineStep)
	{ 	
	case LUCKY_MAINSTEP_SELECT_SHOP:		// ��Ű �ӽ� ���� ����
		{
			TickLuckyStepStepSelectShop();
		}
		break;
	case LUCKY_MAINSTEP_SELECT_GAMING:		// ��Ű �ӽ� ������
		{
			TickLuckyStepStepSelectGaming();
		}
		break;
	}

}
void CINFLuckyMachine::TickSlot(int i_nSlot,float i_fElapsedTime)
{
	int nRulType = m_nSlotType[i_nSlot];
	
	if(m_fTimeAllTimeStepSlot[i_nSlot] != -1)
	{
		////////////////////// ��ü ���� �ð�  //////////////////////
		m_fTimeAllTimeStepSlot[i_nSlot] += i_fElapsedTime;
	}

	if(!m_bCompleteStep[i_nSlot])
	{
		////////////////////// �Ѱ� ���� ������ �ð� //////////////////////
		m_fTimeOneStepSlot[i_nSlot] += i_fElapsedTime;
		int nSlotStep = m_nSlotStep[i_nSlot];		
		
		if(m_fTimeOneStepSlot[i_nSlot] > m_stRulTime[i_nSlot][nRulType].stRulTime[nSlotStep].fSlotTime)
		{						
			// �Ѵܰ� �� ���� �ö���
			m_bCompleteStep[i_nSlot] = SlotSpeedStepUp(i_nSlot);						
			UpdateCurrentSlotPos(i_nSlot);	// �̹� ���ܿ� ���� ���۰��� ����
		}
	}

	{
		////////////////////// ��ĭ ������ �ð� //////////////////////		
		int nSlotStep = m_nSlotStep[i_nSlot];		
		if(m_stRulTime[i_nSlot][nRulType].stRulTime[nSlotStep].fHoldTime <= m_fTimeHold[i_nSlot])
		{
			// ������ ���� ��� �̵� �� �̵�.
			m_fTimeHold[i_nSlot] = 0.0f;
			m_nSlotStartPosIdx[i_nSlot] += m_stRulTime[i_nSlot][nRulType].stRulTime[nSlotStep].nSlotStep;
		}
		else
		{// �ð� ���.
			m_fTimeHold[i_nSlot] += i_fElapsedTime;
		}
		// ī��Ʈ�� ���� �ȳ��� ��
		if(m_nSlotStartPosIdx[i_nSlot] <0)
		{
			if(m_vecIconNum[i_nSlot].empty())
			{
				m_nSlotStartPosIdx[i_nSlot] = 0;
			}
			else
			{
				m_nSlotStartPosIdx[i_nSlot] = ((int)m_vecIconNum[i_nSlot].size());
			}
		}
		else if(((int)m_vecIconNum[i_nSlot].size()) < m_nSlotStartPosIdx[i_nSlot])
		{
			m_nSlotStartPosIdx[i_nSlot] = 0;
		}		
		
	}
	
	if(m_bCompleteStep[i_nSlot] && !m_bSlotDone[i_nSlot])
	{
		// ������ ������ ���鼭 �� ������ �Ϸ� �ƴ��� �Ǵ�
		// �� ������ ������ �Ϸ� ��?
		m_bSlotDone[i_nSlot] = IsSlotDone(i_nSlot);	
	}	

	if(m_fTimeAllTimeStepSlot[i_nSlot] > MAX_LUCKY_MACHINE_TIME)
	{
		// �� �ð� �ʰ� �ϸ� ������ ����
		m_fTimeAllTimeStepSlot[i_nSlot] = -1.0f;
		
		int nSlotStep = m_nSlotStep[i_nSlot];		

		int nCap = abs(m_nSuccessSlot[i_nSlot] - m_nSlotStartPosIdx[i_nSlot]);
		if(nCap > OVERTIME_LUCKY_MACHINE_STEP)
		{
			if(m_stRulTime[i_nSlot][nRulType].stRulTime[nSlotStep].nSlotStep > 0)
			{
				m_nSlotStartPosIdx[i_nSlot] = m_nSuccessSlot[i_nSlot] - OVERTIME_LUCKY_MACHINE_STEP;
			}
			else
			{
				m_nSlotStartPosIdx[i_nSlot] = m_nSuccessSlot[i_nSlot] + OVERTIME_LUCKY_MACHINE_STEP;
			}
		}
		
		// ī��Ʈ�� ���� �ȳ��� ��
		if(m_nSlotStartPosIdx[i_nSlot] <0)
		{
			if(m_vecIconNum[i_nSlot].empty())
			{
				m_nSlotStartPosIdx[i_nSlot] = 0;
			}
			else
			{
				m_nSlotStartPosIdx[i_nSlot] = ((int)m_vecIconNum[i_nSlot].size());
			}
		}
		else if(((int)m_vecIconNum[i_nSlot].size()) < m_nSlotStartPosIdx[i_nSlot])
		{
			m_nSlotStartPosIdx[i_nSlot] = 0;
		}

		
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFLuckyMachine::IsSlotDone(int i_nSlot)
{
	if(m_vecIconNum[i_nSlot].empty())
	{
		// �������� ������ �Ϸ� �Ȳ���~
		return TRUE;
	}
	

	int nTakeGetItemNum = m_vecTakeItem[i_nSlot].Itemnum;
	int nCheckItemPos = m_nSlotStartPosIdx[i_nSlot];

// 	 // �Ϸ� ��ġ üũ 

	int nCompleteItemIdx = LUCKY_ITEM_POS;
	if(nCompleteItemIdx > (int)m_vecIconNum[i_nSlot].size())
	{
		nCompleteItemIdx = (int)m_vecIconNum[i_nSlot].size();
	}

	nCheckItemPos += nCompleteItemIdx;
	if(nCheckItemPos >= (int)m_vecIconNum[i_nSlot].size())
	{
		nCheckItemPos -= (int)m_vecIconNum[i_nSlot].size();
	}	
	if(nCheckItemPos < 0)
	{
		nCheckItemPos = 0;
	}
	

	ST_ICON_ITEM_NUMBER stIconNum = m_vecIconNum[i_nSlot][nCheckItemPos];
	if(stIconNum.nItemNum == nTakeGetItemNum)
	{
		// 2010. 05. 13 by jskim �ű� ��Ű �ӽ� �߰���� - �޴� ������ ����Ʈ ����
		m_nAME_EffectNum[i_nSlot] = 0.001f;
		//end 2010. 05. 13 by jskim �ű� ��Ű �ӽ� �߰���� - �޴� ������ ����Ʈ ����
		return TRUE;
	}
	
	return FALSE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::TickLuckyStepStepSelectShop()
{
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::TickLuckyStepStepSelectGaming()
{
	FLOAT fElapsedTime = g_pD3dApp->GetElapsedTime();
	int i;
	

	// 2012-02-02 by jhahn ��Ű �ӽ� UV �ִϸ��̼� ó��
	for (int nCount  = 0 ; nCount < LUCKY_SELECT_MAX; nCount++)
	{
		m_pImgMainBG[nCount]->Tick();
	}
	//end 2012-02-02 by jhahn ��Ű �ӽ� UV �ִϸ��̼� ó��
	
	

	if(m_fLuckyTimeEffectBG > LUCKYMACHINE_EFFECT_FRAME_CHANGE_TIME)
	{
		m_nLuckyEffectBG = !m_nLuckyEffectBG;
		m_fLuckyTimeEffectBG = 0;
	}
	else
	{
		m_fLuckyTimeEffectBG += fElapsedTime;
	}

		if(m_fTimeEffectBG > LUCKYMACHINE_EFFECT_FRAME_CHANGE_TIME)
		{
		m_nEffectBG += 1;	
			m_fTimeEffectBG = 0.0f;
		}
		else
		{
			m_fTimeEffectBG += fElapsedTime;
		}
	if(m_nEffectBG > 6)
		m_nEffectBG = 0;

	switch(m_nState)
	{
	case LUCKY_STATE_READY:
		{
		}
		break;
	case LUCKY_STATE_RULLET:
		{			
			//for(i = 0; i < LUCKY_ITEM_LIST_NUM_LINE ;i++)
			for(i = 0 ; i < CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind); i++)
			{
				if(m_bSlotDone[i])
				{					
					continue;
				}
				TickSlot(i,fElapsedTime);				
			}
			
			if(IsAllRuletDone())
			{
				// ��� ������ �Ϸ�� ����
				ChangeState(LUCKY_STATE_PRIZE);				
			}
			
		}
		break;
	case LUCKY_STATE_PRIZE:
		{
			
		}
		break;
	}
	for(i=0; i< LUCKY_ITEM_LIST_NUM_LINE; i++)
	{
		if(m_nAME_EffectNum[i] > 0)
			m_nAME_EffectNum[i] += g_pD3dApp->GetElapsedTime();

		if(m_nAME_EffectNum[i] > 1.0f)
		{
			m_nAME_EffectNum[i]=0.001f;
		}
	}
	
}
BOOL CINFLuckyMachine::SlotSpeedStepUp(int i_nSlot)
{	
	int nRulType = m_nSlotType[i_nSlot];
	
	int nOldSlotStep = 	m_nSlotStep[i_nSlot];
	int nSlotStep = m_nSlotStep[i_nSlot]+1;
	if(nSlotStep >= MAX_SLOT_RULLET_STEP)
	{
		m_nSlotStep[i_nSlot] = (MAX_SLOT_RULLET_STEP-1);
		return TRUE;
	}	
	
	BOOL bCompleteStep = m_stRulTime[i_nSlot][nRulType].stRulTime[nOldSlotStep].bCompleteStep;

	m_nSlotStep[i_nSlot] = nSlotStep;
	m_fTimeOneStepSlot[i_nSlot] = 0.0f;		

	//if(-1 == m_stRulTime[i_nSlot][nRulType].stRulTime[nSlotStep].fSlotTime)
	if(bCompleteStep || (-1 == m_stRulTime[i_nSlot][nRulType].stRulTime[nSlotStep].fSlotTime))
	{
		m_nSlotStep[i_nSlot] = nOldSlotStep;
		// ���ܿϷ�
		return TRUE;
	}	
	
	return FALSE;

}


int CINFLuckyMachine::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(INF_MSGPROC_BREAK == WndProcCommon(uMsg, wParam, lParam))
	{
		return INF_MSGPROC_BREAK;
	}

	switch(m_nLuckyMechineStep)
	{ 	
	case LUCKY_MAINSTEP_SELECT_SHOP:		// ��Ű �ӽ� ���� ����
		{
			return WndProcLuckyStepStepSelectShop(uMsg, wParam, lParam);
		}
		break;
	case LUCKY_MAINSTEP_SELECT_GAMING:		// ��Ű �ӽ� ������
		{
			return WndProcLuckyStepStepSelectGaming(uMsg, wParam, lParam);
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFLuckyMachine::WndProcCommon(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			m_pPrevStepBtn->OnMouseMove(pt);			
			
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(m_pPrevStepBtn->OnLButtonDown(pt))
			{
				return INF_MSGPROC_BREAK;
			}			
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(m_pPrevStepBtn->OnLButtonUp(pt))
			{				
				OnClickPrevStepBtn();
				// ��ư Ŭ�� 			
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
				
				return INF_MSGPROC_BREAK;
			}			
			
		}
		break;
		
	}
	return INF_MSGPROC_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int	CINFLuckyMachine::WndProcLuckyStepStepSelectShop(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			m_pSelectShopPrevBtn->OnMouseMove(pt);
			m_pSelectShopNextBtn->OnMouseMove(pt);			
			
			BOOL bSelectBtn = FALSE;
			char chTmp[MAX_PATH];
			ZERO_MEMORY(chTmp);

			
			{
				vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
				while(itNpcBtn != m_vecstruShopList.end())
				{
					structNPCList* pstruNPCList = (*itNpcBtn);
					
					if(pstruNPCList->pInfImageBtn && (pstruNPCList->pInfImageBtn->OnMouseMove(pt)))
					{
						bSelectBtn = TRUE;					
						strncpy(chTmp, pstruNPCList->struLuckyMachineOmi.szDescription, SIZE_MAX_LUCKYMACHINE_DESCRIPTION);
					}		
					itNpcBtn++;
				}
			}
			
			g_pGameMain->SetToolTip(pt.x,pt.y,chTmp);
			if(bSelectBtn)
			{				
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
			if(m_pSelectShopPrevBtn->OnLButtonDown(pt))
			{
				return INF_MSGPROC_BREAK;
			}
			if(m_pSelectShopNextBtn->OnLButtonDown(pt))
			{
				return INF_MSGPROC_BREAK;
			}
			
			{
				vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
				while(itNpcBtn != m_vecstruShopList.end())
				{
					structNPCList* pstruNPCList = (*itNpcBtn);
					
					if(pstruNPCList->pInfImageBtn && pstruNPCList->pInfImageBtn->OnLButtonDown(pt))
					{
						return INF_MSGPROC_BREAK;
					}		
					itNpcBtn++;
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
			if(m_pSelectShopPrevBtn->OnLButtonUp(pt))
			{						
				OnClickNPCListPos(-1);
				// ��ư Ŭ�� 			
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
				
				return INF_MSGPROC_BREAK;
			}
			if(m_pSelectShopNextBtn->OnLButtonUp(pt))
			{						
				OnClickNPCListPos(1);
				// ��ư Ŭ�� 			
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);						
				return INF_MSGPROC_BREAK;
			}		
			
			{
				vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
				while(itNpcBtn != m_vecstruShopList.end())
				{
					structNPCList* pstruNPCList = (*itNpcBtn);					
					
					if(pstruNPCList->pInfImageBtn && (pstruNPCList->pInfImageBtn->OnLButtonUp(pt)))
					{
						if(NOT_OPEN_LUCKY_MACHINE != pstruNPCList->struLuckyMachineOmi.SourceIndex)
						{
							OnClickSelectNPCShop(&pstruNPCList->struLuckyMachineOmi);
							
							g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
						}
						return INF_MSGPROC_BREAK;
					}		
					itNpcBtn++;
				}
			}
			
		}
		break;
		
	}
	return INF_MSGPROC_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int	CINFLuckyMachine::WndProcLuckyStepStepSelectGaming(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_MOUSEMOVE:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

					// 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
					if(m_pBtnGamble->OnMouseMove(pt) && m_struSelLuckyMachine.MachineKind != LUCKY_MACHINE_KIND_HAPPY_MACHINE) 
					{
						g_pGameMain->SetToolTip(pt.x,pt.y,STRMSG_C_081201_0100);
						return INF_MSGPROC_BREAK;
					}										
					else if(m_pBtnGamble1->OnMouseMove(pt) && m_struSelLuckyMachine.MachineKind == LUCKY_MACHINE_KIND_HAPPY_MACHINE)
					{
						g_pGameMain->SetToolTip(pt.x,pt.y,STRMSG_C_081201_0100);
						return INF_MSGPROC_BREAK;
					}
					if(m_pBtnTake->OnMouseMove(pt) && m_struSelLuckyMachine.MachineKind != LUCKY_MACHINE_KIND_HAPPY_MACHINE)
					{
					}
					else if(m_pBtnTake1->OnMouseMove(pt) && m_struSelLuckyMachine.MachineKind == LUCKY_MACHINE_KIND_HAPPY_MACHINE)
					{
					}
					//end 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
				
				if(pt.x > LUCKYMACHINE_COIN_X &&
					pt.x < LUCKYMACHINE_COIN_X + ICON_SIZE_W &&
					pt.y > LUCKYMACHINE_COIN_Y &&
					pt.y < LUCKYMACHINE_COIN_Y + ICON_SIZE_H)
				{
					g_pGameMain->SetToolTip(pt.x,pt.y,STRMSG_C_081201_0102);
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
				if(m_pBtnGamble->OnLButtonDown(pt) && m_struSelLuckyMachine.MachineKind != LUCKY_MACHINE_KIND_HAPPY_MACHINE)
				{
					return INF_MSGPROC_BREAK;
				}
				// 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
				else if(m_pBtnGamble1->OnLButtonDown(pt) && m_struSelLuckyMachine.MachineKind == LUCKY_MACHINE_KIND_HAPPY_MACHINE)
				{
					return INF_MSGPROC_BREAK;
				}
				//end 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
				if(m_pBtnTake->OnLButtonDown(pt) && m_struSelLuckyMachine.MachineKind != LUCKY_MACHINE_KIND_HAPPY_MACHINE)
				{
					return INF_MSGPROC_BREAK;
				}
				// 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
				else if(m_pBtnTake1->OnLButtonDown(pt) && m_struSelLuckyMachine.MachineKind == LUCKY_MACHINE_KIND_HAPPY_MACHINE)
				{
					return INF_MSGPROC_BREAK;
				}
				//end 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
			}
			break;
		case WM_LBUTTONUP:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				if(m_pBtnGamble->OnLButtonUp(pt) && m_struSelLuckyMachine.MachineKind != LUCKY_MACHINE_KIND_HAPPY_MACHINE)
				{
					GambleButtonOK();
					return INF_MSGPROC_BREAK;
				}
				// 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
				else if(m_pBtnGamble1->OnLButtonUp(pt) && m_struSelLuckyMachine.MachineKind == LUCKY_MACHINE_KIND_HAPPY_MACHINE)
				{
					GambleButtonOK();
					return INF_MSGPROC_BREAK;
				}
				//end 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
				if(m_pBtnTake->OnLButtonUp(pt) && m_struSelLuckyMachine.MachineKind != LUCKY_MACHINE_KIND_HAPPY_MACHINE)
				{
					PrizeItemGiveMe();
					return INF_MSGPROC_BREAK;
				}
				// 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
				else if(m_pBtnTake1->OnLButtonUp(pt) && m_struSelLuckyMachine.MachineKind == LUCKY_MACHINE_KIND_HAPPY_MACHINE)
				{
					PrizeItemGiveMe();
					return INF_MSGPROC_BREAK;
				}
				//end 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����

				if(pt.x > LUCKYMACHINE_COIN_X &&
					pt.x < LUCKYMACHINE_COIN_X + ICON_SIZE_W &&
					pt.y > LUCKYMACHINE_COIN_Y &&
					pt.y < LUCKYMACHINE_COIN_Y + ICON_SIZE_H &&
					((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
					((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_INVEN_POS)
				{
					if(m_nState == LUCKY_STATE_RULLET)
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081201_0101,COLOR_ERROR);
						break;
					}
					
					if(m_bPrizeItem)
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081202_0102, COLOR_ERROR);
						break;
					}
					// 2009-04-14 by bhsohn �κ� 3�� �̸��Ͻ�, ��Ű�ӽ� �� ���� ����
					if(CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind) > g_pGameMain->m_pInven->GetInvenFreeSize())
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081124_0101,COLOR_ERROR);
						break;
					}
					// end 2009-04-14 by bhsohn �κ� 3�� �̸��Ͻ�, ��Ű�ӽ� �� ���� ����
					if(m_pCoinItemInfo == NULL)
					{
						if(g_pGameMain->m_stSelectItem.pSelectItem &&
							g_pGameMain->m_stSelectItem.pSelectItem->pItem)
						{
							//CItemInfo* pItemInfo = (CItemInfo*)((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem;
							CItemInfo* pItemInfo = (CItemInfo*)g_pGameMain->m_stSelectItem.pSelectItem->pItem;
							// 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
							if(pItemInfo && pItemInfo->ItemInfo
								//&& COMPARE_BIT_FLAG(pItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_LUCKY_ITEM)
								&& (IsPossibleUpLoadCoin(pItemInfo->ItemInfo)))	
							{
								// LuckyMachine���� ���̴� �������̸� 								
// 								util::del(m_pCoinItemInfo);
// 								m_pCoinItemInfo = new CItemInfo((ITEM_GENERAL*)pItemInfo);
// 								m_pCoinItemInfo->CurrentCount = 1;								 
// 								
// 								g_pStoreData->UpdateItemCount(pItemInfo->UniqueNumber, pItemInfo->CurrentCount - 1);
// 								g_pD3dApp->m_pSound->PlayD3DSound(SOUND_LUCKY_COIN, D3DXVECTOR3(0,0,0), FALSE);	

								// ������ �ø���
								UpLoadCoin((ITEM_GENERAL*)pItemInfo);								
							}
							else
							{
								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081124_0102,COLOR_ERROR);//"�ش� �������� ��� �� �� ���� ������ �Դϴ�."
							}
						}
						else
						{
							DBGOUT("m_stSelectItem Error\n");
						}					
					}
				}
				//end 2010. 05. 04 by jskim �Ƹ� �ӽ� UI ����
			}
			break;

	}
	return INF_MSGPROC_NORMAL;
}

INT CINFLuckyMachine::FindSearchItem(INT i_nSlotNum, INT i_nMysteryNum)
{
	i_nSlotNum = i_nSlotNum - 1;
	// 2009-03-04 by bhsohn ��Ű �ӽ� ������
	if(i_nSlotNum < 0 || i_nSlotNum >= LUCKY_ITEM_LIST_NUM_LINE)
	{
		return 0;
	}
	// end 2009-03-04 by bhsohn ��Ű �ӽ� ������
	m_vecIconNum[i_nSlotNum].clear();

	// �Է¹��� ������ ��ȣ�� ������ �ִ� �������� vector�� ������ش�.
	vector<MYSTERY_ITEM_DROP*> vecMystery;
	g_pDatabase->FindMysteryInfo(i_nMysteryNum,&vecMystery);

	DBGOUT("Lucky Machine %d Slot List\n",i_nSlotNum);
	

	vector<MYSTERY_ITEM_DROP*>::iterator it = vecMystery.begin();
	ST_ICON_ITEM_NUMBER stNum;
	while(it != vecMystery.end())
	{
		ITEM * pItem = g_pDatabase->GetServerItemInfo((*it)->DropItemNum);
		// 2009-05-08 by bhsohn ��Ű �ӽ� ������ ���� ������, Ŭ���̾�Ʈ ���� ����
		if(NULL == pItem)
		{
			char szTemp[256] = {0, };
				sprintf ( szTemp , "%s\n[%s]" , STRMSG_C_060608_0000 , "LuckMachine:Can't Found ItemInfo." );

			g_pD3dApp->NetworkErrorMsgBox ( szTemp );
			//g_pD3dApp->NetworkErrorMsgBox(STRMSG_C_060608_0000);	// "������ ������ ����Ǿ����ϴ�. ������ ����˴ϴ�."
			break;
		}
		// end 2009-05-08 by bhsohn ��Ű �ӽ� ������ ���� ������, Ŭ���̾�Ʈ ���� ����
		
		stNum.nIconNum = pItem->SourceIndex;
		stNum.nItemNum = pItem->ItemNum;
		
		m_vecIconNum[i_nSlotNum].push_back(stNum);
		DBGOUT("Lucky Machine Item Num : %d \n",stNum.nItemNum);
		it++;
	}
	stNum.nIconNum = LUCKYMACHINE_BLANK_ICON_NUMBER;
	stNum.nItemNum = LUCKYMACHINE_BLANK_ICON_NUMBER;
	m_vecIconNum[i_nSlotNum].push_back(stNum);
	random_shuffle(m_vecIconNum[i_nSlotNum].begin(),m_vecIconNum[i_nSlotNum].end());
	//
	if(m_vecIconNum[i_nSlotNum].size() < LUCKY_ITEM_LIST_NUM)
	{// Error -> ��ũ �������� ��� 7�� �̻��̾����.
		return -1;
	}
	return 0;
}
void CINFLuckyMachine::ClearSearchItem()
{
	m_vecIconNum[0].clear();
	m_vecIconNum[1].clear();
	m_vecIconNum[2].clear();
	m_vecTakeItem.clear();
	
}
void CINFLuckyMachine::Init()
{
	g_pShuttleChild->ResortingItem();// ����
	if(g_pGameMain && g_pGameMain->m_pInven)
	{
		g_pGameMain->m_pInven->SetScrollEndLine();				// 2006-06-21 by ispark
		g_pGameMain->m_pInven->SetAllIconInfo();
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFLuckyMachine::TakeItem(LUCKY_ITEM_SIMPLEINFO i_sItemInfo)
/// \brief		���� ���� ������.
/// \author		dgwoo
/// \date		2008-11-04 ~ 2008-11-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::TakeItem(LUCKY_ITEM_SIMPLEINFO i_sItemInfo)
{
	if(i_sItemInfo.Itemnum == 0)
		i_sItemInfo.Itemnum = LUCKYMACHINE_BLANK_ICON_NUMBER;
	
	m_vecTakeItem.push_back(i_sItemInfo);
	DBGOUT("***** Lucky Machine Take Item Num : %d \n",i_sItemInfo.Itemnum);
}


void CINFLuckyMachine::SetPrizeItem(ITEM_GENERAL i_Item)
{
	m_vecPrizeItem.push_back(i_Item);
	DBGOUT("** Lucky Machine SetPrizeItem Num : %d \n",i_Item.ItemNum);
	m_bPrizeItem = TRUE;
}


void CINFLuckyMachine::SetPrizeItem(MSG_FC_TRADE_UPDATE_ITEM_COUNT *pMsg)
{
	MSG_FC_TRADE_UPDATE_ITEM_COUNT sMsg;
	sMsg.ItemUniqueNumber		= pMsg->ItemUniqueNumber;
	sMsg.ItemUpdateType			= pMsg->ItemUpdateType;
	sMsg.NewCount				= pMsg->NewCount;
	m_vecPrizeItemCount.push_back(sMsg);
	DBGOUT("** Lucky Machine SetPrizeItem(c) Num : %d \n",pMsg->ItemUniqueNumber);
	m_bPrizeItem = TRUE;
}


/**********************************************************************
**
**	���� ������ ��Ͽ��� ������ ����.
**
**	Create Info :	2010. 08. 02. by hsLee.
**
***********************************************************************/
void CINFLuckyMachine :: SetDeletePrizeItem ( const UID64_t a_ItemUniqueNumber )
{

	// Insert ��� üũ.
	vector<ITEM_GENERAL>::iterator itr_InsertItem = m_vecPrizeItem.begin();

	for( ; itr_InsertItem != m_vecPrizeItem.end(); itr_InsertItem++ )
	{
		if ( a_ItemUniqueNumber == itr_InsertItem->UniqueNumber )
		{
			m_vecPrizeItem.erase ( itr_InsertItem ); 
			break;
		}
	}

	// Update ��� üũ.
	vector<MSG_FC_TRADE_UPDATE_ITEM_COUNT>::iterator itr_UpdateItem = m_vecPrizeItemCount.begin();
	
	for ( ; itr_UpdateItem != m_vecPrizeItemCount.end(); itr_UpdateItem++ )
	{
		if ( a_ItemUniqueNumber == itr_UpdateItem->ItemUniqueNumber )
		{
			m_vecPrizeItemCount.erase ( itr_UpdateItem ); 
			break;
		}
	}
}


void CINFLuckyMachine::PrizeItemGiveMe()
{
	if(!m_bPrizeItem)
	{
		return;
	}
	else if(!IsAllRuletDone())
	{
		// "��Ű�ӽ��� ������ ���� �Ŀ� �������� ���� �� �ֽ��ϴ�."
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081202_0100,COLOR_ERROR);
		return;
	}

	//if(	m_bSlotDone[0] && m_bSlotDone[1] && m_bSlotDone[2])
	vector<ITEM_GENERAL>::iterator it = m_vecPrizeItem.begin();
	while(it != m_vecPrizeItem.end())
	{
		ITEM_GENERAL itemGeneral = (*it);
		g_pStoreData->PutItem((char*)&itemGeneral, TRUE);
		char buf[256];
		
		if(IS_COUNTABLE_ITEM(itemGeneral.ItemInfo->Kind))
		{
			wsprintf(buf,STRMSG_C_081124_0103,itemGeneral.ItemInfo->ItemName,itemGeneral.CurrentCount);
		}
		else
		{
			wsprintf(buf,STRMSG_C_081124_0103,itemGeneral.ItemInfo->ItemName,1);
		}

		g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
		it++;
	}
	vector<MSG_FC_TRADE_UPDATE_ITEM_COUNT>::iterator itc =m_vecPrizeItemCount.begin();
	while(itc != m_vecPrizeItemCount.end())
	{
		//g_pStoreData->PutItem((char*)((ITEM_GENERAL*)it), TRUE);
		
		char buf[256];
		ZERO_MEMORY(buf);
		
		if(itc->ItemUniqueNumber == g_pGameMain->m_pInven->GetItemSpiUniqueNumber())
		{
			// SPI�������� �Ծ���.				
			wsprintf( buf, STRMSG_C_ITEM_0002, itc->NewCount - g_pGameMain->m_pInven->GetItemSpi());//"%d ���ǰ� �߰� �Ǿ����ϴ�."
			g_pGameMain->m_pInven->SetItemSpi(itc->NewCount);
		}
		else
		{
			CItemInfo* pItem = g_pStoreData->FindItemInInventoryByUniqueNumber( itc->ItemUniqueNumber );
			if(pItem)
			{
				if(IS_COUNTABLE_ITEM(pItem->ItemInfo->Kind))
				{
					wsprintf(buf,STRMSG_C_081124_0103,pItem->ItemInfo->ItemName,itc->NewCount - pItem->CurrentCount);
				}
				else
				{
					wsprintf(buf,STRMSG_C_081124_0103,pItem->ItemInfo->ItemName,1);
				}
				g_pStoreData->UpdateItemCount( itc->ItemUniqueNumber, itc->NewCount );				
			}
		}
		if(strlen(buf) > 0)
		{
			g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
		}
		itc++;		
	}

	m_vecPrizeItem.clear();
	m_vecPrizeItemCount.clear();
	m_bPrizeItem = FALSE;

	for(int i=0; i<3; i++)
	{
		m_nAME_EffectNum[i] = 0.0f;
	}	
	Init();
	ChangeState(LUCKY_STATE_READY);
}

void CINFLuckyMachine::GambleButtonOK()
{
	if(m_bPrizeItem)			// �������� �ִٸ� �׺��� ���� �ʴ´�.
		return;
	ClearSearchItem();
	if(LUCKY_STATE_RULLET == m_nState ||
		LUCKY_STATE_PRIZE == m_nState)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081202_0101,COLOR_ERROR);
		return;
	}
	else if(NULL == m_pCoinItemInfo)
	{
		//"��ȭ�� �����ϴ�. ��ȭ�� �ְ� �ٽ� �õ��� �ּ���."
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081124_0100,COLOR_ERROR);
		return;
	}
	else if(CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind) > g_pGameMain->m_pInven->GetInvenFreeSize())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081124_0101,COLOR_ERROR);
		return;
	}

	int nSlot = 0;	
	for(nSlot = 0 ; nSlot < CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind); nSlot++)
	{	
		m_fTimeAllTimeStepSlot[nSlot] = 0.0f;
		m_nSuccessSlot[nSlot]	= 0;		// ���� ��ġ
		m_nSlotType[nSlot] = nSlot;		
	}
	if(LUCKY_SELECT_TYPE_MINI == m_struSelLuckyMachine.MachineKind)
	{
		// �̴ϴ� ���� ���� ��������
		m_nSlotType[0] = 2;		
	}


	if(m_struSelLuckyMachine.MachineNum != 0)
	{
		MSG_FC_ITEM_USE_LUCKY_ITEM sMsg;

		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
		memset(&sMsg, 0x00, sizeof(MSG_FC_ITEM_USE_LUCKY_ITEM));
		sMsg.MachineNum = m_struSelLuckyMachine.MachineNum;
		// end 2009-03-04 by bhsohn ��Ű �ӽ� ������

		sMsg.ItemUID = m_pCoinItemInfo->UniqueNumber;

		ClearConinItemInfo();

		g_pFieldWinSocket->SendMsg(T_FC_ITEM_USE_LUCKY_ITEM,(char*)&sMsg,sizeof(sMsg));
		g_pD3dApp->m_pSound->PlayD3DSound(SOUND_LUCKY_SLOT, D3DXVECTOR3(0,0,0), FALSE);		

		// 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
// 		CItemInfo* pCoinSelItem = g_pStoreData->FindItemInInventoryByUniqueNumber(sMsg.ItemUID);
// 		if(pCoinSelItem && pCoinSelItem->ItemInfo)
// 		{
// 			// ������ �̹� ������ �ٽ� ���� 
// 			UpLoadCoin((ITEM_GENERAL*)pCoinSelItem);
// 		}
		// end 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��

	}	

}
void CINFLuckyMachine::ChangeState(int i_nState)
{
	switch(i_nState)
	{
	case LUCKY_STATE_READY:
		ResetData();
		break;
	case LUCKY_STATE_RULLET:
		{
			vector<LUCKY_ITEM_SIMPLEINFO>::iterator it = m_vecTakeItem.begin();
			while(it != m_vecTakeItem.end())
			{
				LUCKY_ITEM_SIMPLEINFO stLucky = (*it);
				if(-1 == FindSearchItem(stLucky.SlotNum,stLucky.MysteryItemDropNum))
				{
					//m_bSlotDone[stLucky.SlotNum-1] = TRUE;
					//DBGOUT("Lucky Machine Error(count deficiency) : %d Slot %d MysteryItem\n",stLucky.SlotNum,stLucky.MysteryItemDropNum);
				}
				it++;
			}
			// ���Ϳ� �ֵ��ϰ� �� ���� ���� �Ϸ� ǰ�� ã��.
			UpdateSuccessSlot();			

		}
		break;
	case LUCKY_STATE_PRIZE:
		{
			// 2008-12-29 by bhsohn ��Ű �ӽ� ���� ����Ʈ �߰�
			vector<LUCKY_ITEM_SIMPLEINFO>::iterator it = m_vecTakeItem.begin();
			while(it != m_vecTakeItem.end())
			{
				LUCKY_ITEM_SIMPLEINFO stLucky = (*it);
				if(IsShowLucySuccessEffect(stLucky.MysteryItemDropNum, stLucky.Itemnum))
				{
					ShowLuckySuccessEffect();					
					break;
				}
				it++;
			}
			// end 2008-12-29 by bhsohn ��Ű �ӽ� ���� ����Ʈ �߰�
			g_pD3dApp->m_pSound->StopD3DSound(SOUND_LUCKY_SLOT);
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081202_0102,COLOR_ERROR);					
			
		}
		break;
	}
	m_nState = i_nState;
}

void CINFLuckyMachine::OnCloseInfWnd()
{
	// 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
	if(m_pCoinItemInfo)
	{	
// 		m_pCoinItemInfo->ItemWindowIndex = POS_INVALID_POSITION;
// 		if(g_pStoreData)
// 		{
// 			g_pStoreData->PutItem((char*)((ITEM_GENERAL*)m_pCoinItemInfo), TRUE);
// 		}
// 		util::del(m_pCoinItemInfo);
// 		m_pCoinItemInfo = NULL;
		m_pCoinItemInfo->ItemWindowIndex = POS_INVALID_POSITION;

 		CItemInfo* pCoinSelItem = g_pStoreData->FindItemInInventoryByUniqueNumber(m_pCoinItemInfo->UniqueNumber);
 		if(pCoinSelItem && pCoinSelItem->ItemInfo)
 		{
 			// �̹� ������ ������ ī��Ʈ ���� ������			
 			g_pStoreData->UpdateItemCount(m_pCoinItemInfo->UniqueNumber, pCoinSelItem->CurrentCount + m_pCoinItemInfo->CurrentCount);
 		}
		// 2009-04-14 by bhsohn �κ� 3�� �̸��Ͻ�, ��Ű�ӽ� �� ���� ����
		else if(NULL == pCoinSelItem)
		{
			g_pStoreData->PutItem((char*)((ITEM_GENERAL*)m_pCoinItemInfo), TRUE);			
		}
		// end 2009-04-14 by bhsohn �κ� 3�� �̸��Ͻ�, ��Ű�ӽ� �� ���� ����
		ClearConinItemInfo();
	}
	// end 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
	PrizeItemGiveMe();
	// 2010. 05. 27 by jskim �ű� ��Ű �ӽ� ���� - ���� ����
	m_fYouLuckyViewTime = 0.0f;
	m_dwYouLuckyAlpha = 0;		
	m_bYouLucky = FALSE;
	//end 2010. 05. 27 by jskim �ű� ��Ű �ӽ� ���� - ���� ����
}
BOOL CINFLuckyMachine::IsCloseLucky()
{
	if(LUCKY_STATE_RULLET == m_nState ||
		LUCKY_STATE_PRIZE == m_nState)
	{
		return FALSE;
	}
	return TRUE;
}
UID64_t CINFLuckyMachine::GetSelUID()
{
	if(m_pCoinItemInfo)
		return m_pCoinItemInfo->UniqueNumber;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�� ��ġ�� ����Ʈ ���̰� �Ѵ�.
/// \author		// 2008-12-29 by bhsohn ��Ű �ӽ� ���� ����Ʈ �߰�
/// \date		2008-12-29 ~ 2008-12-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFLuckyMachine::IsShowLucySuccessEffect(INT	i_MysteryItemDropNum, int i_nMysteryNum)
{
	if(LUCKYMACHINE_BLANK_ICON_NUMBER == i_nMysteryNum)
	{
		return FALSE;		
	}
	MYSTERY_ITEM_DROP*	pMisteyItemDrop = g_pDatabase->GetMysteryInfo(i_MysteryItemDropNum, i_nMysteryNum);
	if(NULL == pMisteyItemDrop)
	{
		return FALSE;
	}

	if(pMisteyItemDrop->Period > 0)
	{
		return TRUE;
	}
	
	return FALSE;	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�� ��ġ�� ����Ʈ ���̰� �Ѵ�.
/// \author		// 2008-12-29 by bhsohn ��Ű �ӽ� ���� ����Ʈ �߰�
/// \date		2008-12-29 ~ 2008-12-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::ShowLuckySuccessEffect()
{
	// 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
	m_bYouLucky = TRUE;	
	m_fYouLuckyViewTime= 0.0f;
	m_dwYouLuckyAlpha= 0;
	// end 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��

	D3DXVECTOR3	vPos = g_pD3dApp->GetMyShuttleCharacterPos();	
	D3DXVECTOR3	vVel, vUp;	
	
	vUp = D3DXVECTOR3(0,1,0);
	
	vVel = g_pD3dApp->GetViewDirVel();
	vVel.y = 0;
	D3DXVec3Normalize(&vVel,&vVel);

	g_pD3dApp->CreateMyAppEffectData(RC_EFF_LUCKY_SUCCESS, vPos, vVel, vUp); // �� ��ġ�� ����Ʈ ����

	// ����Ʈ�� �ٸ� ����� �����ְ��Ѵ�
	{
		D3DXVECTOR3	vSVel, vSUp;	
		
		vSUp = D3DXVECTOR3(0,1,0);		
		vSVel = D3DXVECTOR3(0,0,0); // ���⺤�Ͱ� ũ�Ⱑ 0�̸� �޴� ��� ���⺤�ͷ� �Ѵ�.		

		MSG_FC_CHARACTER_SHOW_MAP_EFFECT sMsg;
		memset(&sMsg, 0x00, sizeof(MSG_FC_CHARACTER_SHOW_MAP_EFFECT));
		
		sMsg.avec3Position		= vPos;
		sMsg.avec3Target		= vSVel*1000.0f;
		sMsg.avec3Up			= vSUp*1000.0f;
		sMsg.nLifetime			= 10;
		sMsg.EffectIndex		= RC_EFF_LUCKY_SUCCESS;
		
		g_pD3dApp->SendCharcterShowMapEffect(&sMsg);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		UINT		BuildingIndex;							// �ǹ�(���� ��) ���� ��ȣ
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::EnterMachineShop(UINT i_uBuildingIndex)
{	
	m_nLuckySelectStartIdx = 0;
	memset(&m_struSelLuckyMachine, 0x00, sizeof(LUCKY_MACHINE_OMI));			// ������ ��Ű ���� �ʱ�ȭ
	m_nLuckyMechineStep = -1;

	m_uBuildingIndex = i_uBuildingIndex;

	m_bInfluenceOneShopMode = FALSE;
	
	// ���� ���� �ʱ�ȭ
	ClearConinItemInfo();
	
	LUCKY_MACHINE_OMI stSelLuckyMachine;	
	if(GetInfluenceOneShop(&stSelLuckyMachine))
	{	
		// �̹����� �Ѱ��ִ�.
		CreateNPCListBtn();
		m_bInfluenceOneShopMode = TRUE;
		// NPCShop��ư �ڵ� Ŭ��
		OnClickSelectNPCShop(&stSelLuckyMachine);		
	}
// 	else
// 	{
// 		SetLuckyMechineStep(LUCKY_MAINSTEP_SELECT_SHOP);	
// 	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::SetLuckyMechineStep(int nLuckyMechineStep)
{	
	int nOldLuckyMechineStep = m_nLuckyMechineStep;

	m_nLuckyMechineStep = nLuckyMechineStep;

	if(nOldLuckyMechineStep == nLuckyMechineStep)
	{
		// ���ϰ� ������ ������Ʈ �� �ʿ����.
		return;		
	}
	RefreshLuckyMachineCtrl();	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::RefreshNPCListBtnStat()
{
// 	AddNPCListBtn(1000);
// 	AddNPCListBtn(1001);
// 	AddNPCListBtn(1002);
// 	AddNPCListBtn(1003);

// 	
// 	int nLuckyMachineLen = g_pDatabase->GetLuckyMachineLen();
// 	int nCnt = 0;
// 	for(nCnt = 0;nCnt<nLuckyMachineLen;nCnt++)
// 	{
// 		LUCKY_MACHINE_OMI*  pLuckyMachine = g_pDatabase->GetLuckyMachineInfo(nCnt);
// 		if(NULL == pLuckyMachine)
// 		{
// 			break;
// 		}
// 		AddNPCListBtn(pLuckyMachine->SourceIndex);
// 	}
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::CreateNPCListBtn()
{
	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	
	// ���� Ÿ������
	// 2009-05-13 by bhsohn ��Ű ���� ���� ����
	if((myShuttleInfo.InfluenceType != m_nLoadNpcListInfluenceType)
		||(m_uLoadBuildingIndex != m_uBuildingIndex))
	{
		// ������ Ʋ���ų� ���� �ε����� Ʋ���� ��ư �ʱ�ȭ
		DeleteDeviceObjectsNPCListBtn();		
	}
	m_nLoadNpcListInfluenceType = myShuttleInfo.InfluenceType;	
	// 2009-05-13 by bhsohn ��Ű ���� ���� ����
	m_uLoadBuildingIndex = m_uBuildingIndex;
	// end 2009-05-13 by bhsohn ��Ű ���� ���� ����

	int nLuckyMachineLen = g_pDatabase->GetLuckyMachineLen();
	int nCnt = 0;
	for(nCnt = 0;nCnt<nLuckyMachineLen;nCnt++)
	{
		LUCKY_MACHINE_OMI*  pLuckyMachine = g_pDatabase->GetLuckyMachineInfo(nCnt);
		if(NULL == pLuckyMachine)
		{
			break;
		}
		
		if(IsHasNPCBtn(pLuckyMachine) // �̹� �ִ� �ҽ��� �������� �ѱ���
			|| (!IsMyBelignece(pLuckyMachine))) // ���� �����̳�>?
		{			
			continue;
		}		

		structNPCList* pstruNPCList = new structNPCList;

		memcpy(&pstruNPCList->struLuckyMachineOmi, pLuckyMachine, sizeof(LUCKY_MACHINE_OMI));

		int nSourceIndex = pstruNPCList->struLuckyMachineOmi.SourceIndex;

		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "%d_3", nSourceIndex);
		wsprintf(szDownBtn, "%d_1", nSourceIndex);
		wsprintf(szSelBtn, "%d_0", nSourceIndex);
		wsprintf(szDisBtn, "%d_2", nSourceIndex);

		// 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����
		//pstruNPCList->pInfImageBtn = new CINFImageBtn;
		pstruNPCList->pInfImageBtn = new CINFAUTOSPRITEBTN( LUCKY_MACHINE_BTN_FRAME_COUNT, LUCKY_MACHINE_BTN_FRAME );
		// end 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����
		pstruNPCList->pInfImageBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		pstruNPCList->pInfImageBtn->RestoreDeviceObjects();
		pstruNPCList->pInfImageBtn->ShowWindow(FALSE);

		char chTmp[16];		
		DataHeader	* pDataHeader = NULL;
		
		wsprintf(chTmp, "%d_5", nSourceIndex);
		pDataHeader = FindResource(chTmp);
		if(pDataHeader)
		{
			pstruNPCList->pInfShopImage[0] = new CINFImageEx;
			pstruNPCList->pInfShopImage[0]->InitDeviceObjects(pDataHeader );
			pstruNPCList->pInfShopImage[0]->RestoreDeviceObjects();
		}

		wsprintf(chTmp, "%d_6", nSourceIndex);
		pDataHeader = FindResource(chTmp);
		if(pDataHeader)
		{
			pstruNPCList->pInfShopImage[1] = new CINFImageEx;
			pstruNPCList->pInfShopImage[1]->InitDeviceObjects(pDataHeader );
			pstruNPCList->pInfShopImage[1]->RestoreDeviceObjects();
		}

		pstruNPCList->bReStore = TRUE;		
		
		m_vecstruShopList.push_back(pstruNPCList);		
	}

	{
		if(((int)m_vecstruShopList.size()) < MAX_ONEPAGE_NPCLIST_COUNT)
		{
			// �������� �� ���� ������ ä������Ѵ�.
			int nDummyLen = MAX_ONEPAGE_NPCLIST_COUNT - ((int)m_vecstruShopList.size());
			int nCnt = 0;
			for(nCnt = 0;nCnt < nDummyLen;nCnt++)
			{
				structNPCList* pstruNPCList = new structNPCList;
				
				// �ʱ�ȭ
				memset(&pstruNPCList->struLuckyMachineOmi, 0x00, sizeof(LUCKY_MACHINE_OMI));			
				
				pstruNPCList->struLuckyMachineOmi.SourceIndex = NOT_OPEN_LUCKY_MACHINE;			// ��������
				int nSourceIndex = pstruNPCList->struLuckyMachineOmi.SourceIndex;
				
				char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
				wsprintf(szUpBtn, "%d", nSourceIndex);
				wsprintf(szDownBtn, "%d", nSourceIndex);
				wsprintf(szSelBtn, "%d", nSourceIndex);
				wsprintf(szDisBtn, "%d", nSourceIndex);
				
				// 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����
				//pstruNPCList->pInfImageBtn = new CINFImageBtn;
				pstruNPCList->pInfImageBtn = new CINFAUTOSPRITEBTN( LUCKY_MACHINE_BTN_FRAME_COUNT, LUCKY_MACHINE_BTN_FRAME );
				// end 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����
				pstruNPCList->pInfImageBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
				pstruNPCList->pInfImageBtn->RestoreDeviceObjects();
				pstruNPCList->pInfImageBtn->ShowWindow(FALSE);
				
				pstruNPCList->pInfShopImage[0] = NULL;
				pstruNPCList->pInfShopImage[1] = NULL;
				
				pstruNPCList->bReStore = TRUE;
				
				m_vecstruShopList.push_back(pstruNPCList);				
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFLuckyMachine::IsMyBelignece(LUCKY_MACHINE_OMI* pLuckyMechineOmi)
{
	if(m_uBuildingIndex == pLuckyMechineOmi->BuildingInx)
	{
		return TRUE;
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFLuckyMachine::IsHasNPCBtn(LUCKY_MACHINE_OMI* pLuckyMechineOmi)
{	
	vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
	while(itNpcBtn != m_vecstruShopList.end())
	{
		structNPCList* pstruNPCList = (*itNpcBtn);
		if((pLuckyMechineOmi->BuildingInx == pstruNPCList->struLuckyMachineOmi.BuildingInx)
			&&(pLuckyMechineOmi->MachineNum == pstruNPCList->struLuckyMachineOmi.MachineNum))
		{
			return TRUE;
		}
		itNpcBtn++;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::InitDeviceObjectsNPCListBtn()
{
	vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
	while(itNpcBtn != m_vecstruShopList.end())
	{
		structNPCList* pstruNPCList = (*itNpcBtn);
		
		if(NULL == pstruNPCList->pInfImageBtn)
		{
			char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
			wsprintf(szUpBtn, "%d_3", pstruNPCList->struLuckyMachineOmi.SourceIndex);
			wsprintf(szDownBtn, "%d_1", pstruNPCList->struLuckyMachineOmi.SourceIndex);
			wsprintf(szSelBtn, "%d_0", pstruNPCList->struLuckyMachineOmi.SourceIndex);
			wsprintf(szDisBtn, "%d_2", pstruNPCList->struLuckyMachineOmi.SourceIndex);
			
			// 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����
			//pstruNPCList->pInfImageBtn = new CINFImageBtn;
			pstruNPCList->pInfImageBtn = new CINFAUTOSPRITEBTN( LUCKY_MACHINE_BTN_FRAME_COUNT, LUCKY_MACHINE_BTN_FRAME );
			// end 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����
			pstruNPCList->pInfImageBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
			
			pstruNPCList->bReStore = FALSE;
		}
		int nCnt = 0;
		for(nCnt = 0;nCnt < MAX_SHOP_BG;nCnt++)
		{
			if(NULL == pstruNPCList->pInfShopImage[nCnt])
			{
				char chTmp[16];							
				DataHeader	* pDataHeader = NULL;					
				wsprintf(chTmp, "%d_%d", pstruNPCList->struLuckyMachineOmi.SourceIndex, (5+nCnt));
				
				pDataHeader = FindResource(chTmp);
				if(pDataHeader)
				{
					pstruNPCList->pInfShopImage[nCnt] = new CINFImageEx;
					pstruNPCList->pInfShopImage[nCnt]->InitDeviceObjects(pDataHeader);
				}					
			}
		}
		
		itNpcBtn++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::RestoreDeviceObjectsNPCListBtn()
{
	vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
	while(itNpcBtn != m_vecstruShopList.end())
	{
		structNPCList* pstruNPCList = (*itNpcBtn);
		
		if( !pstruNPCList->bReStore)
		{
			if(pstruNPCList->pInfImageBtn)
			{
				pstruNPCList->pInfImageBtn->RestoreDeviceObjects();					
			}
			int nCnt = 0;
			for(nCnt = 0;nCnt < MAX_SHOP_BG;nCnt++)
			{
				if(pstruNPCList->pInfShopImage[nCnt])
				{
					pstruNPCList->pInfShopImage[nCnt]->RestoreDeviceObjects();								
				}
			}
			pstruNPCList->bReStore = TRUE;
		}
		
		itNpcBtn++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::InvalidateDeviceObjectsNPCListBtn()
{	
	vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
	while(itNpcBtn != m_vecstruShopList.end())
	{
		structNPCList* pstruNPCList = (*itNpcBtn);
		
		if(pstruNPCList->bReStore)
		{
			if(pstruNPCList->pInfImageBtn )
			{
				pstruNPCList->pInfImageBtn->InvalidateDeviceObjects();		
			}
			int nCnt = 0;
			for(nCnt = 0;nCnt < MAX_SHOP_BG;nCnt++)
			{
				if(pstruNPCList->pInfShopImage[nCnt])
				{
					pstruNPCList->pInfShopImage[nCnt]->InvalidateDeviceObjects();								
				}
			}
			
			pstruNPCList->bReStore = FALSE;
		}
		
		itNpcBtn++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::DeleteDeviceObjectsNPCListBtn()
{
	vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
	while(itNpcBtn != m_vecstruShopList.end())
	{
		structNPCList* pstruNPCList = (*itNpcBtn);
		
		if(pstruNPCList->pInfImageBtn)
		{
			pstruNPCList->bReStore = FALSE;
			pstruNPCList->pInfImageBtn->DeleteDeviceObjects();					
			util::del(pstruNPCList->pInfImageBtn);			
		}
		int nCnt = 0;
		for(nCnt = 0;nCnt < MAX_SHOP_BG;nCnt++)
		{
			if(pstruNPCList->pInfShopImage[nCnt])
			{
				pstruNPCList->pInfShopImage[nCnt]->DeleteDeviceObjects();					
				util::del(pstruNPCList->pInfShopImage[nCnt]);							
			}
		}
		util::del((*itNpcBtn));		
		itNpcBtn++;
	}	
	
	m_vecstruShopList.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::ShowNPCListBtn(BOOL i_bShow)
{
	vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
	while(itNpcBtn != m_vecstruShopList.end())
	{
		structNPCList* pstruNPCList = (*itNpcBtn);
		
		if(pstruNPCList->pInfImageBtn)
		{
			pstruNPCList->pInfImageBtn->ShowWindow(i_bShow);
		}		
		itNpcBtn++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// NPC��ư ��ġ ����
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFLuckyMachine::RefreshNPCListBtnPos()
{
	BOOL bRtnValue = TRUE;	// �Ѱ��� X�� �ֳ�?	
	
	// 2009. 11. 09 by ckPark ��Ű�ӽ� ������ư ���� ����
	//m_nLuckySelectStartIdx = 0;
	// end 2009. 11. 09 by ckPark ��Ű�ӽ� ������ư ���� ����

	vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
	int nCnt = 0;
	for(nCnt = 0;nCnt < m_nLuckySelectStartIdx;nCnt++)
	{
		if(itNpcBtn == m_vecstruShopList.end())
		{
			break;
		}
		structNPCList* pstruNPCList = (*itNpcBtn);
		
		if(pstruNPCList->pInfImageBtn)
		{
			pstruNPCList->pInfImageBtn->ShowWindow(FALSE);
		}		

		itNpcBtn++;

	}
	// ������ ��
	for(nCnt = 0;nCnt < MAX_ONEPAGE_NPCLIST_COUNT;nCnt++)
	{
		if(itNpcBtn == m_vecstruShopList.end())
		{
			break;
		}
		structNPCList* pstruNPCList = (*itNpcBtn);
		
		if(pstruNPCList->pInfImageBtn)
		{
			if(NOT_OPEN_LUCKY_MACHINE == pstruNPCList->struLuckyMachineOmi.SourceIndex)
			{
				// ��������
				bRtnValue = FALSE;
			}
			int nPosX = LUCKYMACHINE_SELECT_SHOP_NPC_X+(nCnt*LUCKYMACHINE_SELECT_SHOP_NPC_WIDTH);
			int nPosY = LUCKYMACHINE_SELECT_SHOP_NPC_Y;
			pstruNPCList->pInfImageBtn->SetBtnPosition(nPosX, nPosY);
			pstruNPCList->pInfImageBtn->ShowWindow(TRUE);
		}		
		itNpcBtn++;
	}
	while(itNpcBtn != m_vecstruShopList.end())
	{
		structNPCList* pstruNPCList = (*itNpcBtn);
		
		if(pstruNPCList->pInfImageBtn)
		{
			pstruNPCList->pInfImageBtn->ShowWindow(FALSE);
		}		
		itNpcBtn++;
	}
	return bRtnValue;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		//NPCǥ��
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::OnClickNPCListPos(int i_nStep)
{	
	int nTmpLuckySelectStartIdx = m_nLuckySelectStartIdx;
	nTmpLuckySelectStartIdx += i_nStep;

	int nShopListLen = ((int)m_vecstruShopList.size()) - MAX_ONEPAGE_NPCLIST_COUNT;

	if(nTmpLuckySelectStartIdx > nShopListLen)
	{
		nTmpLuckySelectStartIdx = nShopListLen;
	}

	if(nTmpLuckySelectStartIdx < 0)
	{
		nTmpLuckySelectStartIdx = 0;
	}
	
	m_nLuckySelectStartIdx = nTmpLuckySelectStartIdx;

	// 2009. 11. 09 by ckPark ��Ű�ӽ� ������ư ���� ����
	DeleteDeviceObjectsNPCListBtn();
	// end 2009. 11. 09 by ckPark ��Ű�ӽ� ������ư ���� ����

	RefreshLuckyMachineCtrl();	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		//NPCǥ��
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::RefreshLuckyMachineCtrl()
{
	BOOL bPrevBtn, bTypeNorLuckyBtn, bTypeMiniLuckyBtn, bShopPrevBtn, bShopNextBtn, bNPCListBtn;	
	bPrevBtn = bTypeNorLuckyBtn = bTypeMiniLuckyBtn = bShopPrevBtn = bShopNextBtn = bNPCListBtn= FALSE;
	
	switch(m_nLuckyMechineStep)
	{	
	case LUCKY_MAINSTEP_SELECT_SHOP:
		{
			bShopPrevBtn = bShopNextBtn = TRUE;
			bPrevBtn = FALSE;
			bNPCListBtn = TRUE;
			CreateNPCListBtn();	// NPC��ư ����
		}
		break;
	case LUCKY_MAINSTEP_SELECT_GAMING:
		{
			bPrevBtn = TRUE;
			LUCKY_MACHINE_OMI stSelLuckyMachine;	
			if(GetInfluenceOneShop(&stSelLuckyMachine))
			{
				bPrevBtn = FALSE;
			}
		}
		break;
	}
	{
		m_pPrevStepBtn->ShowWindow(bPrevBtn);
	}	
	
	// ���� ���� 
	{
		m_pSelectShopPrevBtn->ShowWindow(bShopPrevBtn);
		m_pSelectShopNextBtn->ShowWindow(bShopNextBtn);

		if(bShopPrevBtn && bShopNextBtn)
		{
			m_pSelectShopPrevBtn->EnableBtn(TRUE);
			m_pSelectShopNextBtn->EnableBtn(TRUE);
		}
		
	}
	if(bNPCListBtn)
	{
		if(!RefreshNPCListBtnPos())	// NPC��ư ��ġ ����
		{
			// �Ѱ��� X�� �ֳ�?			
			m_pSelectShopPrevBtn->EnableBtn(FALSE);
			m_pSelectShopNextBtn->EnableBtn(FALSE);

		}
	}
	else
	{
		ShowNPCListBtn(bNPCListBtn);
	}	
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		//NPCǥ��
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::OnClickSelectNPCShop(LUCKY_MACHINE_OMI	*pSelLuckyMachine)
{		
	memcpy(&m_struSelLuckyMachine, pSelLuckyMachine, sizeof(LUCKY_MACHINE_OMI));

	if(CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind) > LUCKY_ITEM_LIST_NUM_LINE)
	{
		// �ִ� ���Լ��� ������ ����.
		m_struSelLuckyMachine.MachineKind = LUCKY_ITEM_LIST_NUM_LINE;
	}

	g_pGameMain->ShowShopInven(BUILDINGKIND_LUCKY, TRUE);
	SetLuckyMechineStep(LUCKY_MAINSTEP_SELECT_GAMING);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::OnClickPrevStepBtn()
{
	switch(m_nLuckyMechineStep)
	{ 		
	case LUCKY_MAINSTEP_SELECT_GAMING:		// ��Ű �ӽ� ������
		{
			if(m_bPrizeItem)			// �������� �ִٸ� �׺��� ���� �ʴ´�.
			{
				return;
			}
			
			if(!IsCloseLucky())		
			{
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081202_0101,COLOR_ERROR);
				return;
			}
			// 2009-05-11 by bhsohn ��Ű �ӽ� �ڷ� ���� Ű ���� ����
			if(m_pCoinItemInfo)
			{					
				m_pCoinItemInfo->ItemWindowIndex = POS_INVALID_POSITION;
				
				CItemInfo* pCoinSelItem = g_pStoreData->FindItemInInventoryByUniqueNumber(m_pCoinItemInfo->UniqueNumber);
				if(pCoinSelItem && pCoinSelItem->ItemInfo)
				{
					// �̹� ������ ������ ī��Ʈ ���� ������			
					g_pStoreData->UpdateItemCount(m_pCoinItemInfo->UniqueNumber, pCoinSelItem->CurrentCount + m_pCoinItemInfo->CurrentCount);
				}				
				else if(NULL == pCoinSelItem)
				{
					g_pStoreData->PutItem((char*)((ITEM_GENERAL*)m_pCoinItemInfo), TRUE);			
				}				
				ClearConinItemInfo();
			}
			// end 2009-05-11 by bhsohn ��Ű �ӽ� �ڷ� ���� Ű ���� ����
			// �κ��� ������
			g_pGameMain->m_pInven->ShowInven(NULL, NULL, FALSE, TRUE);

			// ������ ��Ű ���� �ʱ�ȭ
			memset(&m_struSelLuckyMachine, 0x00, sizeof(LUCKY_MACHINE_OMI));

			SetLuckyMechineStep(LUCKY_MAINSTEP_SELECT_SHOP);

			// �ʱ�ȭ
			ResetLuckyMachineInfo();
		}
		break;
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::RenderLuckyMachineSlot(int nSlotNum)
{
	if(m_vecIconNum[nSlotNum].empty() || m_vecTakeItem.size() <= nSlotNum)
	{
		return;
	}	

	CINFIcon* pIcon = ((CINFGameMain*)m_pParent)->m_pIcon;

	vector<ST_ICON_ITEM_NUMBER>::iterator it = m_vecIconNum[nSlotNum].begin();
	int nCnt = 0;
	for(nCnt = 0 ; nCnt < m_nSlotStartPosIdx[nSlotNum]; nCnt++)
	{
		if(it == m_vecIconNum[nSlotNum].end())
		{
			it = m_vecIconNum[nSlotNum].begin();
		}
		it++;
	}

	int nSlotPosX = 0;
	int nSlotPosY = 0;
	int nItemPos = 0;
	
	int nMaxItemSlotCnt = min(LUCKY_ITEM_LIST_NUM, m_vecIconNum[nSlotNum].size());

	int nSlotPosIdx = nSlotNum;
	if(LUCKY_SELECT_TYPE_MINI == CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind))
	{
		nSlotPosIdx = LUCKY_SELECT_TYPE_MINI;
	}

	for(nItemPos = 0 ; nItemPos < nMaxItemSlotCnt; nItemPos++)
	{
		if(it == m_vecIconNum[nSlotNum].end())
		{
			it = m_vecIconNum[nSlotNum].begin();
		}
		
		ST_ICON_ITEM_NUMBER stItemNum = (*it);				
		
		{
			char buf[20];
			
			wsprintf(buf, "%08d",stItemNum.nIconNum);
			pIcon->SetIcon( buf, LUCKYMACHINE_SLOT_X+(LUCKYMACHINE_SLOT_GAP_X*nItemPos),
								LUCKYMACHINE_SLOT_Y+(LUCKYMACHINE_SLOT_GAP_Y*nSlotPosIdx), 1.0f );
			pIcon->Render();
			// 2010. 05. 12 by jskim �ű� ��Ű �ӽ� �߰���� - ��Ű�ӽſ��� �޴� �����۸� �����ֱ�
			if(nItemPos != 3)
			{
				m_pImgDisableItem->Move(LUCKYMACHINE_SLOT_X+(LUCKYMACHINE_SLOT_GAP_X*nItemPos), 
										LUCKYMACHINE_SLOT_Y+(LUCKYMACHINE_SLOT_GAP_Y*nSlotPosIdx));

				m_pImgDisableItem->SetScale(pIcon->GetIconSize().x, pIcon->GetIconSize().y);
				m_pImgDisableItem->Render();	
			}
			else
			{
				if(m_nAME_EffectNum[nSlotNum] != 0.0f && m_nAME_EffectNum[nSlotNum] <= 0.5f)
				{
					m_pAME_slot[0]->Move(LUCKYMACHINE_SLOT_X+(LUCKYMACHINE_SLOT_GAP_X*nItemPos) - 10,
						LUCKYMACHINE_SLOT_Y+(LUCKYMACHINE_SLOT_GAP_Y*nSlotPosIdx) - 10);
					m_pAME_slot[0]->Render();
				}
				else if(m_nAME_EffectNum[nSlotNum] > 0.5f)
				{
					m_pAME_slot[1]->Move(LUCKYMACHINE_SLOT_X+(LUCKYMACHINE_SLOT_GAP_X*nItemPos)  - 10,
						LUCKYMACHINE_SLOT_Y+(LUCKYMACHINE_SLOT_GAP_Y*nSlotPosIdx) - 10);
					m_pAME_slot[1]->Render();
				}
			}
		}
		it++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFLuckyMachine::IsAllRuletDone()
{
	//int nRuletLen = m_struSelLuckyMachine.MachineKind;	// �ִ� �귿 ��
	int nRuletLen = CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind);
	int nCnt = 0;
	for(nCnt = 0;nCnt < nRuletLen;nCnt++)
	{
		if(!m_bSlotDone[nCnt])
		{
			return FALSE;
		}
	}		
	return TRUE;	

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::ResetLuckyMachineInfo()
{
	m_vecPrizeItem.clear();
	m_vecPrizeItemCount.clear();
	m_bPrizeItem = FALSE;
	Init();
	ChangeState(LUCKY_STATE_READY);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�ø��� �ִ� �����̳�?
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFLuckyMachine::IsPossibleUpLoadCoin(ITEM* i_pItem)
{	
	if(!COMPARE_BIT_FLAG(i_pItem->ItemAttribute, ITEM_ATTR_LUCKY_ITEM))
	{
		// ��Ű ���� ���� �ƴϴ�.
		//"\\y���������� �̿� �� �� �����ϴ�."
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080603_0100, COLOR_ERROR);
		return FALSE;
	}
	if(m_nState == LUCKY_STATE_RULLET)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081201_0101,COLOR_ERROR);
		return FALSE;
	}
	
	if(m_bPrizeItem)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081202_0102, COLOR_ERROR);
		return FALSE;
	}

	// 2009-04-14 by bhsohn �κ� 3�� �̸��Ͻ�, ��Ű�ӽ� �� ���� ����
	if(CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind) > g_pGameMain->m_pInven->GetInvenFreeSize())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081124_0101,COLOR_ERROR);
		return FALSE;
	}
	// end 2009-04-14 by bhsohn �κ� 3�� �̸��Ͻ�, ��Ű�ӽ� �� ���� ����

	int nMachineNum = m_struSelLuckyMachine.MachineNum;
	int nLuckyMachineLen = g_pDatabase->GetLuckyMachineLen();
	int nCnt = 0;
	for(nCnt = 0;nCnt<nLuckyMachineLen;nCnt++)
	{
		LUCKY_MACHINE_OMI*  pLuckyMachine = g_pDatabase->GetLuckyMachineInfo(nCnt);
		if(NULL == pLuckyMachine)
		{
			break;
		}
		if((nMachineNum == pLuckyMachine->MachineNum)
			&& (i_pItem->ItemNum == pLuckyMachine->CoinItemNum ))
		{
			return TRUE;
		}

	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�ø��� �ִ� �����̳�?
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::UpLoadCoin(ITEM_GENERAL* i_pItem)
{
	if(NULL == i_pItem)
	{
		return;
	}
	// LuckyMachine���� ���̴� �������̸� 								
	ClearConinItemInfo(); // �������� �ʱ�ȭ
	m_pCoinItemInfo = new CItemInfo(i_pItem);	
	
	// ������ �Ѱ��� �ö󰬴�.
	m_pCoinItemInfo->CurrentCount = 1;								 	
	g_pStoreData->UpdateItemCount(i_pItem->UniqueNumber, i_pItem->CurrentCount - m_pCoinItemInfo->CurrentCount);

	// 2009-05-18 by bhsohn ��Ű�������� �κ�Ǯ�ϋ��� ������ ó��
	util::del(m_pItemCoinItemTmp);
	m_pItemCoinItemTmp = new ITEM_GENERAL;
	memcpy(m_pItemCoinItemTmp, i_pItem, sizeof(ITEM_GENERAL));
	m_pItemCoinItemTmp->CurrentCount = 1;
	// end 2009-05-18 by bhsohn ��Ű�������� �κ�Ǯ�ϋ��� ������ ó��
	
	// �κ��� �ִ°� �� ������ 
	//g_pStoreData->UpdateItemCount(i_pItem->UniqueNumber, 0);	
	g_pD3dApp->m_pSound->PlayD3DSound(SOUND_LUCKY_COIN, D3DXVECTOR3(0,0,0), FALSE);			

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�κ��� Ǯ�̶�� ������ ������ ������ ���йǷ�, ������ ������� ������
/// \author		// 2009-05-18 by bhsohn ��Ű�������� �κ�Ǯ�ϋ��� ������ ó��
/// \date		2009-05-18 ~ 2009-05-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::ErrorLuckyMechineInvenFull()
{
	if(NULL == m_pItemCoinItemTmp)
	{
		return;
	}
	g_pStoreData->PutItem((char*)((ITEM_GENERAL*)m_pItemCoinItemTmp), TRUE);	

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�ø��� �ִ� �����̳�?
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::UpdateSuccessSlot()
{
	int nSlotPos = 0;	
	int nItemPos = 0;
	for(nSlotPos = 0;nSlotPos < LUCKY_ITEM_LIST_NUM_LINE;nSlotPos++)
	{
		int nTakeGetItemNum = m_vecTakeItem[nSlotPos].Itemnum;
		m_nSuccessSlot[nSlotPos]	= 0;		// ���� ��ġ ����
		nItemPos = 0;

		vector<ST_ICON_ITEM_NUMBER>::iterator itSearchItem = m_vecIconNum[nSlotPos].begin();
		while(itSearchItem != m_vecIconNum[nSlotPos].end())
		{
			ST_ICON_ITEM_NUMBER stIconNum = (*itSearchItem);
			if(stIconNum.nItemNum == nTakeGetItemNum)
			{				
				m_nSuccessSlot[nSlotPos] = nItemPos;
				break;
			}
			nItemPos++;
			itSearchItem++;
		}
	}
	// 2009-05-18 by bhsohn ��Ű�������� �κ�Ǯ�ϋ��� ������ ó��
	// ���� ��ġ�� ���Ž�, ���� ���� ���� �ʱ�ȭ
	util::del(m_pItemCoinItemTmp);	
	// end 2009-05-18 by bhsohn ��Ű�������� �κ�Ǯ�ϋ��� ������ ó��
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�ø��� �ִ� �����̳�?
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CINFImage*	CINFLuckyMachine::GetShopImageBk(int nEffectBG)
{
	vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
	while(itNpcBtn != m_vecstruShopList.end())
	{
		structNPCList* pstruNPCList = (*itNpcBtn);		
		
		if((m_struSelLuckyMachine.SourceIndex == pstruNPCList->struLuckyMachineOmi.SourceIndex )
			&& pstruNPCList->pInfShopImage[nEffectBG])
		{
			return 	pstruNPCList->pInfShopImage[nEffectBG];
		}		
		itNpcBtn++;
	}
	return NULL;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		������ ��ġ ����
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::UpdateCurrentSlotPos(int i_nSlot)
{
	int nSlotTmpPos = (FIRST_SLOT_MACHINE_SUCCESS_STEP) + RANDI(0, LUCKY_ITEM_POS - 1);
	switch(i_nSlot)
	{
	case 0:	
		{
			// 0�� ����
			nSlotTmpPos = (FIRST_SLOT_MACHINE_SUCCESS_STEP) + RANDI(0, LUCKY_ITEM_POS - 1);			
		}
		break;
	case 1:
		{
			// 1������
			nSlotTmpPos = (SECOND_SLOT_MACHINE_SUCCESS_STEP) + RANDI(0, LUCKY_ITEM_POS - 1);			
		}
		break;
	case 2:
		{
			// 2������
			nSlotTmpPos = (THIRD_SLOT_MACHINE_SUCCESS_STEP) + RANDI(0, LUCKY_ITEM_POS - 1);			
		}
		break;
	}
	switch(m_nSlotStep[i_nSlot])
	{
	case 1:
		{
			// ù��° ���ܿ� ���� ��ġ�� �����Ѵ�.
			
			int nSlotItemMax = ((int)m_vecIconNum[i_nSlot].size()) - RANDI(0, LUCKY_ITEM_POS - 1);
			int nSlotPos = min(nSlotTmpPos, nSlotItemMax);
			m_nSlotStartPosIdx[i_nSlot] = m_nSuccessSlot[i_nSlot] - nSlotPos;
		}
		break;
	}
	//
	// ���� ������
	if(m_nSlotStartPosIdx[i_nSlot] <0)
	{
		if(m_vecIconNum[i_nSlot].empty())
		{
			m_nSlotStartPosIdx[i_nSlot] = 0;
		}
		else
		{
			m_nSlotStartPosIdx[i_nSlot] = ((int)m_vecIconNum[i_nSlot].size()) + m_nSlotStartPosIdx[i_nSlot];
		}
	}
	else if(((int)m_vecIconNum[i_nSlot].size()) < m_nSlotStartPosIdx[i_nSlot])
	{
		m_nSlotStartPosIdx[i_nSlot] = m_nSlotStartPosIdx[i_nSlot] - ((int)m_vecIconNum[i_nSlot].size());
	}	
	
	// ī��Ʈ�� ���� �ȳ��� ��
	if(m_nSlotStartPosIdx[i_nSlot] <0)
	{
		if(m_vecIconNum[i_nSlot].empty())
		{
			m_nSlotStartPosIdx[i_nSlot] = 0;
		}
		else
		{
			m_nSlotStartPosIdx[i_nSlot] = ((int)m_vecIconNum[i_nSlot].size());
		}
	}
	else if(((int)m_vecIconNum[i_nSlot].size()) < m_nSlotStartPosIdx[i_nSlot])
	{
		m_nSlotStartPosIdx[i_nSlot] = 0;
	}		

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���¿� ������ �Ѱ� �ִ����� ���� ó��
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFLuckyMachine::GetInfluenceOneShop(LUCKY_MACHINE_OMI* o_pSelLuckyMachine)
{
	int nLuckyMachineLen = g_pDatabase->GetLuckyMachineLen();
	int nMyBeligenceCnt = 0;
	
	LUCKY_MACHINE_OMI	stTmpSelLuckyMachine;
	memset(&stTmpSelLuckyMachine, 0x00, sizeof(LUCKY_MACHINE_OMI));
	memset(o_pSelLuckyMachine, 0x00, sizeof(LUCKY_MACHINE_OMI));

	int nCnt = 0;	
	for(nCnt = 0;nCnt<nLuckyMachineLen;nCnt++)
	{
		LUCKY_MACHINE_OMI*  pLuckyMachine = g_pDatabase->GetLuckyMachineInfo(nCnt);
		if(NULL == pLuckyMachine)
		{
			continue;
		}
// 		if((pLuckyMachine->BuildingInx == stTmpSelLuckyMachine.BuildingInx)
// 			&&(pLuckyMachine->MachineNum == stTmpSelLuckyMachine.MachineNum))
// 		{
// 			continue;
// 		}
// 		else if(!IsMyBelignece(pLuckyMachine)) // ���� �����̳�>?
// 		{			
// 			continue;
// 		}		
		if(IsMyBelignece(pLuckyMachine) && pLuckyMachine->BuildingInx == m_uBuildingIndex)
		{
			memcpy(o_pSelLuckyMachine, pLuckyMachine, sizeof(LUCKY_MACHINE_OMI));
		return TRUE;
	}
//		memcpy(&o_pSelLuckyMachine, pLuckyMachine, sizeof(LUCKY_MACHINE_OMI));
//		nMyBeligenceCnt++;
	}
// 	if(1 == nMyBeligenceCnt)
// 	{
// 		// ������ �Ѱ��̴�.
// 		memcpy(o_pSelLuckyMachine, &stTmpSelLuckyMachine, sizeof(LUCKY_MACHINE_OMI));
// 		return TRUE;
// 	}
	return FALSE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		//NPCǥ��
/// \author		// 2009-03-04 by bhsohn ��Ű �ӽ� ������
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFLuckyMachine::IsInfluenceOneShopMode()
{
	return m_bInfluenceOneShopMode;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
/// \date		2009-04-02 ~ 2009-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CItemInfo* CINFLuckyMachine::GetConinItemInfo()
{
	return m_pCoinItemInfo;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
/// \date		2009-04-02 ~ 2009-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::ClearConinItemInfo()
{
	if(m_pCoinItemInfo)
	{
		m_pCoinItemInfo->ItemWindowIndex = POS_INVALID_POSITION;
		util::del(m_pCoinItemInfo);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
/// \date		2009-04-02 ~ 2009-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::TickYouLucky()
{	
	if(!m_bYouLucky)
	{
		return;
	}
	// 1�ð��� ������ �׋����� ����ش�. 	
	m_fYouLuckyViewTime += g_pD3dApp->GetCheckElapsedTime();	
	
	// 8�ʵ��� ����ְ� �����.
	if(m_fYouLuckyViewTime > MAX_YOU_LUCKY_TIME)
	{		
		m_fYouLuckyViewTime = 0.0f;
		m_dwYouLuckyAlpha = 0;		
		m_bYouLucky = FALSE;
		return;
	}	
	// ���̸� �������..
	//if(m_fYouLuckyViewTime > MAX_YOU_DELAY_LUCKY_TIME)
	{
		m_dwYouLuckyAlpha = g_pGameMain->GetCurrentColor(m_fYouLuckyViewTime, 
			ALPHA_DELAY_WAVE, 
			MAX_YOU_LUCKY_TIME,
			MAX_YOU_DELAY_LUCKY_TIME);	
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
/// \date		2009-04-02 ~ 2009-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::RenderYouLucky()
{		
	TickYouLucky();
	if(!m_bYouLucky)
	{
		return;
	}

	int nYouLucky = m_nLuckyEffectBG;
	if(nYouLucky >= MAX_LUCKY_YOULUCKY)
	{
		nYouLucky = 0;
	}
	DWORD dwYouLuckyAlpha = m_dwYouLuckyAlpha;
	if(m_fYouLuckyViewTime < MAX_YOU_DELAY_LUCKY_TIME)
	{
		dwYouLuckyAlpha = 0xFFFFFFFF;
	}
	else
	{
		nYouLucky = 0;
	}
	if(m_pImgYouLuckyBG[nYouLucky])
	{
		m_pImgYouLuckyBG[nYouLucky]->SetColor(dwYouLuckyAlpha);
		m_pImgYouLuckyBG[nYouLucky]->Move(LUCKYMACHINE_EFFECT_BG_X,0);
		m_pImgYouLuckyBG[nYouLucky]->Render();	
	}	
}