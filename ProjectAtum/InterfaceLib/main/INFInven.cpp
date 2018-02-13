// INFInven.cpp: implementation of the CINFInven class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RangeTime.h"
#include "INFInven.h"
#include "INFGameMain.h"
#include "AtumApplication.h"
#include "INFCityBase.h"
#include "GameDataLast.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "StoreData.h"
#include "INFTrade.h"
#include "Chat.h"
#include "INFIcon.h"
#include "AtumDatabase.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "FieldWinSocket.h"
#include "INFWindow.h"
#include "ItemInfo.h"
#include "AtumSound.h"
#include "Skill.h"
#include "Interface.h"
// 2008-09-22 by bhsohn EP3 ĳ���� â
//#include "INFCharacterInfo.h"
#include "INFCharacterInfoExtend.h"
#include "INFCommunity.h"
#include "INFGameMainQSlot.h"
#include "dxutil.h"
#include "TutorialSystem.h"
#include "INFChangeCharactor.h"
// 2007-12-05 by bhsohn ��Ż ����, ��� ���� ����
#include "ClientParty.h"
#include "INFImageEx.h"								// 2011. 10. 10 by jskim UI�ý��� ����

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define INVEN_SLOT_SIZE			30
#define INVEN_SLOT_INTERVAL		32
#define INVEN_ITEM_SLOT_START_X 309
#define INVEN_ITEM_SLOT_START_Y 30

#define INVEN_SCROLL_LINE_START_X		410
#define INVEN_SCROLL_LINE_START_Y		30
#define INVEN_SCROLL_BUTTON_START_X		406
#define INVEN_SCROLL_BUTTON_START_Y		30
#define INVEN_SCROLL_BUTTON_WIDTH		11
#define INVEN_SCROLL_BUTTON_HEIGHT		38

#define INVEN_SCROLL_LINE_LENGTH		190//(178/*187-INVEN_SCROLL_BUTTON_HEIGHT+8*/)// 15:���� ���� ������
#define INVEN_SCROLL_LINE_MOVE_LENGTH	(INVEN_SCROLL_LINE_LENGTH - INVEN_SCROLL_BUTTON_HEIGHT)
#define INVEN_SCROLL_NUMBER				((((float)(g_pStoreData->m_mapItemUniqueNumber.size()/ INVEN_X_NUMBER)-2 )<0)?0:(g_pStoreData->m_mapItemUniqueNumber.size() / INVEN_X_NUMBER-2))// min:0�ΰ�� ���ƾ� �Ѵ�.
#define INVEN_SCROLL_INTERVAL			(INVEN_SCROLL_LINE_LENGTH / SetScrollLine())

#define INVEN_SPI_START_X				414//333

#define INVEN_GARBAGE_START_X			271
#define INVEN_GARBAGE_START_Y			221
#define INVEN_GARBAGE_SIZE				24
#define INVEN_NOT_USE_SLOT_ICON			"07900270"

// ���� ��ġ
// ���� ������ ���� ��ġ
#define POS_PROW_X			16		// ��� ��(���̴�)
#define POS_PROW_Y			102
#define	POS_PROWIN_X		265		// ���� ���(��ǻ��)
#define	POS_PROWIN_Y		102	
#define	POS_PROWOUT_X		16		// ���� ��(�⺻����)
#define	POS_PROWOUT_Y		67	
#define	POS_WINGIN_X		141		// �Ӹ� (��ü ����� ���� ����)
#define	POS_WINGIN_Y		172//32//172	
#define	POS_WINGOUT_X		265		// ������ ��(��޹���)
#define	POS_WINGOUT_Y		67	
#define	POS_CENTER_X		16		// ���(�Ƹ�)
#define	POS_CENTER_Y		172	
#define	POS_REAR_X			265		// ��� ��(����)
#define	POS_REAR_Y			137	
#define	POS_ATTACHMENT_X	16		// ������ �Ʒ�(������ũ)
#define	POS_ATTACHMENT_Y	137	
#define POS_PET_X			265		// ���� �Ʒ�(������)
#define POS_PET_Y			172	

#define CITY_SHOP_INVEN_START_Y		(CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y)

// �κ��丮 ĳ���� ������
#define ROTATION_NONE			-1
#define ROTATION_LEFT			0
#define ROTATION_RIGHT			1
#define ROTATION_UP				2
#define ROTATION_DOWN			3
#define ROTATION_ORIGINAL		4

#define ROTATION_STATE_N		-1		// ����
#define ROTATION_STATE_O		0		// ����
#define ROTATION_STATE_P		1		// ����

#define ROTATION_BASE_GAB_W
#define ROTATION_BASE_GAB_H

#define INVEN_BACK_W			388
#define INVEN_BACK_H			237
#define INVEN_BACK_POS_X		19
#define INVEN_BACK_POS_Y		19

#define INVEN_FULL_BACK_W		426
#define INVEN_FULL_BACK_H		275

#define INVEN_WARPOINT_Y		242

// 2006-03-07 by ispark, �� ���� ��ġ ����
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn �±� ���� �߰�
#define INVEN_SPI_START_Y		222//233
#define SPI_START_X				70//68
#define SPI_START_Y				3//5
#define FONTWEITGHT_X1			98
#define FONTWEITGHT_X2			65
#else
#define INVEN_SPI_START_Y		222//233
#define SPI_START_X				68//130
#define SPI_START_Y				3//5
#define FONTWEITGHT_X1			87
#define FONTWEITGHT_X2			50
#endif

// 2006-04-04 by ispark
//-----------------------------------------------------------------------------
// Name: struct MIRRORVERTEX
// Desc: Custom mirror vertex type
//-----------------------------------------------------------------------------
struct MIRRORVERTEX
{
    D3DXVECTOR3 p;
    D3DXVECTOR2 uv;

    static const DWORD FVF;
};
const DWORD MIRRORVERTEX::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

CINFInven::CINFInven(CAtumNode* pParent)
{
	FLOG( "CINFInven(CAtumNode* pParent)" );
	m_pEqip = NULL;
	m_pInven = NULL;
	m_pItemSl = NULL ;
	m_pSpi = NULL;
	m_pScrollLine = NULL;
	m_pScrollButton = NULL;
	m_pInvenBase = NULL;
	// 2007-02-12 by bhsohn Item ���� ���� ó��
	m_pMultiItemSelImage = NULL;	
	// end 2007-02-12 by bhsohn Item ���� ���� ó��

	m_pParent = pParent;

	int i;
	for(i=0;i<INVEN_NUMBER;i++)
	{
		m_pInvenDisplayInfo[i] = NULL;
	}

	// 2010-06-15 by shcho&hslee ��ý��� - ����ǰ �ִ� ���� ����.
	//for(i=0;i<WEAR_ITEM_NUMBER;i++)
	for(i=0;i<MAX_EQUIP_POS;i++)
	{
		m_pWearDisplayInfo[i] = NULL;
	}
	m_nInvenCurrentScroll = 0;
	m_bScrollLock = FALSE;
	m_ptMouse.x = 0;
	m_ptMouse.y = 0;

	m_pSelectIcon = NULL;
	m_pSelectItem = NULL;
//	m_nSelectItemWindowPosition = -1;

	m_pFontSpi = NULL;
	m_pFontWeight[0] = NULL;
	m_pFontWeight[1] = NULL;
	m_pFontWeight[2] = NULL;

	for(i=0;i<INVEN_Y_NUMBER;i++)
	{
		m_pFontItemNum[i] = NULL;
	}
	m_nItemSpi = 0;
	m_hyItemSpiUniqueNumber = 0;
	
	// 2007-06-20 by bhsohn ������ ������, �޸� ���� ����
	//m_pDeleteItem = NULL;
	InitDeleteItemInfo();
	

	m_bEnchantState = FALSE;// ��æƮ �ó����� : 1. m_pSelectItem, m_pSelectIcon, m_bEnchantState ����
							//					 2. MsgBox(_Q_USE_ENCHANT_ITEM_CARD) ���� ������(cancel�� ���)
							//					 3. WM_LBUTTONDOWN�޽������� ���õ� �������� ������ ������ ����
							//					 4. �׷��� ���� ��� m_pSelectItem, m_pSelectIcon, m_bEnchantState �ʱ�ȭ
	m_pEnchantItem = NULL;
	m_bRestored = FALSE;
	m_bInvalidated = FALSE;
	m_pCurrentItemInfo = NULL;

	m_iWearPosition = POS_INVALID_POSITION;

	m_nRenderMoveIconIntervalHeight = 0;
	m_nRenderMoveIconIntervalWidth  = 0;

	m_tBlinkTime.Set( 0.0f, 1.5f, 0.5f );
	m_bSelectItem = FALSE;
	m_bSelectWearItem = FALSE;
	m_bMouseDownState = FALSE;

	m_pInvenRotationBase = NULL;
	for(i=0;i<INVEN_ROTATION_STATE;i++)
	{
		for(int j=0;j<INVEN_ROTATION_NUM;j++)
		{
			m_pInvenDirection[i][j] = NULL;
		}
	}
	m_nRotationState = ROTATION_NONE;
	m_nButtonState	= ROTATION_NONE;
	for(i=0; i<4; i++)
	{
		m_bByPush[i] = FALSE;
	}

	m_nInvenCurrentScrollPosition = 0;

	m_pMirrorVB      = NULL;				// 2006-04-04 by ispark
	m_pMirrorTexture = NULL;

	// 2007-06-20 by bhsohn ������ ������, �޸� ���� ����
	memset(&m_struDeleteItem, 0x00, sizeof(structDelItemInfo));
}

CINFInven::~CINFInven()
{
	FLOG( "~CINFInven()" );
	util::del(m_pEqip );
	util::del(m_pInven  );
	util::del(m_pItemSl);
	util::del(m_pSpi ); 
	util::del(m_pScrollLine);
	util::del(m_pScrollButton ); 
	util::del(m_pFontSpi ); 
	util::del(m_pFontWeight[0] ); 
	util::del(m_pFontWeight[1] ); 
	util::del(m_pFontWeight[2] ); 
	util::del(m_pInvenBase);
	// 2007-02-12 by bhsohn Item ���� ���� ó��
	util::del(m_pMultiItemSelImage);	
	// end 2007-02-12 by bhsohn Item ���� ���� ó��

	util::del(m_pInvenRotationBase);
	int i;
	for(i=0;i<INVEN_ROTATION_STATE;i++)
	{
		for(int j=0;j<INVEN_ROTATION_NUM;j++)
		{
			util::del(m_pInvenDirection[i][j]);
		}
	}

	for(i=0;i<INVEN_Y_NUMBER;i++)
	{
		util::del(m_pFontItemNum[i]);
	}
	for(i=0;i<INVEN_NUMBER;i++)
	{
		util::del(m_pInvenDisplayInfo[i]);
	}

	// 2010-06-15 by shcho&hslee ��ý��� - ����ǰ �ִ� ���� ����.
	//for(i=0;i<WEAR_ITEM_NUMBER;i++)
	for(i=0;i<MAX_EQUIP_POS;i++)
	{
		util::del(m_pWearDisplayInfo[i]);
	}

	SAFE_RELEASE( m_pMirrorVB );					// 2006-04-04 by ispark
	SAFE_RELEASE(m_pMirrorTexture);
}

HRESULT CINFInven::InitDeviceObjects()
{
	FLOG( "CINFInven::InitDeviceObjects()" );
	m_pFontSpi = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontSpi->InitDeviceObjects(g_pD3dDev);
	m_pFontWeight[0] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontWeight[0]->InitDeviceObjects(g_pD3dDev);
	m_pFontWeight[1] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontWeight[1]->InitDeviceObjects(g_pD3dDev);
	m_pFontWeight[2] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontWeight[2]->InitDeviceObjects(g_pD3dDev);

	int i;
	for(i=0;i<INVEN_Y_NUMBER;i++)
	{
		m_pFontItemNum[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),8, D3DFONT_ZENABLE,  TRUE,256,32);
		m_pFontItemNum[i]->InitDeviceObjects(g_pD3dDev);
	}
	
	DataHeader	* pDataHeader;
	
	m_pEqip = new CINFImageEx;
	pDataHeader = FindResource("equip");
	m_pEqip->InitDeviceObjects(pDataHeader ) ;

		m_pInven = new CINFImageEx;
	pDataHeader = FindResource("inven");
	m_pInven->InitDeviceObjects(pDataHeader ) ;

		m_pItemSl = new CINFImageEx;
	pDataHeader = FindResource("itemsl");
	m_pItemSl->InitDeviceObjects(pDataHeader ) ;

		m_pSpi = new CINFImageEx;
	pDataHeader = FindResource("spi");
	m_pSpi->InitDeviceObjects(pDataHeader ) ;	

		m_pScrollLine = new CINFImageEx;
	pDataHeader = FindResource("scrline");
	m_pScrollLine->InitDeviceObjects(pDataHeader ) ;

		m_pScrollButton = new CINFImageEx;
	pDataHeader = FindResource("c_scrlb");
	m_pScrollButton->InitDeviceObjects(pDataHeader ) ;	

	m_pWearPositionBlink = new CINFImageEx;
	pDataHeader = FindResource("iFocus");
	m_pWearPositionBlink->InitDeviceObjects(pDataHeader ) ;	

	m_pInvenBase = new CINFImageEx;
	pDataHeader = FindResource("cinven");
	m_pInvenBase->InitDeviceObjects(pDataHeader ) ;	

	m_pInvenRotationBase = new CINFImageEx;
	pDataHeader = FindResource("innom");
	m_pInvenRotationBase->InitDeviceObjects(pDataHeader ) ;

	for(i=0;i<INVEN_ROTATION_STATE;i++)
	{
		for(int j=0;j<INVEN_ROTATION_NUM;j++)
		{
			char buf[64];
			ZERO_MEMORY(buf);
			wsprintf(buf, "inven%d%d", i, j);

			m_pInvenDirection[i][j] = new CINFImageEx;
			pDataHeader = FindResource(buf);
			m_pInvenDirection[i][j]->InitDeviceObjects(pDataHeader ) ;
		}		
	}

	// 2007-02-12 by bhsohn Item ���� ���� ó��
	m_pMultiItemSelImage = new CINFImageEx;
	pDataHeader = FindResource("selicon");
	m_pMultiItemSelImage->InitDeviceObjects(pDataHeader) ;	
	// end 2007-02-12 by bhsohn Item ���� ���� ó��

	//////////////////////////////////////////////////////////////////////////
    if( FAILED( g_pD3dDev->CreateVertexBuffer( 4*sizeof(MIRRORVERTEX),
                                                  D3DUSAGE_WRITEONLY,
                                                  MIRRORVERTEX::FVF,
                                                  D3DPOOL_MANAGED, &m_pMirrorVB, NULL ) ) )
        return E_FAIL;

    // Initialize the mirror's vertices
    MIRRORVERTEX* v;
    m_pMirrorVB->Lock( 0, 0, (void**)&v, 0 );
    v[0].p = D3DXVECTOR3(0.0f, 0.0f, 1.0f );
    v[1].p = D3DXVECTOR3(1.0f, 0.0f, 1.0f );
    v[2].p = D3DXVECTOR3(0.0f, -1.0f, 1.0f );
    v[3].p = D3DXVECTOR3(1.0f, -1.0f, 1.0f );    
	v[0].uv.x = v[0].uv.y = v[1].uv.y = v[2].uv.x = 0;
	v[1].uv.x = v[2].uv.y = v[3].uv.y = v[3].uv.x = 1;
//	v[0].color = v[1].color = v[2].color = v[3].color = 0x96FF00D8;
	m_pMirrorVB->Unlock();

	// 2006-04-04 by ispark, �κ� ��� ����
	D3DXMatrixOrthoLH(&m_pMatInvenBackProj, 
					 (float)g_pD3dApp->GetBackBufferDesc().Width,
					 (float)g_pD3dApp->GetBackBufferDesc().Height, 
					 0.0f, 1.0f);

	// 2008-08-25 by bhsohn �κ���� ����
	//pDataHeader = FindResource("w_w11");
	pDataHeader = FindResource("w_wi11");
	if(!pDataHeader)
	{
		pDataHeader = FindResource("w_w11");
	}

	D3DXIMAGE_INFO SrcInfo;
	if(FAILED(D3DXCreateTextureFromFileInMemoryEx( g_pD3dDev, (LPCVOID)pDataHeader->m_pData, pDataHeader->m_DataSize,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
		0, &SrcInfo, NULL, &m_pMirrorTexture)))
	{
		SAFE_RELEASE(m_pMirrorTexture);
		return E_FAIL;
	}
	
	return S_OK ;
}

HRESULT CINFInven::RestoreDeviceObjects()
{
	FLOG( "CINFInven::RestoreDeviceObjects()" );
	m_pEqip->RestoreDeviceObjects();
	m_pInven->RestoreDeviceObjects();
	m_pItemSl->RestoreDeviceObjects();
	m_pSpi->RestoreDeviceObjects();
	m_pInvenBase->RestoreDeviceObjects();
	m_pInvenRotationBase->RestoreDeviceObjects();
	// 2007-02-12 by bhsohn Item ���� ���� ó��
	m_pMultiItemSelImage->RestoreDeviceObjects();
	// end 2007-02-12 by bhsohn Item ���� ���� ó��

	int i;
	for(i=0;i<INVEN_ROTATION_STATE;i++)
	{
		for(int j=0;j<INVEN_ROTATION_NUM;j++)
		{
			m_pInvenDirection[i][j]->RestoreDeviceObjects();
		}
	}

	m_pScrollLine->RestoreDeviceObjects();
	m_pScrollButton->RestoreDeviceObjects();

	m_pFontSpi->RestoreDeviceObjects();
	m_pFontWeight[0]->RestoreDeviceObjects();
	m_pFontWeight[1]->RestoreDeviceObjects();
	m_pFontWeight[2]->RestoreDeviceObjects();

	for(i=0;i<INVEN_Y_NUMBER;i++)
	{
		m_pFontItemNum[i]->RestoreDeviceObjects();
	}

	m_pWearPositionBlink->RestoreDeviceObjects();
	m_bRestored = TRUE;
	return S_OK ;
}

HRESULT CINFInven::DeleteDeviceObjects()
{
	FLOG( "CINFInven::DeleteDeviceObjects()" );
	m_pEqip->DeleteDeviceObjects();
	util::del(m_pEqip );
	m_pInven->DeleteDeviceObjects();
	util::del(m_pInven );
	m_pInvenBase->DeleteDeviceObjects();
	util::del(m_pInvenBase );
	// 2007-02-12 by bhsohn Item ���� ���� ó��	
	m_pMultiItemSelImage->DeleteDeviceObjects();
	util::del(m_pMultiItemSelImage );
	// end 2007-02-12 by bhsohn Item ���� ���� ó��
	m_pItemSl->DeleteDeviceObjects();
	util::del(m_pItemSl );
	m_pSpi->DeleteDeviceObjects();
	util::del(m_pSpi );
	m_pScrollLine->DeleteDeviceObjects();
	util::del(m_pScrollLine );
	m_pScrollButton->DeleteDeviceObjects();
	util::del(m_pScrollButton );
	m_pFontSpi->DeleteDeviceObjects();
	util::del(m_pFontSpi );
	m_pFontWeight[0]->DeleteDeviceObjects();
	util::del(m_pFontWeight[0] );
	m_pFontWeight[1]->DeleteDeviceObjects();
	util::del(m_pFontWeight[1] );
	m_pFontWeight[2]->DeleteDeviceObjects();
	util::del(m_pFontWeight[2] );

	int i;
	for(i=0;i<INVEN_Y_NUMBER;i++)
	{
		m_pFontItemNum[i]->DeleteDeviceObjects();
		util::del(m_pFontItemNum[i]);
	}

	m_pWearPositionBlink->DeleteDeviceObjects();
	util::del(m_pWearPositionBlink );

	m_pInvenRotationBase->DeleteDeviceObjects();
	util::del(m_pInvenRotationBase );

	for(i=0;i<INVEN_ROTATION_STATE;i++)
	{
		for(int j=0;j<INVEN_ROTATION_NUM;j++)
		{
			m_pInvenDirection[i][j]->DeleteDeviceObjects();
			util::del(m_pInvenDirection[i][j]);
		}
	}

	m_bInvalidated = FALSE;

	SAFE_RELEASE( m_pMirrorVB );				// 2006-04-04 by ispark
	SAFE_RELEASE(m_pMirrorTexture);

	return S_OK ;
}


HRESULT CINFInven::InvalidateDeviceObjects()
{
	FLOG( "CINFInven::InvalidateDeviceObjects()" );
	m_pEqip->InvalidateDeviceObjects();
	m_pInven->InvalidateDeviceObjects();
	m_pItemSl->InvalidateDeviceObjects();
	m_pSpi->InvalidateDeviceObjects();
	m_pInvenBase->InvalidateDeviceObjects();
	// 2007-02-12 by bhsohn Item ���� ���� ó��		
	m_pMultiItemSelImage->InvalidateDeviceObjects();
	// end 2007-02-12 by bhsohn Item ���� ���� ó��

	m_pScrollLine->InvalidateDeviceObjects();
	m_pScrollButton->InvalidateDeviceObjects();

	m_pFontSpi->InvalidateDeviceObjects();
	m_pFontWeight[0]->InvalidateDeviceObjects();
	m_pFontWeight[1]->InvalidateDeviceObjects();
	m_pFontWeight[2]->InvalidateDeviceObjects();
	
	int i;
	for(i=0;i<INVEN_Y_NUMBER;i++)
	{
		m_pFontItemNum[i]->InvalidateDeviceObjects();
	}

	m_pWearPositionBlink->InvalidateDeviceObjects();
	m_pInvenRotationBase->InvalidateDeviceObjects();

	for(i=0;i<INVEN_ROTATION_STATE;i++)
	{
		for(int j=0;j<INVEN_ROTATION_NUM;j++)
		{
			m_pInvenDirection[i][j]->InvalidateDeviceObjects();
		}
	}
	
	m_bRestored = FALSE;
	m_bInvalidated = TRUE;
	return S_OK ;
}


void CINFInven::Tick()
{
	//m_fElapsedTime = g_pD3dApp->GetElapsedTime();
	FLOG( "CINFInven::Tick()" );
	if(m_bTurnLeft)		g_pShuttleChild->MirrorTurnUp();
	if(m_bTurnRight)	g_pShuttleChild->MirrorTurnDown();
	if(m_bTurnUp)		g_pShuttleChild->MirrorTurnLeft();
	if(m_bTurnDown)		g_pShuttleChild->MirrorTurnRight();

	if( m_iWearPosition == POS_INVALID_POSITION)
		return;
	m_tBlinkTime.Tick( g_pD3dApp->GetElapsedTime() );
	
	if( m_tBlinkTime.IsOverEnd() )
		m_tBlinkTime.Start();

	
}
#define WEIGHT_START_X			58//9
#define WEIGHT_START_Y			249//22
#define WEIGHT_INTERVAL			13

void CINFInven::Render()
{
	FLOG( "CINFInven::Render()" );
	int nWindowPosY = g_pGameMain->m_nLeftWindowY;
//	if(((CINFGameMain*)m_pParent)->m_nLeftWindowInfo == LEFT_WINDOW_INVEN)
//	{
//		m_pInven->Move(8, nWindowPosY + 5);
//		m_pInven->Render();
//	}
//	m_pEqip->Move(57, nWindowPosY + 28);
//	m_pEqip->Render();

	// 2006-04-04 by ispark
	RenderInvenBack();
	g_pShuttleChild->RenderMirror();
	
	// ���� �κ��̹��� ��ü
	m_pInvenBase->Move(9, nWindowPosY + 30);
	m_pInvenBase->Render();

	// �����̼� �̹���
	if(m_nButtonState == ROTATION_NONE || m_nRotationState == ROTATION_STATE_N)
	{
		m_pInvenRotationBase->Move(16, nWindowPosY + 207);
		m_pInvenRotationBase->Render();
	}
	else
	{
		m_pInvenDirection[m_nButtonState][m_nRotationState]->Move(16, nWindowPosY + 207);
		m_pInvenDirection[m_nButtonState][m_nRotationState]->Render();
	}
	

	// ��ũ�� �̹���
	if(g_pStoreData->m_mapItemUniqueNumber.size() > INVEN_X_NUMBER)
	{
		m_pScrollLine->Move(INVEN_SCROLL_LINE_START_X, nWindowPosY + INVEN_SCROLL_LINE_START_Y);
		m_pScrollLine->Render();
//		m_pScrollButton->Move(INVEN_SCROLL_BUTTON_START_X, 
//			nWindowPosY + INVEN_SCROLL_BUTTON_START_Y+(SetScrollLineInterval()*m_nInvenCurrentScroll));
		m_pScrollButton->Move(INVEN_SCROLL_BUTTON_START_X, nWindowPosY + INVEN_SCROLL_BUTTON_START_Y + m_nInvenCurrentScrollPosition);
		m_pScrollButton->Render();
	}

		// �κ��丮 ����
	CINFIcon* pIconInfo = ((CINFGameMain*)m_pParent)->m_pIcon;
	int i;
	for(i=0;i<INVEN_Y_NUMBER;i++)
	{
		for(int j=0;j<INVEN_X_NUMBER;j++)
		{
//			m_pItemSl->Move(INVEN_ITEM_SLOT_START_X + INVEN_SLOT_INTERVAL*j, 
//				nWindowPosY + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*i);
//			m_pItemSl->Render();
			if(m_pInvenDisplayInfo[i*INVEN_X_NUMBER+j])
			{
				// 2005-11-28 by ispark, SourceIndex�� ����
				char buf[64];


				// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
				
				//wsprintf(buf, "%08d", m_pInvenDisplayInfo[i*INVEN_X_NUMBER+j]->pItem->ItemInfo->SourceIndex);
				strcpy( buf, m_pInvenDisplayInfo[i*INVEN_X_NUMBER+j]->IconName );

				// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����


				pIconInfo->SetIcon(buf, 
						INVEN_ITEM_SLOT_START_X + INVEN_SLOT_INTERVAL*j + 1,
						nWindowPosY + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*i + 1, 1.0f);
				pIconInfo->Render();
				// ī���ͺ������ �����̱�
				if( IS_COUNTABLE_ITEM(m_pInvenDisplayInfo[i*INVEN_X_NUMBER+j]->pItem->Kind) )
				{
					CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( 
						m_pInvenDisplayInfo[i*INVEN_X_NUMBER+j]->pItem->UniqueNumber );
					if( pItemInfo->CurrentCount > 1 )
					{
						// ������ �����ش�.
						char buf[128];
// 2006-09-08 by ispark, �ѱ��� �ٸ� ���� ���� ����� �ҽ� ����
//#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)
						// 2006-02-02 by ispark, �۷ι��� ���� ��� ó���� �ٸ���
						wsprintf(buf, "%d",pItemInfo->CurrentCount);
						int len = strlen(buf) - 1;			// ����� �Ѱ� �̻� ���´ٴ� ���ǿ� -1�� �ߴ�.
						m_pFontItemNum[i]->DrawText(INVEN_ITEM_SLOT_START_X + INVEN_SLOT_INTERVAL*j + 21 - len*6,	// ���⼭ 6�� ���� ���� �ؽ�Ʈ �����̴�.
							nWindowPosY + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*i - 1, 
							QSLOT_COUNTERBLE_NUMBER,buf, 0L);
//#else
//						// 2006-08-08 by dgwoo ������ ������ ����.
//						wsprintf(buf, "%d",pItemInfo->CurrentCount);
//						int len = strlen(buf) - 1;			// ����� �Ѱ� �̻� ���´ٴ� ���ǿ� -1�� �ߴ�.
//						m_pFontItemNum[i]->DrawText(INVEN_ITEM_SLOT_START_X + INVEN_SLOT_INTERVAL*j + 21 - len*6,	// ���⼭ 6�� ���� ���� �ؽ�Ʈ �����̴�.
//							nWindowPosY + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*i - 1, 
//							QSLOT_COUNTERBLE_NUMBER,buf, 0L);

//						wsprintf(buf, "%4.d",pItemInfo->CurrentCount);
//						m_pFontItemNum[i]->DrawText(INVEN_ITEM_SLOT_START_X + INVEN_SLOT_INTERVAL*j + 1,
//							nWindowPosY + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*i + 1, 
//							QSLOT_COUNTERBLE_NUMBER,buf, 0L);
//#endif
					}
				}
			}
		}
	}

	// 2010-06-15 by shcho&hslee ��ý��� - ����ǰ �ִ� ���� ����.
	// ������ ������ ������ ������
	//for(i=0;i<WEAR_ITEM_NUMBER;i++)
	for(i=0;i<MAX_EQUIP_POS;i++)
	{
		if(m_pWearDisplayInfo[i])// ������ ������ ������ ������
		{
			// 2005-11-28 by ispark, SourceIndex�� ����
			char buf[64];

			// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

			//wsprintf(buf, "%08d", m_pWearDisplayInfo[i]->pItem->ItemInfo->SourceIndex);
			strcpy( buf, m_pWearDisplayInfo[i]->IconName );

			// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

			switch(i)
			{
			case POS_PROW:
				pIconInfo->SetIcon(buf, POS_PROW_X+1,nWindowPosY + POS_PROW_Y+1, 1.0f);
				break;
			case POS_PROWIN:
				pIconInfo->SetIcon(buf, POS_PROWIN_X+1,nWindowPosY + POS_PROWIN_Y+1, 1.0f);
				break;
			case POS_PROWOUT:
				pIconInfo->SetIcon(buf, POS_PROWOUT_X+1,nWindowPosY + POS_PROWOUT_Y+1, 1.0f);
				break;
			case POS_WINGIN:
//				pIconInfo->SetIcon(m_pWearDisplayInfo[i]->IconName, POS_WINGIN_X+1,nWindowPosY + POS_WINGIN_Y+1, 1.0f);
				pIconInfo->SetIcon(buf, POS_WINGIN_X+1,nWindowPosY + POS_WINGIN_Y+1, 1.0f);
				break;
			case POS_WINGOUT:
				pIconInfo->SetIcon(buf, POS_WINGOUT_X+1,nWindowPosY + POS_WINGOUT_Y+1, 1.0f);
				break;
			case POS_CENTER:
				pIconInfo->SetIcon(buf, POS_CENTER_X+1,nWindowPosY + POS_CENTER_Y+1, 1.0f);
				break;
			case POS_REAR:
				pIconInfo->SetIcon(buf, POS_REAR_X+1,nWindowPosY + POS_REAR_Y+1, 1.0f);
				break;
			case POS_ACCESSORY_UNLIMITED:
				pIconInfo->SetIcon(buf, POS_ATTACHMENT_X+1,nWindowPosY + POS_ATTACHMENT_Y+1, 1.0f);
				break;
			case POS_ACCESSORY_TIME_LIMIT:
				pIconInfo->SetIcon(buf, POS_PET_X+1,nWindowPosY + POS_PET_Y+1, 1.0f);
//				pIconInfo->SetIcon(m_pWearDisplayInfo[i]->IconName, POS_PET_X+1,nWindowPosY + POS_PET_Y+1, 1.0f);
				break;
			}
			pIconInfo->Render();
		}
	}

	if( m_iWearPosition != POS_INVALID_POSITION )
	{
		if( m_tBlinkTime.IsUnderMiddle() )
		{
			RenderWearPosition( m_iWearPosition );
		}
	}

//	pIconInfo->SetIcon(INVEN_NOT_USE_SLOT_ICON, POS_WINGIN_X+1,nWindowPosY + POS_WINGIN_Y+1, 1.0f);
//	pIconInfo->Render();
// 2006-03-30 by ispark
//	pIconInfo->SetIcon(INVEN_NOT_USE_SLOT_ICON, POS_PET_X+1,nWindowPosY + POS_PET_Y+1, 1.0f);
//	pIconInfo->Render();

//	m_pSpi->Move(14, nWindowPosY + 237);
//	m_pSpi->Render();

	char temp1[64];
	char temp2[64];
	SIZE size;
	// SPI
	wsprintf( temp1, "%d", m_nItemSpi );
	MakeCurrencySeparator( temp2, temp1, 3, ',' );
	size = m_pFontSpi->GetStringSize(temp2);
	m_pFontSpi->DrawText(INVEN_SPI_START_X-size.cx, nWindowPosY + INVEN_SPI_START_Y, GUI_FONT_COLOR_BM,temp2, 0L);

	// War Point
	wsprintf(temp1,"%d",g_pShuttleChild->m_myShuttleInfo.WarPoint);
	MakeCurrencySeparator(temp2,temp1,3,',');
	size = m_pFontSpi->GetStringSize(temp2);
	m_pFontSpi->DrawText(INVEN_SPI_START_X-size.cx, nWindowPosY + INVEN_WARPOINT_Y, GUI_FONT_COLOR_BM,temp2, 0L);

	char buff[64];
	wsprintf(buff, "(%3d%%)", 
		(int) (g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo) *100.0f ) );
	m_pFontWeight[0]->DrawText(WEIGHT_START_X+10, nWindowPosY+WEIGHT_START_Y-WEIGHT_INTERVAL,GUI_FONT_COLOR_BM, STRMSG_C_INTERFACE_0026, 0 );//"���緮"
	m_pFontWeight[1]->DrawText(WEIGHT_START_X+FONTWEITGHT_X1, nWindowPosY+WEIGHT_START_Y-WEIGHT_INTERVAL,GUI_FONT_COLOR_BM, buff, 0 );
	// 2006-09-18 by dgwoo ETRI ���������� �����̾��϶� ���緮�� �������ش�.
	//wsprintf(buff, "%2d/%d", (int)(g_pStoreData->GetTotalUseInven()), SIZE_MAX_ITEM_GENERAL - 1);	// 2006-06-23 by ispark, -1�� ���� �������� �����ϴ� ���̴�.
	
	// 2009. 11. 3 by jskim ĳ��(�κ�/â�� Ȯ��) ������ �߰� ����
	//wsprintf(buff, "%d/%d", (int)(g_pStoreData->GetTotalUseInven()), CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1) - 1);	// 2006-06-23 by ispark, -1�� ���� �������� �����ϴ� ���̴�.
	// 2009. 12. 17 by jskim ĳ��(�κ�/â�� Ȯ��) �����
// 	CHARACTER* pMainInfo = g_pD3dApp->GetMFSMyShuttleInfo();
// 	wsprintf(buff, "%d/%d", (int)(g_pStoreData->GetTotalUseInven()), CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1, pMainInfo->GetAddedPermanentInventoryCount()) - 1);	// 2006-06-23 by ispark, -1�� ���� �������� �����ϴ� ���̴�.
	wsprintf(buff, "%d/%d", (int)(g_pStoreData->GetTotalUseInven()), CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1, g_pShuttleChild->m_myShuttleInfo.GetAddedPermanentInventoryCount()) - 1);	// 2006-06-23 by ispark, -1�� ���� �������� �����ϴ� ���̴�.
	//end 2009. 12. 17 by jskim ĳ��(�κ�/â�� Ȯ��) �����
	//end 2009. 11. 3 by jskim ĳ��(�κ�/â�� Ȯ��) ������ �߰� ����
	m_pFontWeight[2]->DrawText(WEIGHT_START_X+FONTWEITGHT_X2, nWindowPosY+WEIGHT_START_Y-WEIGHT_INTERVAL,GUI_FONT_COLOR_BM, buff, 0 );	

	// 2006-07-26 by ispark, ���� ������ ������
//	if(m_pSelectIcon)
//	{
//        POINT ptCursor;
//        GetCursorPos( &ptCursor );
//        ScreenToClient( g_pD3dApp->GetHwnd(), &ptCursor );
//		CheckMouseReverse(&ptCursor);		
//
//		m_pSelectIcon->Move(ptCursor.x-m_nRenderMoveIconIntervalWidth, ptCursor.y-m_nRenderMoveIconIntervalHeight);
//		m_pSelectIcon->Render();
//	}
}

void CINFInven::RenderSpi(int x, int y)
{
	m_pSpi->Move(x, y);
	m_pSpi->Render();

	char temp1[64];
	char temp2[64];
	wsprintf( temp1, "%d", m_nItemSpi );
	MakeCurrencySeparator( temp2, temp1, 3, ',' );
	m_pFontSpi->DrawText(x+SPI_START_X, y + SPI_START_Y, GUI_FONT_COLOR,temp2, 0L);
}
void CINFInven::RenderWearPosition( int m_iWearPosition )
{
	int posx,posy;
	int nWindowPosY = g_pGameMain->m_nLeftWindowY;
	int nPosShop = 0;
	if(g_pD3dApp->m_dwGameState == _SHOP)
		nPosShop = CITY_BASE_NPC_BOX_START_X;

	switch(m_iWearPosition)
	{
	case POS_PROW:
		posx = POS_PROW_X+1+nPosShop;
		posy = nWindowPosY + POS_PROW_Y+1;
		break;
	case POS_PROWIN:
		posx = POS_PROWIN_X+1+nPosShop;
		posy = nWindowPosY + POS_PROWIN_Y+1;
		break;
	case POS_PROWOUT:
		posx = POS_PROWOUT_X+1+nPosShop;
		posy = nWindowPosY + POS_PROWOUT_Y+1;
		break;
	case POS_WINGIN:
		posx = POS_WINGIN_X+1+nPosShop;
		posy = nWindowPosY + POS_WINGIN_Y+1;
		break;
	case POS_WINGOUT:
		posx = POS_WINGOUT_X+1+nPosShop;
		posy = nWindowPosY + POS_WINGOUT_Y+1;
		break;
	case POS_CENTER:
		posx = POS_CENTER_X+1+nPosShop;
		posy = nWindowPosY + POS_CENTER_Y+1;
		break;
	case POS_REAR:
		posx = POS_REAR_X+1+nPosShop;
		posy = nWindowPosY + POS_REAR_Y+1;
		break;
	case POS_ACCESSORY_UNLIMITED:
		posx = POS_ATTACHMENT_X+1+nPosShop;
		posy = nWindowPosY + POS_ATTACHMENT_Y+1;
		break;
	case POS_ACCESSORY_TIME_LIMIT:
		// 2006-03-30 by ispark
		posx = POS_PET_X+1+nPosShop;
		posy = nWindowPosY + POS_PET_Y+1;
		break;
	}

	// �κ��������� ����â ���� ����, ������ġ���� ������ �̹���
	m_pWearPositionBlink->Move(posx,posy);

	DWORD dwColor = 0x00FFFFFF;
	DWORD alpha = 255* m_tBlinkTime.GetPositionRate();
	alpha = alpha << 24;
	m_pWearPositionBlink->SetColor( alpha | dwColor );
	m_pWearPositionBlink->Render();
}
void CINFInven::RenderOnCityBase()
{
	FLOG( "CINFInven::RenderOnCityBase()" );
	
	g_pGameMain->m_pInfWindow->m_nLeftWindowInfo = 1;
	g_pGameMain->m_nLeftWindowY = CITY_SHOP_INVEN_START_Y;
	int nWindowPosY = g_pGameMain->m_nLeftWindowY;

	//////////////////////////////////////////////////////////////////////////
	// ������ Ʋ���� - ���� ��� -
	int x = CITY_BASE_NPC_BOX_START_X;
	int y = nWindowPosY;
	int cx = 426;
	int cy = 275;

	g_pGameMain->m_pInfWindow->m_pBoxImage[0]->Move(x,y);
	g_pGameMain->m_pInfWindow->m_pBoxImage[0]->Render();
	g_pGameMain->m_pInfWindow->m_pBoxImage[1]->Move(x+19, y);
	g_pGameMain->m_pInfWindow->m_pBoxImage[1]->SetScale(cx-38, 1);
	g_pGameMain->m_pInfWindow->m_pBoxImage[1]->Render();
	g_pGameMain->m_pInfWindow->m_pBoxImage[2]->Move(x+cx-19,y);
	g_pGameMain->m_pInfWindow->m_pBoxImage[2]->Render();
	
	g_pGameMain->m_pInfWindow->m_pBoxImage[3]->Move(x, y+19);
	g_pGameMain->m_pInfWindow->m_pBoxImage[3]->SetScale(1, cy-38);
	g_pGameMain->m_pInfWindow->m_pBoxImage[3]->Render();
	
	g_pGameMain->m_pInfWindow->m_pBoxImage[5]->Move(x+cx-19, y+19);
	g_pGameMain->m_pInfWindow->m_pBoxImage[5]->SetScale(1, cy-38);
	g_pGameMain->m_pInfWindow->m_pBoxImage[5]->Render();
	
	g_pGameMain->m_pInfWindow->m_pBoxImage[6]->Move(x,y+cy-19);
	g_pGameMain->m_pInfWindow->m_pBoxImage[6]->Render();
	g_pGameMain->m_pInfWindow->m_pBoxImage[7]->Move(x+19,y+cy-19);
	g_pGameMain->m_pInfWindow->m_pBoxImage[7]->SetScale(cx-38, 1);
	g_pGameMain->m_pInfWindow->m_pBoxImage[7]->Render();
	g_pGameMain->m_pInfWindow->m_pBoxImage[8]->Move(x+cx-19,y+cy-19);
	g_pGameMain->m_pInfWindow->m_pBoxImage[8]->Render();

	g_pShuttleChild->SetRenderInven(TRUE);
	//
	//////////////////////////////////////////////////////////////////////////
	// 2006-04-04 by ispark
	RenderInvenBack(TRUE);
	g_pShuttleChild->RenderMirror();

	// ���� �κ��̹��� ��ü
	m_pInvenBase->Move(CITY_BASE_NPC_BOX_START_X+9, nWindowPosY + 30);
	m_pInvenBase->Render();

	// �����̼� �̹���
	if(m_nButtonState == ROTATION_NONE || m_nRotationState == ROTATION_STATE_N)
	{
		m_pInvenRotationBase->Move(CITY_BASE_NPC_BOX_START_X+16, nWindowPosY + 207);
		m_pInvenRotationBase->Render();
	}
	else
	{
		m_pInvenDirection[m_nButtonState][m_nRotationState]->Move(CITY_BASE_NPC_BOX_START_X+16, nWindowPosY + 207);
		m_pInvenDirection[m_nButtonState][m_nRotationState]->Render();
	}
	
	// ��ũ�� �̹���
	if(g_pStoreData->m_mapItemUniqueNumber.size() > INVEN_X_NUMBER)
	{
		m_pScrollLine->Move(INVEN_SCROLL_LINE_START_X+CITY_BASE_NPC_BOX_START_X, nWindowPosY + INVEN_SCROLL_LINE_START_Y);
		m_pScrollLine->Render();
//		m_pScrollButton->Move(INVEN_SCROLL_BUTTON_START_X+CITY_BASE_NPC_BOX_START_X, 
//			nWindowPosY + INVEN_SCROLL_BUTTON_START_Y+(SetScrollLineInterval()*m_nInvenCurrentScroll));
		m_pScrollButton->Move(INVEN_SCROLL_BUTTON_START_X+CITY_BASE_NPC_BOX_START_X, nWindowPosY + INVEN_SCROLL_BUTTON_START_Y + m_nInvenCurrentScrollPosition);
		m_pScrollButton->Render();
	}

	CINFIcon* pIconInfo = ((CINFGameMain*)m_pParent)->m_pIcon;
	int i;
	for(i=0;i<INVEN_Y_NUMBER;i++)
	{
		for(int j=0;j<INVEN_X_NUMBER;j++)
		{
			m_pItemSl->Move(CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X + INVEN_SLOT_INTERVAL*j, 
				CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*i);
			m_pItemSl->Render();
			if(m_pInvenDisplayInfo[i*INVEN_X_NUMBER+j])
			{
				// 2007-02-12 by bhsohn Item ���� ���� ó��
				BOOL bMultiSel = FALSE;
				if(g_pD3dApp->CheckMultItemSel(m_pInvenDisplayInfo[i*INVEN_X_NUMBER+j]->pItem->UniqueNumber))
				{
					// ���� ������ ������ ������
					bMultiSel = TRUE;
				}
				// end 2007-02-12 by bhsohn Item ���� ���� ó��

				// 2005-11-28 by ispark, SourceIndex�� ����
				char buf[64];


				// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

				//wsprintf(buf, "%08d", m_pInvenDisplayInfo[i*INVEN_X_NUMBER+j]->pItem->ItemInfo->SourceIndex);
				strcpy( buf, m_pInvenDisplayInfo[i*INVEN_X_NUMBER+j]->IconName );

				// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����



				pIconInfo->SetIcon(buf, 
						CITY_BASE_NPC_BOX_START_X + INVEN_ITEM_SLOT_START_X + INVEN_SLOT_INTERVAL*j + 1,
						CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*i + 1, 1.0f);
				pIconInfo->Render();
				
				if( IS_COUNTABLE_ITEM(m_pInvenDisplayInfo[i*INVEN_X_NUMBER+j]->pItem->Kind) )
				{
					CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( 
						m_pInvenDisplayInfo[i*INVEN_X_NUMBER+j]->pItem->UniqueNumber );
					if( pItemInfo->CurrentCount > 1 )
					{
						// ������ �����ش�.
						char buf[128];
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn �±� ���� �߰�
						wsprintf(buf, "%d",pItemInfo->CurrentCount);
						int len = strlen(buf) - 1;			// ����� �Ѱ� �̻� ���´ٴ� ���ǿ� -1�� �ߴ�.
						m_pFontItemNum[i]->DrawText(CITY_BASE_NPC_BOX_START_X + INVEN_ITEM_SLOT_START_X + INVEN_SLOT_INTERVAL*j + 21 - len*6,	// ���⼭ 6�� ���� ���� �ؽ�Ʈ �����̴�.
							CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*i - 1, 
							QSLOT_COUNTERBLE_NUMBER,buf, 0L);
#else
						wsprintf(buf, "%d",pItemInfo->CurrentCount);
						int len = strlen(buf) - 1;			// ����� �Ѱ� �̻� ���´ٴ� ���ǿ� -1�� �ߴ�.
						m_pFontItemNum[i]->DrawText(CITY_BASE_NPC_BOX_START_X + INVEN_ITEM_SLOT_START_X + INVEN_SLOT_INTERVAL*j + 21 - len*6,	// ���⼭ 6�� ���� ���� �ؽ�Ʈ �����̴�.
							CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*i - 1, 
							QSLOT_COUNTERBLE_NUMBER,buf, 0L);
//						wsprintf(buf, "%4.d",pItemInfo->CurrentCount);
//						m_pFontItemNum[i]->DrawText(CITY_BASE_NPC_BOX_START_X + INVEN_ITEM_SLOT_START_X + INVEN_SLOT_INTERVAL*j + 2,
//							CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*i + 2, 
//							QSLOT_COUNTERBLE_NUMBER,buf, 0L);
#endif
					}
				}
				// 2007-02-12 by bhsohn Item ���� ���� ó��				
				if(bMultiSel)
				{
					float fXPos, fYPos;
					fXPos = CITY_BASE_NPC_BOX_START_X + INVEN_ITEM_SLOT_START_X + INVEN_SLOT_INTERVAL*j + 1;
					fYPos = CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*i + 1;						
					m_pMultiItemSelImage->Move(fXPos, fYPos);
					m_pMultiItemSelImage->Render();
					
				}
				// end 2007-02-12 by bhsohn Item ���� ���� ó��				
			}
		}
	}

	// 2010-06-15 by shcho&hslee ��ý��� - ����ǰ �ִ� ���� ����.
	// ������ ������ ������ ������
	//for(i=0;i<WEAR_ITEM_NUMBER;i++)
	for(i=0;i<<MAX_EQUIP_POS;i++)
	{
		if(m_pWearDisplayInfo[i])// ������ ������ ������ ������
		{
			// 2005-11-28 by ispark, SourceIndex�� ����
			char buf[64];


			// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

			//wsprintf(buf, "%08d", m_pWearDisplayInfo[i]->pItem->ItemInfo->SourceIndex);
			strcpy( buf, m_pWearDisplayInfo[i]->IconName );

			// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����


			switch(i)
			{
			case POS_PROW:
				pIconInfo->SetIcon(buf, CITY_BASE_NPC_BOX_START_X+POS_PROW_X+1,CITY_SHOP_INVEN_START_Y + POS_PROW_Y+1, 1.0f);
				break;
			case POS_PROWIN:
				pIconInfo->SetIcon(buf, CITY_BASE_NPC_BOX_START_X+POS_PROWIN_X+1,CITY_SHOP_INVEN_START_Y + POS_PROWIN_Y+1, 1.0f);
				break;
			case POS_PROWOUT:
				pIconInfo->SetIcon(buf, CITY_BASE_NPC_BOX_START_X+POS_PROWOUT_X+1,CITY_SHOP_INVEN_START_Y + POS_PROWOUT_Y+1, 1.0f);
				break;
			case POS_WINGIN:
//				pIconInfo->SetIcon(m_pWearDisplayInfo[i]->IconName, CITY_BASE_NPC_BOX_START_X+POS_WINGIN_X+1,CITY_SHOP_INVEN_START_Y + POS_WINGIN_Y+1, 1.0f);
				pIconInfo->SetIcon(buf, CITY_BASE_NPC_BOX_START_X+POS_WINGIN_X+1,CITY_SHOP_INVEN_START_Y + POS_WINGIN_Y+1, 1.0f);
				break;
			case POS_WINGOUT:
				pIconInfo->SetIcon(buf, CITY_BASE_NPC_BOX_START_X+POS_WINGOUT_X+1,CITY_SHOP_INVEN_START_Y + POS_WINGOUT_Y+1, 1.0f);
				break;
			case POS_CENTER:
				pIconInfo->SetIcon(buf, CITY_BASE_NPC_BOX_START_X+POS_CENTER_X+1,CITY_SHOP_INVEN_START_Y + POS_CENTER_Y+1, 1.0f);
				break;
			case POS_REAR:
				pIconInfo->SetIcon(buf, CITY_BASE_NPC_BOX_START_X+POS_REAR_X+1,CITY_SHOP_INVEN_START_Y + POS_REAR_Y+1, 1.0f);
				break;
			case POS_ACCESSORY_UNLIMITED:
				pIconInfo->SetIcon(buf, CITY_BASE_NPC_BOX_START_X+POS_ATTACHMENT_X+1,CITY_SHOP_INVEN_START_Y + POS_ATTACHMENT_Y+1, 1.0f);
				break;
			case POS_ACCESSORY_TIME_LIMIT:
				pIconInfo->SetIcon(buf, CITY_BASE_NPC_BOX_START_X+POS_PET_X+1,CITY_SHOP_INVEN_START_Y + POS_PET_Y+1, 1.0f);
//				pIconInfo->SetIcon(m_pWearDisplayInfo[i]->IconName, CITY_BASE_NPC_BOX_START_X+POS_PET_X+1,CITY_SHOP_INVEN_START_Y + POS_PET_Y+1, 1.0f);
				break;
			}
			pIconInfo->Render();
		}
	}

	if( m_iWearPosition != POS_INVALID_POSITION )
	{
		if( m_tBlinkTime.IsUnderMiddle() )
		{
			RenderWearPosition( m_iWearPosition );
		}
	}

	char temp1[64];
	char temp2[64];
	SIZE size;
	// SPI
	wsprintf( temp1, "%d", m_nItemSpi );
	MakeCurrencySeparator( temp2, temp1, 3, ',' );
	size = m_pFontSpi->GetStringSize(temp2);
	m_pFontSpi->DrawText(INVEN_SPI_START_X+CITY_BASE_NPC_BOX_START_X-size.cx, nWindowPosY + INVEN_SPI_START_Y, GUI_FONT_COLOR_BM,temp2, 0L);

	// War Point
	wsprintf(temp1,"%d",g_pShuttleChild->m_myShuttleInfo.WarPoint);
	MakeCurrencySeparator(temp2,temp1,3,',');
	size = m_pFontSpi->GetStringSize(temp2);
	m_pFontSpi->DrawText(INVEN_SPI_START_X+CITY_BASE_NPC_BOX_START_X-size.cx, nWindowPosY + INVEN_WARPOINT_Y, GUI_FONT_COLOR_BM,temp2, 0L);

	char buff[64];
	wsprintf(buff, "(%3d%%)", 
		(int) (g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo) *100.0f ) );
	m_pFontWeight[0]->DrawText(WEIGHT_START_X+CITY_BASE_NPC_BOX_START_X+10, nWindowPosY+WEIGHT_START_Y-WEIGHT_INTERVAL,GUI_FONT_COLOR_BM, STRMSG_C_INTERFACE_0026, 0 );//"���緮"
	m_pFontWeight[1]->DrawText(WEIGHT_START_X+CITY_BASE_NPC_BOX_START_X+FONTWEITGHT_X1, nWindowPosY+WEIGHT_START_Y-WEIGHT_INTERVAL,GUI_FONT_COLOR_BM, buff, 0 );
	
	// ���緮
	// 2009. 11. 3 by jskim ĳ��(�κ�/â�� Ȯ��) ������ �߰� ����
	//wsprintf(buff, "%d/%d", (int)(g_pStoreData->GetTotalUseInven()), CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1) - 1);	// 2006-06-23 by ispark, -1�� ���� �������� �����ϴ� ���̴�.
	// 2009. 12. 17 by jskim ĳ��(�κ�/â�� Ȯ��) ����� 
// 	CHARACTER* pMainInfo = g_pD3dApp->GetMFSMyShuttleInfo();
// 	wsprintf(buff, "%d/%d", (int)(g_pStoreData->GetTotalUseInven()), CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1, pMainInfo->GetAddedPermanentInventoryCount()) - 1);	// 2006-06-23 by ispark, -1�� ���� �������� �����ϴ� ���̴�.
	wsprintf(buff, "%d/%d", (int)(g_pStoreData->GetTotalUseInven()), CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1, g_pShuttleChild->m_myShuttleInfo.GetAddedPermanentInventoryCount()) - 1);	// 2006-06-23 by ispark, -1�� ���� �������� �����ϴ� ���̴�.
	//end 2009. 12. 17 by jskim ĳ��(�κ�/â�� Ȯ��) ����� 
	//end 2009. 11. 3 by jskim ĳ��(�κ�/â�� Ȯ��) ������ �߰� ����
	m_pFontWeight[2]->DrawText(WEIGHT_START_X+CITY_BASE_NPC_BOX_START_X+FONTWEITGHT_X2, nWindowPosY+WEIGHT_START_Y-WEIGHT_INTERVAL,GUI_FONT_COLOR_BM, buff, 0 );
}

void CINFInven::SetAllIconInfo()
{
	FLOG( "CINFInven::SetAllIconInfo()" );
	InitInvenIconInfo();
	InitWearIconInfo();
	CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.begin();
	while(it != g_pStoreData->m_mapItemWindowPosition.end())
	{
		CItemInfo* pItemInfo = it->second;
//		int nMinIndex = GetScrollLineNumber(m_nInvenCurrentScroll)*INVEN_X_NUMBER;
//		int nMaxIndex = (GetScrollLineNumber(m_nInvenCurrentScroll)*INVEN_X_NUMBER)+INVEN_X_NUMBER*INVEN_Y_NUMBER;
		int nMinIndex = m_nInvenCurrentScroll*INVEN_X_NUMBER;
		int nMaxIndex = (m_nInvenCurrentScroll*INVEN_X_NUMBER)+(INVEN_X_NUMBER*INVEN_Y_NUMBER);
		if(pItemInfo->Wear>WEAR_NOT_ATTACHED && pItemInfo->ItemWindowIndex < POS_ITEMWINDOW_OFFSET)
		{
			SetSingleWearIconInfo(pItemInfo);
		}
//		else if( pItemInfo->ItemWindowIndex >= POS_ITEMWINDOW_OFFSET+m_nInvenCurrentScroll*INVEN_X_NUMBER && 
//			pItemInfo->ItemWindowIndex < POS_ITEMWINDOW_OFFSET+m_nInvenCurrentScroll*INVEN_X_NUMBER+INVEN_X_NUMBER*INVEN_Y_NUMBER)
		else if( pItemInfo->ItemWindowIndex >= POS_ITEMWINDOW_OFFSET+nMinIndex &&
			pItemInfo->ItemWindowIndex < POS_ITEMWINDOW_OFFSET+nMaxIndex)
		{
			if(IS_COUNTABLE_ITEM(pItemInfo->Kind))
			{
				if(pItemInfo->CurrentCount>0)
					SetSingleInvenIconInfo(pItemInfo);
#ifdef _DEBUG
				else
				{
					DBGOUT("CINFInven::SetAllIconInfo(), Do not rendering : item[%d] count == 0 \n", pItemInfo->ItemNum);
				}
#endif
			}
			else
			{
				SetSingleInvenIconInfo(pItemInfo); 
//				if(!IsTradingItem(pItemInfo))
//				{
//					SetSingleInvenIconInfo(pItemInfo); 
//				}
			}
			if(pItemInfo->CurrentCount<0)
			{
				char buf[256];
				wsprintf(buf,"DB ERROR : item(%d) count = %d", pItemInfo->ItemNum, pItemInfo->CurrentCount);
				g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
			}
		}
		it++;
	}
//	map<int, ITEM_GENERAL*>::iterator it = g_pStoreData->m_mapItemInInventory.begin();
//	while(it != g_pStoreData->m_mapItemInInventory.end())
//	{
//		ITEM_GENERAL* pItem = it->second;
//		if(pItem->Wear>WEAR_NOT_ATTACHED && pItem->ItemWindowIndex < POS_ITEMWINDOW_OFFSET)
//		{
//			SetSingleWearIconInfo(pItem);
//		}
///		else if( pItem->ItemWindowIndex >= POS_ITEMWINDOW_OFFSET+m_nInvenCurrentScroll*INVEN_X_NUMBER && 
//			pItem->ItemWindowIndex < POS_ITEMWINDOW_OFFSET+m_nInvenCurrentScroll*INVEN_X_NUMBER+INVEN_X_NUMBER*INVEN_Y_NUMBER)
//		{
//			if(IS_COUNTABLE_ITEM(pItem->Kind))
//			{
//				if(pItem->CurrentCount>0)
//					SetSingleInvenIconInfo(pItem);
//#ifdef _DEBUG
//				else
//				{
//					DBGOUT("CINFInven::SetAllIconInfo(), Do not rendering : item[%d] count == 0 \n", pItem->ItemNum);
//				}
//#endif
//			}
//			else
//			{
//				if(!IsTradingItem(pItem)) {
//					SetSingleInvenIconInfo(pItem); }
//			}
//			if(pItem->CurrentCount<0)
//			{
//				char buf[256];
//				wsprintf(buf,"DB ERROR : item(%d) count = %d", pItem->ItemNum, pItem->CurrentCount);
//				g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
//			}
//		}
//		it++;
//	}
}

BOOL CINFInven::IsTradingItem(ITEM_GENERAL* pItem)
{
	FLOG( "CINFInven::IsTradingItem(ITEM_GENERAL* pItem)" );
	if( ((CINFGameMain*)m_pParent)->m_nLeftWindowInfo == LEFT_WINDOW_TRANS)
	{
		vector<CItemInfo*>::iterator it = ((CINFGameMain*)m_pParent)->m_pTrade->m_vecMyStore.begin();
		while(it != ((CINFGameMain*)m_pParent)->m_pTrade->m_vecMyStore.end())
		{
			if((*it)->UniqueNumber == pItem->UniqueNumber)
			{
				return TRUE;
			}
			it++;
		}

	}
	return FALSE;
}

void CINFInven::InitInvenIconInfo()
{
	FLOG( "CINFInven::InitInvenIconInfo()" );
	for(int i=0;i<INVEN_NUMBER;i++)
	{
		util::del(m_pInvenDisplayInfo[i]);
	}
}

void CINFInven::InitWearIconInfo()
{
	FLOG( "CINFInven::InitWearIconInfo()" );

	// 2010-06-15 by shcho&hslee ��ý��� - ����ǰ �ִ� ���� ����.
	//for(int i=0;i<WEAR_ITEM_NUMBER;i++)
	for(int i=0;i<MAX_EQUIP_POS;i++)
	{
		util::del(m_pWearDisplayInfo[i]);
	}
}

//void CINFInven::SetSingleInvenIconInfo(ITEM_GENERAL* pItem)
//{
//	FLOG( "CINFInven::SetSingleInvenIconInfo(ITEM_GENERAL* pItem)" );
//	char buf[20];
//	wsprintf(buf, "%08d",pItem->ItemNum);
//	int index = (pItem->ItemWindowIndex-POS_ITEMWINDOW_OFFSET) - m_nInvenCurrentScroll*INVEN_X_NUMBER;// POS_ITEMWINDOW_OFFSET�� WindowIndex�� 100���ͽ������� ��Ÿ��
//	if(index >= 0 && index < INVEN_NUMBER)
//	{
//		if(m_pInvenDisplayInfo[index])
//		{
//			DBGOUT("ERROR CINFInven::SetSingleInvenIconInfo(ITEM_GENERAL* pItem) : [index:%d]�� �������� �ʾҴ�. \n", index);
//		}
//		util::del(m_pInvenDisplayInfo[index]);
//		m_pInvenDisplayInfo[index] = new INVEN_DISPLAY_INFO;
//		memset(m_pInvenDisplayInfo[index], 0x00, sizeof(INVEN_DISPLAY_INFO));
//		strcpy(m_pInvenDisplayInfo[index]->IconName, buf);
//		ITEM *itemInfo = g_pDatabase->GetServerItemInfo(pItem->ItemNum);
//		if(itemInfo)
//			strcpy(m_pInvenDisplayInfo[index]->Name, itemInfo->ItemName);
//		m_pInvenDisplayInfo[index]->pItem = (ITEM_BASE*)pItem;
//	}
//}

void CINFInven::SetSingleInvenIconInfo(CItemInfo* pItemInfo)
{
	char buf[20];

	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	
	//wsprintf(buf, "%08d",pItemInfo->ItemInfo->SourceIndex);			// 2005-08-22 by ispark
	ITEM* pShapeItem = g_pDatabase->GetServerItemInfo( pItemInfo->GetShapeItemNum() );

	int tempSourceIndex = NULL;
	if( pShapeItem )
	{
		tempSourceIndex = pShapeItem->SourceIndex;
	}
	else
	{
		tempSourceIndex = pItemInfo->ItemInfo->SourceIndex;
	}

	wsprintf(buf, "%08d", tempSourceIndex );

	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����


//	int index = (pItemInfo->ItemWindowIndex-POS_ITEMWINDOW_OFFSET) - m_nInvenCurrentScroll*INVEN_X_NUMBER;// POS_ITEMWINDOW_OFFSET�� WindowIndex�� 100���ͽ������� ��Ÿ��
	int index = (pItemInfo->ItemWindowIndex-POS_ITEMWINDOW_OFFSET) - (m_nInvenCurrentScroll*INVEN_X_NUMBER);// POS_ITEMWINDOW_OFFSET�� WindowIndex�� 100���ͽ������� ��Ÿ��
	if(index >= 0 && index < INVEN_NUMBER)
	{
		if(m_pInvenDisplayInfo[index])
		{
//			DBGOUT("ERROR CINFInven::SetSingleInvenIconInfo(ITEM_GENERAL* pItem) : [index:%d]�� �������� �ʾҴ�. \n", index);
		}
		util::del(m_pInvenDisplayInfo[index]);
		m_pInvenDisplayInfo[index] = new INVEN_DISPLAY_INFO;
		memset(m_pInvenDisplayInfo[index], 0x00, sizeof(INVEN_DISPLAY_INFO));
		strcpy(m_pInvenDisplayInfo[index]->IconName, buf);
		ITEM *pITEM = pItemInfo->GetItemInfo();
		if(pITEM && strlen(pITEM->ItemName) > 0)
			strcpy(m_pInvenDisplayInfo[index]->Name, pITEM->ItemName);
		m_pInvenDisplayInfo[index]->pItem = (ITEM_BASE*)pItemInfo;
	}
}
//void CINFInven::SetSingleWearIconInfo(ITEM_GENERAL* pItem)
//{
//	FLOG( "CINFInven::SetSingleWearIconInfo(ITEM_GENERAL* pItem)" );
//	char buf[20];
//	wsprintf(buf, "%08d",pItem->ItemNum);
//	if(pItem->Wear>WEAR_NOT_ATTACHED && pItem->ItemWindowIndex<POS_ITEMWINDOW_OFFSET)
//	{
//		util::del(m_pWearDisplayInfo[pItem->ItemWindowIndex]);
//		m_pWearDisplayInfo[pItem->ItemWindowIndex] = new INVEN_DISPLAY_INFO;
//		memset(m_pWearDisplayInfo[pItem->ItemWindowIndex], 0x00, sizeof(INVEN_DISPLAY_INFO));
//		strcpy(m_pWearDisplayInfo[pItem->ItemWindowIndex]->IconName, buf);
//		ITEM *itemInfo = g_pDatabase->GetServerItemInfo(pItem->ItemNum);
//		if(itemInfo)
//			strcpy(m_pWearDisplayInfo[pItem->ItemWindowIndex]->Name, itemInfo->ItemName);
//		m_pWearDisplayInfo[pItem->ItemWindowIndex]->pItem = pItem;
//	}
//}

void CINFInven::SetSingleWearIconInfo(CItemInfo* pItemInfo)
{
	char buf[20];

	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

	//wsprintf(buf, "%08d",pItemInfo->ItemInfo->SourceIndex);			// 2005-08-22 by ispark
	ITEM* pShapeItem = g_pDatabase->GetServerItemInfo( pItemInfo->GetShapeItemNum() );

	int tempSourceInfo = NULL;
	if( pShapeItem )
	{
		tempSourceInfo = pShapeItem->SourceIndex;
	}
	else
	{
		tempSourceInfo = pItemInfo->ItemInfo->SourceIndex;
	}
	
	wsprintf(buf, "%08d", tempSourceInfo );

	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����


	if(pItemInfo->Wear>WEAR_NOT_ATTACHED && pItemInfo->ItemWindowIndex<POS_ITEMWINDOW_OFFSET)
	{
		util::del(m_pWearDisplayInfo[pItemInfo->ItemWindowIndex]);
		m_pWearDisplayInfo[pItemInfo->ItemWindowIndex] = new INVEN_DISPLAY_INFO;
		memset(m_pWearDisplayInfo[pItemInfo->ItemWindowIndex], 0x00, sizeof(INVEN_DISPLAY_INFO));
		strcpy(m_pWearDisplayInfo[pItemInfo->ItemWindowIndex]->IconName, buf);
		ITEM *pITEM = pItemInfo->GetItemInfo();
		if(pITEM)
			strcpy(m_pWearDisplayInfo[pItemInfo->ItemWindowIndex]->Name, pITEM->ItemName);
		m_pWearDisplayInfo[pItemInfo->ItemWindowIndex]->pItem = (ITEM_BASE*)pItemInfo;
	}
}

void CINFInven::SetItemInfo(INVEN_DISPLAY_INFO *pItemInfo, int x, int y)
{
	FLOG( "CINFInven::SetItemInfo(INVEN_DISPLAY_INFO *pItemInfo, int x, int y)" );
	if(m_pCurrentItemInfo != pItemInfo) 
	{
		if(pItemInfo) 
		{
			CItemInfo* pItem = g_pStoreData->FindItemInInventoryByUniqueNumber(pItemInfo->pItem->UniqueNumber);
			((CINFGameMain*)m_pParent)->SetItemInfo(pItemInfo->pItem->UniqueNumber, pItemInfo->pItem->ItemNum, x, y, 0); 
		}
		else 
		{
			((CINFGameMain*)m_pParent)->SetItemInfo( 0, 0, 0, 0); 
		}

		m_pCurrentItemInfo = pItemInfo;
	}
}

int CINFInven::WndProcOnCityBase(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFInven::WndProcOnCityBase(UINT uMsg, WPARAM wParam, LPARAM lParam)" );

	// �κ��丮 ĳ���� �����̼�
	ProcessRotationUnitWnd(uMsg, wParam, lParam);

	switch(uMsg)
	{
	case WM_RBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			// 2007-02-12 by bhsohn Item ���� ���� ó��
			int nSelItemCnt = g_pD3dApp->GetMultiSelectItem();
			// end 2007-02-12 by bhsohn Item ���� ���� ó��
			
			int i = (pt.x-(CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X))/INVEN_SLOT_INTERVAL;
			if( pt.x > CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X &&
				i >= 0 && i < INVEN_X_NUMBER &&
				pt.x < CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X+INVEN_SLOT_INTERVAL*i+INVEN_SLOT_SIZE)
			{
				int j = (pt.y - CITY_SHOP_INVEN_START_Y - INVEN_ITEM_SLOT_START_Y)/INVEN_SLOT_INTERVAL;
				if( j >= 0 && 
					j < INVEN_Y_NUMBER &&
					m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i] && pt.y > CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y &&
					pt.y < CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*j + INVEN_SLOT_SIZE)
				{
					if(m_bEnchantState && m_pEnchantItem)
					{
						m_bEnchantState = FALSE;
						((CINFGameMain*)m_pParent)->m_pInfWindow->DeleteMsgBox(_Q_USE_ENCHANT_ITEM_CARD);
					}
					else
					{
						// 2007-02-12 by bhsohn Item ���� ���� ó��
						//SetSelectItem(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]);						
						if(nSelItemCnt <= 0 )
						{
							SetMultiSelectItem(NULL);	// ���� ó�� ����							
							// �̱� ����
						SetSelectItem(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]);
						}
						// end 2007-02-12 by bhsohn Item ���� ���� ó��
					}
				}
			}
			if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
				((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_INVEN_POS &&
				g_pInterface->m_pBazaarShop == NULL)
			{
				if(IS_ITEM_SHOP_TYPE(g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind))
				{
					CItemInfo* pItemInfo = (CItemInfo*)((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem;
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
				int nPosX = (pt.x-(CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X))/INVEN_SLOT_INTERVAL;
				if( (pt.x > CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X 
					&&nPosX >= 0 
					&& nPosX < INVEN_X_NUMBER) )
				{
					int nPosY = (pt.y - CITY_SHOP_INVEN_START_Y - INVEN_ITEM_SLOT_START_Y)/INVEN_SLOT_INTERVAL;
					BOOL bPopup = FALSE;
					if( nPosY >= 0 
						&& nPosY < INVEN_Y_NUMBER 
						&& m_pInvenDisplayInfo[nPosY*INVEN_X_NUMBER+nPosX] 
						&& pt.y > CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y 
						&&pt.y < CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*nPosY + INVEN_SLOT_SIZE)
					{						
						if(g_pD3dApp->CheckMultItemSel(m_pInvenDisplayInfo[nPosY*INVEN_X_NUMBER+nPosX]->pItem->UniqueNumber))
						{
							bPopup = TRUE;
							// ���� ������ ������ ������
				// ���� ������ ���� â �˾� 
				((CINFGameMain*)m_pParent)->PopupMultiItemSelect();				
			}
					}	
					if(!bPopup)
					{					
						SetMultiSelectItem(NULL);	// ���� ó�� ����							
					}
				}
				else
				{
					SetMultiSelectItem(NULL);	// ���� ó�� ����
				}
				
			}
			// end 2007-02-12 by bhsohn Item ���� ���� ó��
			SetSelectItem(NULL);
			SetItemInfo(NULL, 0, 0);
		}
		break;
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);

/*			int nWindowPosY = POS_LEFT_WINDOW_Y;
			if(nWindowPosY < SIZE_ROLL_Y )
				nWindowPosY = SIZE_ROLL_Y;
			if(nWindowPosY > LEFT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y)
				nWindowPosY = LEFT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y;
*/			if( pt.x > CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X &&
				pt.x < CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X+INVEN_SLOT_INTERVAL*INVEN_X_NUMBER &&
				pt.y > CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y &&
				pt.y < CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*INVEN_Y_NUMBER)
			{
				// 2005-12-10 by ispark, �⺻ 6���� �Ѿ�� ������
				if(GetScrollLine() > 0) 
				{
					if((int)wParam<0)
					{
						m_nInvenCurrentScroll++;
						if(m_nInvenCurrentScroll > GetScrollLine())
						{
							m_nInvenCurrentScroll = GetScrollLine();
							m_nInvenCurrentScrollPosition = INVEN_SCROLL_LINE_MOVE_LENGTH;
						}
						else
						{
							m_nInvenCurrentScrollPosition = GetScrollLineInterval(m_nInvenCurrentScroll);
						}

						SetAllIconInfo();
						SetItemInfo(NULL, 0, 0);
					}
					else
					{
						m_nInvenCurrentScroll--;
						if(m_nInvenCurrentScroll < 0)
						{
							m_nInvenCurrentScroll = 0;
							m_nInvenCurrentScrollPosition = 0;
						}
						else
						{
							m_nInvenCurrentScrollPosition = GetScrollLineInterval(m_nInvenCurrentScroll);
						}

						SetAllIconInfo();
						SetItemInfo(NULL, 0, 0);
					}
				}
				return INF_MSGPROC_BREAK;
			}
			break;
		}
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			((CINFGameMain*)m_pParent)->SetToolTip(NULL, 0, 0);	
			
			int nWindowPosY = g_pGameMain->m_nLeftWindowY;

			if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem && 
				((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem)
			{
				m_bSelectItem = TRUE;
			}

			if(m_bScrollLock && GetScrollLine() > 0) 
			{
				// 2005-12-08 by ispark, ��ũ�ѹ� ����
				if(pt.x >= INVEN_SCROLL_LINE_START_X+CITY_BASE_NPC_BOX_START_X - 20 && 
					pt.x <= INVEN_SCROLL_LINE_START_X+CITY_BASE_NPC_BOX_START_X + 20)
				{
					if(pt.y >= nWindowPosY + INVEN_SCROLL_LINE_START_Y &&
						pt.y <= nWindowPosY + INVEN_SCROLL_LINE_START_Y + INVEN_SCROLL_LINE_LENGTH)
					{
						int nMove = pt.y - m_ptMouse.y;
						m_nInvenCurrentScrollPosition += nMove;
						m_nInvenCurrentScroll = GetScrollLineNumber(m_nInvenCurrentScrollPosition);
						if(m_nInvenCurrentScrollPosition < 0)
						{
							m_nInvenCurrentScroll = 0;
							m_nInvenCurrentScrollPosition = 0;
						}
						else if(m_nInvenCurrentScrollPosition > INVEN_SCROLL_LINE_MOVE_LENGTH)
						{
							m_nInvenCurrentScroll = GetScrollLine();
							m_nInvenCurrentScrollPosition = INVEN_SCROLL_LINE_MOVE_LENGTH;
						}
	
						if(nMove != 0)
						{
							m_ptMouse = pt;
							SetAllIconInfo();
						}
					}
					else if(pt.y < nWindowPosY + INVEN_SCROLL_LINE_START_Y)
					{
						m_nInvenCurrentScroll = 0;
						m_nInvenCurrentScrollPosition = 0;
						SetAllIconInfo();
					}
					else if(pt.y > nWindowPosY + INVEN_SCROLL_LINE_START_Y + INVEN_SCROLL_LINE_LENGTH)
					{
						m_nInvenCurrentScroll = GetScrollLine();
						m_nInvenCurrentScrollPosition = INVEN_SCROLL_LINE_MOVE_LENGTH;
						SetAllIconInfo();
					}
				}
			}
			if(!m_bSelectItem)
			{
				SetItemInfo(NULL, 0, 0);

				if(	pt.x>CITY_BASE_NPC_BOX_START_X+POS_PROW_X && 
					pt.x<CITY_BASE_NPC_BOX_START_X+POS_PROW_X+INVEN_SLOT_SIZE &&
					pt.y>CITY_SHOP_INVEN_START_Y+POS_PROW_Y && 
					pt.y<CITY_SHOP_INVEN_START_Y+POS_PROW_Y+INVEN_SLOT_SIZE)
				{
					// ���̴� ����
					if(m_pWearDisplayInfo[POS_PROW])
						SetItemInfo(m_pWearDisplayInfo[POS_PROW], pt.x, pt.y);
					else
						((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, STRMSG_C_TOOLTIP_0015);//"���̴� ��� �����ϴ� ��ġ�Դϴ�."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_PROWIN_X && 
						 pt.x<CITY_BASE_NPC_BOX_START_X+POS_PROWIN_X+INVEN_SLOT_SIZE &&
						 pt.y>CITY_SHOP_INVEN_START_Y+POS_PROWIN_Y && 
						 pt.y<CITY_SHOP_INVEN_START_Y+POS_PROWIN_Y+INVEN_SLOT_SIZE)
				{
					if(m_pWearDisplayInfo[POS_PROWIN])
						SetItemInfo(m_pWearDisplayInfo[POS_PROWIN], pt.x, pt.y);
					else
						((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, STRMSG_C_TOOLTIP_0016);//"��ǻ�� �������� �����ϴ� ��ġ�Դϴ�."						
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_PROWOUT_X && 
						 pt.x<CITY_BASE_NPC_BOX_START_X+POS_PROWOUT_X+INVEN_SLOT_SIZE &&
						 pt.y>CITY_SHOP_INVEN_START_Y+POS_PROWOUT_Y && 
						 pt.y<CITY_SHOP_INVEN_START_Y+POS_PROWOUT_Y+INVEN_SLOT_SIZE)
				{
					// 1�� ���� ����
					if(m_pWearDisplayInfo[POS_PROWOUT])
						SetItemInfo(m_pWearDisplayInfo[POS_PROWOUT], pt.x, pt.y);
					else
						((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, STRMSG_C_TOOLTIP_0017);//"�⺻���⸦ �����ϴ� ��ġ�Դϴ�."
					return INF_MSGPROC_BREAK;
				}
				// 2006-07-20 by dgwoo ��ü ����� ���� ���� 
				else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_WINGIN_X && 
						 pt.x<CITY_BASE_NPC_BOX_START_X+POS_WINGIN_X+INVEN_SLOT_SIZE &&
						 pt.y>CITY_SHOP_INVEN_START_Y+POS_WINGIN_Y && 
						 pt.y<CITY_SHOP_INVEN_START_Y+POS_WINGIN_Y+INVEN_SLOT_SIZE)
				{
					if(m_pWearDisplayInfo[POS_WINGIN])
						SetItemInfo(m_pWearDisplayInfo[POS_WINGIN], pt.x, pt.y);
					else
						((CINFGameMain*)m_pParent)->SetToolTip(pt.x,pt.y,STRMSG_C_060720_0100);
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_WINGOUT_X &&
						 pt.x<CITY_BASE_NPC_BOX_START_X+POS_WINGOUT_X+INVEN_SLOT_SIZE &&
						 pt.y>CITY_SHOP_INVEN_START_Y+POS_WINGOUT_Y && 
						 pt.y<CITY_SHOP_INVEN_START_Y+POS_WINGOUT_Y+INVEN_SLOT_SIZE)
				{
					// 2�� ���� ����
					if(m_pWearDisplayInfo[POS_WINGOUT])
						SetItemInfo(m_pWearDisplayInfo[POS_WINGOUT], pt.x, pt.y);
					else
						((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, STRMSG_C_TOOLTIP_0018);//"��޹��⸦ �����ϴ� ��ġ�Դϴ�."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_CENTER_X && 
						 pt.x<CITY_BASE_NPC_BOX_START_X+POS_CENTER_X+INVEN_SLOT_SIZE &&
						 pt.y>CITY_SHOP_INVEN_START_Y+POS_CENTER_Y && 
						 pt.y<CITY_SHOP_INVEN_START_Y+POS_CENTER_Y+INVEN_SLOT_SIZE)
				{
					// �Ƹӷ� ������
					if(m_pWearDisplayInfo[POS_CENTER])
						SetItemInfo(m_pWearDisplayInfo[POS_CENTER], pt.x, pt.y);
					else
						((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, STRMSG_C_TOOLTIP_0019);//"�Ƹ� ��� �����ϴ� ��ġ�Դϴ�."
					return INF_MSGPROC_BREAK;
				}
				else if( m_pWearDisplayInfo[POS_REAR] && 
						 pt.x>CITY_BASE_NPC_BOX_START_X+POS_REAR_X && 
						 pt.x<CITY_BASE_NPC_BOX_START_X+POS_REAR_X+INVEN_SLOT_SIZE &&
						 pt.y>CITY_SHOP_INVEN_START_Y+POS_REAR_Y && 
						 pt.y<CITY_SHOP_INVEN_START_Y+POS_REAR_Y+INVEN_SLOT_SIZE)
				{
					//((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, m_pWearDisplayInfo[POS_REAR]->Name);
					SetItemInfo(m_pWearDisplayInfo[POS_REAR], pt.x, pt.y);
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_ATTACHMENT_X && 
						 pt.x<CITY_BASE_NPC_BOX_START_X+POS_ATTACHMENT_X+INVEN_SLOT_SIZE &&
						 pt.y>CITY_SHOP_INVEN_START_Y+POS_ATTACHMENT_Y &&
						 pt.y<CITY_SHOP_INVEN_START_Y+POS_ATTACHMENT_Y+INVEN_SLOT_SIZE)
				{
					// �����̳ʷ� ������					
					if(m_pWearDisplayInfo[POS_ACCESSORY_UNLIMITED])
						SetItemInfo(m_pWearDisplayInfo[POS_ACCESSORY_UNLIMITED], pt.x, pt.y);
					else
						((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, STRMSG_C_TOOLTIP_0020);//"��Ÿ �����迭 ��� �����ϴ� ��ġ�Դϴ�."
					return INF_MSGPROC_BREAK;
				}
				// 2006-03-30 by ispark, �Ǽ��縮
				else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_PET_X && 
						 pt.x<CITY_BASE_NPC_BOX_START_X+POS_PET_X+INVEN_SLOT_SIZE &&
						 pt.y>CITY_SHOP_INVEN_START_Y+POS_PET_Y &&
						 pt.y<CITY_SHOP_INVEN_START_Y+POS_PET_Y+INVEN_SLOT_SIZE)
				{
					//((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, m_pWearDisplayInfo[POS_PET]->Name);
					if(m_pWearDisplayInfo[POS_ACCESSORY_TIME_LIMIT])
						SetItemInfo(m_pWearDisplayInfo[POS_ACCESSORY_TIME_LIMIT], pt.x, pt.y);
					else
						((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, STRMSG_C_TOOLTIP_0020);//"��Ÿ �����迭 ��� �����ϴ� ��ġ�Դϴ�."
					return INF_MSGPROC_BREAK;
				}
				else
				{
					int i = (pt.x-(CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X))/INVEN_SLOT_INTERVAL;
					if( pt.x > CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X &&
						i >= 0 && i < INVEN_X_NUMBER &&
						pt.x < CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X+INVEN_SLOT_INTERVAL*i+INVEN_SLOT_SIZE)
					{
						int j = (pt.y - CITY_SHOP_INVEN_START_Y - INVEN_ITEM_SLOT_START_Y)/INVEN_SLOT_INTERVAL;
						if( pt.y > CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y &&
							j >= 0 && j < INVEN_Y_NUMBER &&
							pt.y < CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*j + INVEN_SLOT_SIZE)
						{
							if(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i])
							{
								char buf[256];
								ITEM *item = NULL;
								// set tooltip
								if (COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
								{
									if(IS_COUNTABLE_ITEM(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem->Kind))
									{
										item = g_pDatabase->GetServerItemInfo(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem->ItemNum);										
										if(item)
											wsprintf(buf, STRMSG_C_TOOLTIP_0014,item->ItemName,m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem->ItemNum,(int)((ITEM_GENERAL*)m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem)->UniqueNumber,//"%s(%08d)(%08d)(%d ��)"
											((ITEM_GENERAL*)m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem)->CurrentCount );
									}
									else
									{
										item = g_pDatabase->GetServerItemInfo(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem->ItemNum);
										if(item)
											wsprintf(buf, "%s(%8d)(%08d)",item->ItemName,m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem->ItemNum, (int)((ITEM_GENERAL*)m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem)->UniqueNumber);
									}
									
								}
								else
								{
									if(IS_COUNTABLE_ITEM(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem->Kind))
									{
										item = g_pDatabase->GetServerItemInfo(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem->ItemNum);
										if(item)
											wsprintf(buf, STRMSG_C_TOOLTIP_0013,item->ItemName,((ITEM_GENERAL*)m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem)->CurrentCount);//"%s (%d ��)"
									}
									else
									{
										item = g_pDatabase->GetServerItemInfo(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem->ItemNum);
										if(item)
											wsprintf(buf, "%s",item->ItemName);
									}
								}

								if( item )
								{
									m_iWearPosition = item->Position;
								}
								else
								{
									m_iWearPosition = POS_INVALID_POSITION;
								}
								//((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, buf);//m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->Name);
								SetItemInfo(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i], pt.x, pt.y);
							}
							else
							{
								m_iWearPosition = POS_INVALID_POSITION;
								SetItemInfo(NULL, 0, 0);
							}
						}
					}
					else
					{
						m_iWearPosition = POS_INVALID_POSITION;
						
						if(!m_pSelectItem)
						{
							//((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
							SetItemInfo(NULL, 0, 0);
						}
					}
				}
			}		
			else
			{
				if(!m_pSelectItem)
				{
					//((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
					SetItemInfo(NULL, 0, 0);
				}
			}
			//((CINFGameMain*)m_pParent)->SetItemInfo(0,0, 0,0);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nWindowPosY = g_pGameMain->m_nLeftWindowY;
			m_bMouseDownState = TRUE;		

			if( g_pStoreData->m_mapItemUniqueNumber.size() > INVEN_X_NUMBER &&
				pt.x > CITY_BASE_NPC_BOX_START_X+INVEN_SCROLL_BUTTON_START_X && 
				pt.x < CITY_BASE_NPC_BOX_START_X+INVEN_SCROLL_BUTTON_START_X+INVEN_SCROLL_BUTTON_WIDTH &&
				pt.y > nWindowPosY+INVEN_SCROLL_BUTTON_START_Y+m_nInvenCurrentScrollPosition && 
				pt.y < nWindowPosY+INVEN_SCROLL_BUTTON_START_Y+INVEN_SCROLL_BUTTON_HEIGHT+m_nInvenCurrentScrollPosition)
			{
				m_bScrollLock = TRUE;
				m_ptMouse = pt;
			}
			else
			{
				m_bScrollLock = FALSE;
			}
			
			// 2006-07-26 by ispark, ������ ������
			if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
				((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_INVEN_POS)
			{
				m_bSelectItem = TRUE;
			}

			// ����â ������ ����
			if( m_pWearDisplayInfo[POS_PROW] && pt.x>CITY_BASE_NPC_BOX_START_X+POS_PROW_X && pt.x<CITY_BASE_NPC_BOX_START_X+POS_PROW_X+INVEN_SLOT_SIZE &&
				pt.y>nWindowPosY+POS_PROW_Y && pt.y<nWindowPosY+POS_PROW_Y+INVEN_SLOT_SIZE)
			{
				if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC() && IS_TUNING_COLOR_SHOP(g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind))
				{
					// Į�� ���������� ������ ��ü ����
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051207_0003,COLOR_ERROR);
					// 2007-03-02 by bhsohn ���� ���� �߰� ����
					ClearMultiSeletItem();
					break;
				}

				m_nRenderMoveIconIntervalWidth  = pt.x - (CITY_BASE_NPC_BOX_START_X+POS_PROW_X);
				m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY+POS_PROW_Y);

				SetSelectItem(m_pWearDisplayInfo[POS_PROW]);
				m_bSelectWearItem = TRUE;
			}
			else if( m_pWearDisplayInfo[POS_PROWIN] && pt.x>CITY_BASE_NPC_BOX_START_X+POS_PROWIN_X && pt.x<CITY_BASE_NPC_BOX_START_X+POS_PROWIN_X+INVEN_SLOT_SIZE &&
				pt.y>nWindowPosY+POS_PROWIN_Y && pt.y<nWindowPosY+POS_PROWIN_Y+INVEN_SLOT_SIZE)
			{
				if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC() && IS_TUNING_COLOR_SHOP(g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind))
				{
					// Į�� ���������� ������ ��ü ����
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051207_0003,COLOR_ERROR);
					// 2007-03-02 by bhsohn ���� ���� �߰� ����
					ClearMultiSeletItem();
				break;
				}

				m_nRenderMoveIconIntervalWidth  = pt.x - (CITY_BASE_NPC_BOX_START_X+POS_PROWIN_X);
				m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY+POS_PROWIN_Y);

				SetSelectItem(m_pWearDisplayInfo[POS_PROWIN]);
				m_bSelectWearItem = TRUE;
			}
			else if( m_pWearDisplayInfo[POS_PROWOUT] && pt.x>CITY_BASE_NPC_BOX_START_X+POS_PROWOUT_X && pt.x<CITY_BASE_NPC_BOX_START_X+POS_PROWOUT_X+INVEN_SLOT_SIZE &&
				pt.y>nWindowPosY+POS_PROWOUT_Y && pt.y<nWindowPosY+POS_PROWOUT_Y+INVEN_SLOT_SIZE)
			{
				if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC() && IS_TUNING_COLOR_SHOP(g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind))
				{
					// Į�� ���������� ������ ��ü ����
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051207_0003,COLOR_ERROR);
					// 2007-03-02 by bhsohn ���� ���� �߰� ����
					ClearMultiSeletItem();
				break;
				}

				m_nRenderMoveIconIntervalWidth  = pt.x - (CITY_BASE_NPC_BOX_START_X+POS_PROWOUT_X);
				m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY+POS_PROWOUT_Y);

				SetSelectItem(m_pWearDisplayInfo[POS_PROWOUT]);
				m_bSelectWearItem = TRUE;
			}
			// 2006-07-26 by ispark �߰�
			else if( m_pWearDisplayInfo[POS_WINGIN] && pt.x>CITY_BASE_NPC_BOX_START_X+POS_WINGIN_X && pt.x<CITY_BASE_NPC_BOX_START_X+POS_WINGIN_X+INVEN_SLOT_SIZE &&
				pt.y>nWindowPosY+POS_WINGIN_Y && pt.y<nWindowPosY+POS_WINGIN_Y+INVEN_SLOT_SIZE)
			{
				if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC() && IS_TUNING_COLOR_SHOP(g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind))
				{
					// Į�� ���������� ������ ��ü ����
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051207_0003,COLOR_ERROR);
					// 2007-03-02 by bhsohn ���� ���� �߰� ����
					ClearMultiSeletItem();
					break;
				}

				m_nRenderMoveIconIntervalWidth  = pt.x - (CITY_BASE_NPC_BOX_START_X+POS_WINGIN_X);
				m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY+POS_WINGIN_Y);

				SetSelectItem(m_pWearDisplayInfo[POS_WINGIN]);
				m_bSelectWearItem = TRUE;
			}
			else if( m_pWearDisplayInfo[POS_WINGOUT] && pt.x>CITY_BASE_NPC_BOX_START_X+POS_WINGOUT_X && pt.x<CITY_BASE_NPC_BOX_START_X+POS_WINGOUT_X+INVEN_SLOT_SIZE &&
				pt.y>nWindowPosY+POS_WINGOUT_Y && pt.y<nWindowPosY+POS_WINGOUT_Y+INVEN_SLOT_SIZE)
			{
				if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC() && IS_TUNING_COLOR_SHOP(g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind))
				{
					// Į�� ���������� ������ ��ü ����
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051207_0003,COLOR_ERROR);
					// 2007-03-02 by bhsohn ���� ���� �߰� ����
					ClearMultiSeletItem();
				break;
				}

				m_nRenderMoveIconIntervalWidth  = pt.x - (CITY_BASE_NPC_BOX_START_X+POS_WINGOUT_X);
				m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY+POS_WINGOUT_Y);

				SetSelectItem(m_pWearDisplayInfo[POS_WINGOUT]);
				m_bSelectWearItem = TRUE;
			}
			else if( m_pWearDisplayInfo[POS_CENTER] && pt.x>CITY_BASE_NPC_BOX_START_X+POS_CENTER_X && pt.x<CITY_BASE_NPC_BOX_START_X+POS_CENTER_X+INVEN_SLOT_SIZE &&
				pt.y>nWindowPosY+POS_CENTER_Y && pt.y<nWindowPosY+POS_CENTER_Y+INVEN_SLOT_SIZE)
			{
				if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC() && IS_TUNING_COLOR_SHOP(g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind))
				{
					// Į�� ���������� ������ ��ü ����
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051207_0003,COLOR_ERROR);
					// 2007-03-02 by bhsohn ���� ���� �߰� ����
					ClearMultiSeletItem();
				break;
				}

				m_nRenderMoveIconIntervalWidth  = pt.x - (CITY_BASE_NPC_BOX_START_X+POS_CENTER_X);
				m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY+POS_CENTER_Y);

				SetSelectItem(m_pWearDisplayInfo[POS_CENTER]);
				m_bSelectWearItem = TRUE;
			}
			else if( m_pWearDisplayInfo[POS_REAR] && pt.x>CITY_BASE_NPC_BOX_START_X+POS_REAR_X && pt.x<CITY_BASE_NPC_BOX_START_X+POS_REAR_X+INVEN_SLOT_SIZE &&
				pt.y>nWindowPosY+POS_REAR_Y && pt.y<nWindowPosY+POS_REAR_Y+INVEN_SLOT_SIZE)
			{
				if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC() && IS_TUNING_COLOR_SHOP(g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind))
				{
					// Į�� ���������� ������ ��ü ����
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051207_0003,COLOR_ERROR);
					// 2007-03-02 by bhsohn ���� ���� �߰� ����
					ClearMultiSeletItem();
				break;
				}

				m_nRenderMoveIconIntervalWidth  = pt.x - (CITY_BASE_NPC_BOX_START_X+POS_REAR_X);
				m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY+POS_REAR_Y);

				SetSelectItem(m_pWearDisplayInfo[POS_REAR]);
				m_bSelectWearItem = TRUE;
			}
			else if( m_pWearDisplayInfo[POS_ACCESSORY_UNLIMITED] && pt.x>CITY_BASE_NPC_BOX_START_X+POS_ATTACHMENT_X && pt.x<CITY_BASE_NPC_BOX_START_X+POS_ATTACHMENT_X+INVEN_SLOT_SIZE &&
				pt.y>nWindowPosY+POS_ATTACHMENT_Y && pt.y<nWindowPosY+POS_ATTACHMENT_Y+INVEN_SLOT_SIZE)
			{
				if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC() && IS_TUNING_COLOR_SHOP(g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind))
				{
					// Į�� ���������� ������ ��ü ����
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051207_0003,COLOR_ERROR);
					// 2007-03-02 by bhsohn ���� ���� �߰� ����
					ClearMultiSeletItem();
				break;
				}
				
				m_nRenderMoveIconIntervalWidth  = pt.x - (CITY_BASE_NPC_BOX_START_X+POS_ATTACHMENT_X);
				m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY+POS_ATTACHMENT_Y);

				SetSelectItem(m_pWearDisplayInfo[POS_ACCESSORY_UNLIMITED]);
				m_bSelectWearItem = TRUE;
				// 2007-03-02 by bhsohn ���� ���� �߰� ����
				ClearMultiSeletItem();
				break;
			}
			// 2006-03-30 by ispark
			else if( m_pWearDisplayInfo[POS_ACCESSORY_TIME_LIMIT] && pt.x>CITY_BASE_NPC_BOX_START_X+POS_PET_X && pt.x<CITY_BASE_NPC_BOX_START_X+POS_PET_X+INVEN_SLOT_SIZE &&
				pt.y>nWindowPosY+POS_PET_Y && pt.y<nWindowPosY+POS_ATTACHMENT_Y+POS_PET_Y+INVEN_SLOT_SIZE)
			{
				// �Ǽ��縮
				if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC() && IS_TUNING_COLOR_SHOP(g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind))
				{
					// Į�� ���������� ������ ��ü ����
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051207_0003,COLOR_ERROR);
					// 2007-03-02 by bhsohn ���� ���� �߰� ����
					ClearMultiSeletItem();
				break;
				}
				
				m_nRenderMoveIconIntervalWidth  = pt.x - (CITY_BASE_NPC_BOX_START_X+POS_PET_X);
				m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY+POS_PET_Y);

				SetSelectItem(m_pWearDisplayInfo[POS_ACCESSORY_TIME_LIMIT]);
				m_bSelectWearItem = TRUE;
				// 2007-03-02 by bhsohn ���� ���� �߰� ����
				ClearMultiSeletItem();
				break;
			}
//			else
//			{
//				if(m_bSelectItem)
//				{
//					int i = (pt.x - g_pGameMain->m_pQuickSlot->m_nX - 1)/QSLOT_ICON_INTERVAL;
//					if( i >= 0 && 
//						i < QSLOT_NUMBER ) 
//					{
//						SetSelectItem(NULL);
//					}
//					
//					m_bSelectItem = FALSE;
//				}
//			}

			// ������â ������ ����
			int i = (pt.x-(CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X))/INVEN_SLOT_INTERVAL;
			if( pt.x > CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X &&
				i >= 0 && i < INVEN_X_NUMBER /*&&*/
				/*pt.x < INVEN_ITEM_SLOT_START_X+INVEN_SLOT_INTERVAL*i+INVEN_SLOT_SIZE*/)
			{
				int j = (pt.y - nWindowPosY - INVEN_ITEM_SLOT_START_Y)/INVEN_SLOT_INTERVAL;
				if( j >= 0 && 
					j < INVEN_Y_NUMBER &&
					m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i] && pt.y > nWindowPosY + INVEN_ITEM_SLOT_START_Y &&
					pt.y < nWindowPosY + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*j + INVEN_SLOT_SIZE)
				{
					m_nRenderMoveIconIntervalWidth  = pt.x - (CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X+(INVEN_SLOT_INTERVAL*i));
					m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY + INVEN_ITEM_SLOT_START_Y + (INVEN_SLOT_INTERVAL*j));
					m_bSelectWearItem = FALSE;

					if(m_bEnchantState && m_pEnchantItem)
					{
						m_bEnchantState = FALSE;
						((CINFGameMain*)m_pParent)->m_pInfWindow->DeleteMsgBox(_Q_USE_ENCHANT_ITEM_CARD);
					}
					else
					{
						// 2007-02-12 by bhsohn Item ���� ���� ó��
						//if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem == NULL)						
						//{
						//		SetSelectItem(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]);
						//}
						BOOL bMuitiItemSel = FALSE;
						
						//if(GetAsyncKeyState(VK_CONTROL))
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
						}						
						
						if(!bMuitiItemSel)
						{
							int nSelItemCnt = g_pD3dApp->GetMultiSelectItem();
							// 2007-03-02 by bhsohn ���� ���� �߰� ����
							//SetMultiSelectItem(NULL);	// ���� ó�� ����
							BOOL bMultiDragSel = FALSE;
							
							// ���� �������� �� �����߳�?
							if(g_pD3dApp->CheckMultItemSel(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem->UniqueNumber))
							{
								bMultiDragSel = TRUE;
							}
							if(!bMultiDragSel)
							{
							SetMultiSelectItem(NULL);	// ���� ó�� ����
							}							
							// end 2007-03-02 by bhsohn ���� ���� �߰� ����

							// �̱� ����
						if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem == NULL)						
						{
							SetSelectItem(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]);
						}
						}						
						else
						{
							// ���� ����
							if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem == NULL)						
							{
								SetMultiSelectItem(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]);
								SetItemInfo(NULL, 0, 0);
								return INF_MSGPROC_BREAK;
							}
							else
							{
								SetMultiSelectItem(NULL);	// ���� ó�� ����
							}
						}												
						// end 2007-02-12 by bhsohn Item ���� ���� ó��
					}
				}
				else
				{
					// 2007-03-02 by bhsohn ���� ���� �߰� ����
					ClearMultiSeletItem();
				}
			}
			else
			{
				// 2007-03-02 by bhsohn ���� ���� �߰� ����
				// �κ��� ������ �������� �ִ�. 
				ClearMultiSeletItem();
			}

//			// 2005-08-05 by ispark
//			// â�ȿ��� ���콺 Ŭ���� ��ȿ
//			if(	pt.x<INVEN_SCROLL_LINE_START_X + INVEN_SCROLL_BUTTON_WIDTH &&
//				pt.y>nWindowPosY && 
//				pt.y<nWindowPosY+INVEN_SCROLL_LINE_START_Y+INVEN_SCROLL_BUTTON_START_Y)
//			{
//				g_pInterface->SetWindowOrder(WNDLeftRightWindow);
//				return INF_MSGPROC_BREAK;				
//			}
			
			// �켱 ���� ����
//			if( pt.x > 1 && pt.x <426 && pt.y > g_pGameMain->m_nLeftWindowY && pt.y <g_pGameMain->m_nLeftWindowY +275)
//			{
//				g_pInterface->SetWindowOrder(WNDLeftRightWindow);
//				return INF_MSGPROC_BREAK;
//			}
//
			SetItemInfo(NULL, 0, 0);

			// 2006-07-26 by ispark, ���� �������� �ִٸ� ���콺 Ŭ���� ��ȿ
			if(m_bSelectItem)
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

			m_bScrollLock = FALSE;

			CItemInfo* pSelectItem = NULL;
			if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem)
			{
				pSelectItem = (CItemInfo*)(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem); 
			}
			// 2006-07-26 by ispark, �κ��� �������� �ƴ϶�� ������.
			if(((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType != ITEM_INVEN_POS)
			{
				break;
			}
			//////////////////////////////////////////////////////////////////////////
			// ������ -> ����â
			if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_PROW_X && 
				pt.x<CITY_BASE_NPC_BOX_START_X+POS_PROW_X+INVEN_SLOT_SIZE &&
				pt.y>CITY_SHOP_INVEN_START_Y+POS_PROW_Y && 
				pt.y<CITY_SHOP_INVEN_START_Y+POS_PROW_Y+INVEN_SLOT_SIZE)
			{
				SendChangeWearWindowPos(POS_PROW);
			}
			else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_PROWIN_X && 
				pt.x<CITY_BASE_NPC_BOX_START_X+POS_PROWIN_X+INVEN_SLOT_SIZE &&
				pt.y>CITY_SHOP_INVEN_START_Y+POS_PROWIN_Y && 
				pt.y<CITY_SHOP_INVEN_START_Y+POS_PROWIN_Y+INVEN_SLOT_SIZE)
			{
				SendChangeWearWindowPos(POS_PROWIN);
			}
			else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_PROWOUT_X &&
				pt.x<CITY_BASE_NPC_BOX_START_X+POS_PROWOUT_X+INVEN_SLOT_SIZE &&
				pt.y>CITY_SHOP_INVEN_START_Y+POS_PROWOUT_Y && 
				pt.y<CITY_SHOP_INVEN_START_Y+POS_PROWOUT_Y+INVEN_SLOT_SIZE)
			{
				SendChangeWearWindowPos(POS_PROWOUT);
			}
			else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_WINGIN_X && 
				pt.x<CITY_BASE_NPC_BOX_START_X+POS_WINGIN_X+INVEN_SLOT_SIZE &&
				pt.y>CITY_SHOP_INVEN_START_Y+POS_WINGIN_Y && 
				pt.y<CITY_SHOP_INVEN_START_Y+POS_WINGIN_Y+INVEN_SLOT_SIZE)
			{
				// 2006-07-26 by ispark, �߰�
				SendChangeWearWindowPos(POS_WINGIN);
			}
			else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_WINGOUT_X && 
				pt.x<CITY_BASE_NPC_BOX_START_X+POS_WINGOUT_X+INVEN_SLOT_SIZE &&
				pt.y>CITY_SHOP_INVEN_START_Y+POS_WINGOUT_Y && 
				pt.y<CITY_SHOP_INVEN_START_Y+POS_WINGOUT_Y+INVEN_SLOT_SIZE)
			{
				SendChangeWearWindowPos(POS_WINGOUT);
			}
			else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_CENTER_X && 
				pt.x<CITY_BASE_NPC_BOX_START_X+POS_CENTER_X+INVEN_SLOT_SIZE &&
				pt.y>CITY_SHOP_INVEN_START_Y+POS_CENTER_Y && 
				pt.y<CITY_SHOP_INVEN_START_Y+POS_CENTER_Y+INVEN_SLOT_SIZE)
			{
				SendChangeWearWindowPos(POS_CENTER);
			}
			else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_REAR_X && 
				pt.x<CITY_BASE_NPC_BOX_START_X+POS_REAR_X+INVEN_SLOT_SIZE &&
				pt.y>CITY_SHOP_INVEN_START_Y+POS_REAR_Y && 
				pt.y<CITY_SHOP_INVEN_START_Y+POS_REAR_Y+INVEN_SLOT_SIZE)
			{
				SendChangeWearWindowPos(POS_REAR);
			}
			//
			//////////////////////////////////////////////////////////////////////////			
			else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_ATTACHMENT_X && 
				pt.x<CITY_BASE_NPC_BOX_START_X+POS_ATTACHMENT_X+INVEN_SLOT_SIZE &&
				pt.y>CITY_SHOP_INVEN_START_Y+POS_ATTACHMENT_Y &&
				pt.y<CITY_SHOP_INVEN_START_Y+POS_ATTACHMENT_Y+INVEN_SLOT_SIZE)
			{
				SendChangeWearWindowPos(POS_ACCESSORY_UNLIMITED);
			}
			// 2006-03-30 by ispark
			else if( pt.x>CITY_BASE_NPC_BOX_START_X+POS_PET_X && 
				pt.x<CITY_BASE_NPC_BOX_START_X+POS_PET_X+INVEN_SLOT_SIZE &&
				pt.y>CITY_SHOP_INVEN_START_Y+POS_PET_Y &&
				pt.y<CITY_SHOP_INVEN_START_Y+POS_PET_Y+INVEN_SLOT_SIZE)
			{
				SendChangeWearWindowPos(POS_ACCESSORY_TIME_LIMIT);
			}
			else if(pSelectItem && 
					pt.x>CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X && 
					pt.x<CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X+INVEN_SLOT_INTERVAL*INVEN_X_NUMBER && 
					pt.y>CITY_SHOP_INVEN_START_Y+INVEN_ITEM_SLOT_START_Y && 
					pt.y<CITY_SHOP_INVEN_START_Y+INVEN_ITEM_SLOT_START_Y+INVEN_SLOT_INTERVAL*INVEN_Y_NUMBER)
			{
				int i = (pt.x-(CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X))/INVEN_SLOT_INTERVAL;
				int j = (pt.y - CITY_SHOP_INVEN_START_Y - INVEN_ITEM_SLOT_START_Y)/INVEN_SLOT_INTERVAL;
				int nWindowPosition = j*INVEN_X_NUMBER+i+m_nInvenCurrentScroll*INVEN_X_NUMBER+POS_ITEMWINDOW_OFFSET;
				if(	nWindowPosition != pSelectItem->ItemWindowIndex &&
					pSelectItem->ItemWindowIndex >= POS_ITEMWINDOW_OFFSET &&
					pt.x > CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X &&
					i >= 0 && i < INVEN_X_NUMBER &&
					pt.x < CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X+INVEN_SLOT_INTERVAL*i+INVEN_SLOT_SIZE &&
					pt.y > CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y &&
					j >= 0 && j < INVEN_Y_NUMBER &&
					pt.y < CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*j + INVEN_SLOT_SIZE )
				{	// ������ -> ������
					if( pSelectItem->ItemWindowIndex < nWindowPosition )
					{
						INVEN_DISPLAY_INFO* pSelectInvenDisplayInfo = m_pInvenDisplayInfo[pSelectItem->ItemWindowIndex];
						for(int i=pSelectItem->ItemWindowIndex+1; i<= nWindowPosition; i++)
						{
							CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(i);
							if(it != g_pStoreData->m_mapItemWindowPosition.end())
							{
								CItemInfo* pSwapItem = it->second;
								pSwapItem->ItemWindowIndex = i-1;
								g_pStoreData->m_mapItemWindowPosition[i-1] = pSwapItem;
								SetSingleInvenIconInfo(pSwapItem);
							} else {
								pSelectItem->ItemWindowIndex = i-1;
								g_pStoreData->m_mapItemWindowPosition[i-1] = pSelectItem;
								SetAllIconInfo();
								SetSelectItem(NULL);
								return INF_MSGPROC_NORMAL;
							}
						}
						pSelectItem->ItemWindowIndex = nWindowPosition;
						g_pStoreData->m_mapItemWindowPosition[nWindowPosition] = pSelectItem;
						SetSingleInvenIconInfo(pSelectItem);
						SetAllIconInfo();
					}
					else if( pSelectItem->ItemWindowIndex > nWindowPosition )
					{
						INVEN_DISPLAY_INFO* pSelectInvenDisplayInfo = m_pInvenDisplayInfo[pSelectItem->ItemWindowIndex];
						for(int i=pSelectItem->ItemWindowIndex-1; i >= nWindowPosition; i--)
						{
							CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(i);
							if(it != g_pStoreData->m_mapItemWindowPosition.end())
							{
								CItemInfo* pSwapItem = it->second;
								pSwapItem->ItemWindowIndex = i+1;
								g_pStoreData->m_mapItemWindowPosition[i+1] = pSwapItem;
								SetSingleInvenIconInfo(pSwapItem);
							} else {
								DBGOUT("ERROR : CINFInven::WndProc() Item List crashed!!!!!\n");
								SetSelectItem(NULL);
								return INF_MSGPROC_NORMAL;// ������ ����Ʈ�� �������
							}
						}
						pSelectItem->ItemWindowIndex = nWindowPosition;
						g_pStoreData->m_mapItemWindowPosition[nWindowPosition] = pSelectItem;
						SetSingleInvenIconInfo(pSelectItem);
						SetAllIconInfo();
					}
				}
				// ����â -> ������
				else if(pSelectItem->ItemWindowIndex < POS_ITEMWINDOW_OFFSET)
				{
					// 2004-12-10 by jschoi
					if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC() && IS_TUNING_COLOR_SHOP(g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind))
					{
						// Į�� ���������� ������ ��ü ����
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051207_0003,COLOR_ERROR);
						break;
					}
					// 2006-08-02 by ispark, ���λ������� ������ ��ü ����
					if(g_pInterface->m_pBazaarShop)
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_060802_0000,COLOR_ERROR);
						break;
					}
					if(IsAbleReleaseItem(pSelectItem,pSelectItem->ItemWindowIndex))
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
						int count=0;

						// 2010-06-15 by shcho&hslee ��ý��� - ����ǰ �ִ� ���� ����.
						//for(int i=0;i<WEAR_ITEM_NUMBER;i++)
						for(int i=0;i<MAX_EQUIP_POS;i++)
						{
							if(m_pWearDisplayInfo[i])
								count++;
						}
						i=POS_ITEMWINDOW_OFFSET+g_pStoreData->m_mapItemWindowPosition.size();//-count-1;
						pMsg->ToItemWindowIndex = i;
						// �������� ������������ �κ��丮�� ���� �� �ش� ��ų�� ���⼭ ������ ��..���� ��û
						if(pSelectItem)
						{
							g_pShuttleChild->m_pSkill->DeleteSkillFromWearItem(pSelectItem->Kind);
						}
						g_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_CHANGE_WINDOW_POSITION));
//						g_pD3dApp->m_bRequestEnable = FALSE;			// 2006-06-19 by ispark, �޼��� ������ ��ٸ���.
					}
					else
					{
						char buf[128];
						wsprintf(buf,STRMSG_C_ITEM_0007);//"�䱸 �������� ���Ͽ� �ش� �������� ���� �� �� �����ϴ�."
						g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
					}

				}
			}
			else if(pSelectItem && 
					pt.x>CITY_BASE_NPC_BOX_START_X+INVEN_GARBAGE_START_X && 
					pt.x<CITY_BASE_NPC_BOX_START_X+INVEN_GARBAGE_START_X+INVEN_GARBAGE_SIZE &&
					pt.y>CITY_SHOP_INVEN_START_Y+INVEN_GARBAGE_START_Y && 
					pt.y<CITY_SHOP_INVEN_START_Y+INVEN_GARBAGE_START_Y + INVEN_GARBAGE_SIZE
					&& g_pD3dApp->GetMultiSelectItem() <= 0)		// 2007-03-02 by bhsohn ���� ���� �߰� ����)
			{

				if( !((CINFGameMain*)m_pParent)->m_pInfWindow->IsExistMsgBox(_Q_ITEM_DELETE) &&
					!((CINFGameMain*)m_pParent)->m_pInfWindow->IsExistMsgBox(_Q_ITEM_DELETE_NUM))
				{
					char buf[256];
					ITEM *pITEM = pSelectItem->GetItemInfo();
					if(pITEM)
					{
						if(pSelectItem->ItemWindowIndex < POS_ITEMWINDOW_OFFSET)
						{
							((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox(STRMSG_C_ITEM_0008, _MESSAGE);//"������ �������� ���� �� �����ϴ�."
							pSelectItem = NULL;
						}
						else
						{
							if(IS_COUNTABLE_ITEM(pSelectItem->Kind) && pSelectItem->CurrentCount>1)
							{
								wsprintf(buf, STRMSG_C_ITEM_0009, pITEM->ItemName);//"������ %s ��� �����ðڽ��ϱ�?"
								((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox(buf,
									_Q_ITEM_DELETE_NUM, (DWORD)pSelectItem, pSelectItem->CurrentCount);
							}
							else
							{
								wsprintf(buf, STRMSG_C_ITEM_0010, pITEM->ItemName);//"������ %s ��(��)  �����ðڽ��ϱ�?"
								((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox(buf, _Q_ITEM_DELETE);
							}
							// 2007-06-20 by bhsohn ������ ������, �޸� ���� ����
							//m_pDeleteItem = pSelectItem;
							SetDeleteItemInfo(pSelectItem);
						}
					}
				}
			}
			if(m_bSelectItem)
			{
			SetSelectItem(NULL);
				m_bSelectItem = FALSE;
				m_bSelectWearItem = FALSE;
				break;
			}
			((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
			SetItemInfo(NULL, 0, 0);
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			if(((CINFGameMain*)m_pParent)->m_pTrade->m_bTrading)
				return INF_MSGPROC_NORMAL;
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			int i = (pt.x-(CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X))/INVEN_SLOT_INTERVAL;
			if( pt.x > CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X &&
				i >= 0 && i < INVEN_X_NUMBER &&
				pt.x < CITY_BASE_NPC_BOX_START_X+INVEN_ITEM_SLOT_START_X+INVEN_SLOT_INTERVAL*i+INVEN_SLOT_SIZE)
			{
				int j = (pt.y - CITY_SHOP_INVEN_START_Y - INVEN_ITEM_SLOT_START_Y)/INVEN_SLOT_INTERVAL;
				if( pt.y > CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y &&
					j >= 0 && j < INVEN_Y_NUMBER &&
					pt.y < CITY_SHOP_INVEN_START_Y + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*j + INVEN_SLOT_SIZE)
				{
					// 2007-06-20 by bhsohn ������ ������, �޸� ���� ����
					//if(!m_pDeleteItem && m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i])
					if(0 == m_struDeleteItem.UniqueNumber && m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i])
					{
						// set tooltip
						CItemInfo* pItemInfo = (CItemInfo*)m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem;
						ITEM *pITEM = pItemInfo->GetItemInfo();
//						ITEM *item = g_pDatabase->GetServerItemInfo( pItem->ItemNum);
						if(pITEM)
						{
// 2008-06-03 by dgwoo ������ �����ִ� ���¿��� ����Ŭ������ ������ ��ü�� ����.
//							if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC() && IS_TUNING_COLOR_SHOP(g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind))
//							{
//								// Į�� ���������� ������ ��ü ����
//								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051207_0003,COLOR_ERROR);
//							}
//							// 2006-08-03 by ispark, ���λ��������� ������ ��� ����
//							else if(g_pInterface->m_pBazaarShop)
//							{
//								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_060802_0001,COLOR_ERROR);
//							}
							if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC())
							{
								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080603_0100,COLOR_ERROR);
							}
							else
							{
								switch(pITEM->Position)
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
		
										SendUseItem((ITEM_BASE*)pItemInfo);
									}
									break;
								case POS_PROWIN:
								case POS_PROWOUT:
								case POS_WINGIN:
								case POS_WINGOUT:
								case POS_PROW:
								case POS_CENTER:
								case POS_REAR:
								case POS_ACCESSORY_UNLIMITED:
								case POS_ACCESSORY_TIME_LIMIT:				// 2006-03-31 by ispark
								case POS_PET :								// 2010-06-15 by shcho&hslee ��ý��� - ���� ����.
									{
										SetSelectItem(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]);
										SendChangeWearWindowPos(pITEM->Position);
									}
									break;
								default:
									{
										char buf[128];
										wsprintf(buf,STRMSG_C_ITEM_0011,pItemInfo->ItemNum);//"item [ %08d ] �� ���� �� �� ���� �������Դϴ�."
										g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
									}
								}
							}
						}
						SetSelectItem(NULL);
						((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
					}
				}
			}
		}
		break;
	// 2007-02-12 by bhsohn Item ���� ���� ó��
	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_CONTROL:
				{
					g_pD3dApp->OnCtrlBtnClick(TRUE);					
				}
				break;
			}
		}
		break;
	case WM_KEYUP:
		{
			switch( wParam )
			{
			case VK_CONTROL:
				{					
					g_pD3dApp->OnCtrlBtnClick(FALSE);
				}
				break;
			}		
		}
		break;
		// end 2007-02-12 by bhsohn Item ���� ���� ó��
	}
	
	return INF_MSGPROC_NORMAL;
}

int CINFInven::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFInven::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );

	// �κ��丮 ĳ���� �����̼�
	ProcessRotationUnitWnd(uMsg, wParam, lParam);

	switch(uMsg)
	{
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);

			int nWindowPosY = g_pGameMain->m_nLeftWindowY;
			if( pt.x > INVEN_ITEM_SLOT_START_X &&
				pt.x < INVEN_ITEM_SLOT_START_X+INVEN_SLOT_INTERVAL*INVEN_X_NUMBER &&
				pt.y > nWindowPosY + INVEN_ITEM_SLOT_START_Y &&
				pt.y < nWindowPosY + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*INVEN_Y_NUMBER)
			{
				// 2005-12-10 by ispark, �⺻ 6���� �Ѿ�� ������
				if(GetScrollLine() > 0) 
				{
					if((int)wParam<0)
					{
						m_nInvenCurrentScroll++;
						if(m_nInvenCurrentScroll > GetScrollLine())
						{
							m_nInvenCurrentScroll = GetScrollLine();
							m_nInvenCurrentScrollPosition = INVEN_SCROLL_LINE_MOVE_LENGTH;
						}
						else
						{
							m_nInvenCurrentScrollPosition = GetScrollLineInterval(m_nInvenCurrentScroll);
						}

						SetAllIconInfo();
						SetItemInfo(NULL, 0, 0);
					}
					else
					{
						m_nInvenCurrentScroll--;
						if(m_nInvenCurrentScroll < 0)
						{
							m_nInvenCurrentScroll = 0;
							m_nInvenCurrentScrollPosition = 0;
						}
						else
						{
							m_nInvenCurrentScrollPosition = GetScrollLineInterval(m_nInvenCurrentScroll);
						}

						SetAllIconInfo();
						SetItemInfo(NULL, 0, 0);
					}

				}
				return INF_MSGPROC_BREAK;
			}
			break;
		}
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			((CINFGameMain*)m_pParent)->SetToolTip(NULL, 0, 0);

			int nWindowPosY = g_pGameMain->m_nLeftWindowY;
//			if(m_bSelectItem == TRUE && m_bMouseDownState == TRUE)
//			{
//				m_bSelectItem = FALSE;
////				if(m_bSelectWearItem) m_bSelectItem = TRUE;
//			}
			if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem && 
				((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_INVEN_POS)
			{
				m_bSelectItem = TRUE;
			}
	
			if(m_bScrollLock && GetScrollLine() > 0)
			{
				// 2005-12-08 by ispark, ��ũ�ѹ� ����
				if(pt.x >= INVEN_SCROLL_LINE_START_X - 20 && 
					pt.x <= INVEN_SCROLL_LINE_START_X + 20)
				{
					if(pt.y >= nWindowPosY + INVEN_SCROLL_LINE_START_Y &&
						pt.y <= nWindowPosY + INVEN_SCROLL_LINE_START_Y + INVEN_SCROLL_LINE_LENGTH)
					{
						int nMove = pt.y - m_ptMouse.y;
//						float fLength = GetScrollLineNumber(m_nInvenCurrentScroll);
						m_nInvenCurrentScrollPosition += nMove;
						m_nInvenCurrentScroll = GetScrollLineNumber(m_nInvenCurrentScrollPosition);
						if(m_nInvenCurrentScrollPosition < 0)
						{
							m_nInvenCurrentScroll = 0;
							m_nInvenCurrentScrollPosition = 0;
						}
						else if(m_nInvenCurrentScrollPosition > INVEN_SCROLL_LINE_MOVE_LENGTH)
						{
							m_nInvenCurrentScroll = GetScrollLine();
							m_nInvenCurrentScrollPosition = INVEN_SCROLL_LINE_MOVE_LENGTH;
						}
					
						if(nMove != 0)
						{
							m_ptMouse = pt;
							SetAllIconInfo();
						}
					}
					else if(pt.y < nWindowPosY + INVEN_SCROLL_LINE_START_Y)
					{
						m_nInvenCurrentScroll = 0;
						m_nInvenCurrentScrollPosition = 0;
						SetAllIconInfo();
					}
					else if(pt.y > nWindowPosY + INVEN_SCROLL_LINE_START_Y + INVEN_SCROLL_LINE_LENGTH)
					{
						m_nInvenCurrentScroll = GetScrollLine();
						m_nInvenCurrentScrollPosition = INVEN_SCROLL_LINE_MOVE_LENGTH;
						SetAllIconInfo();
					}
				}
			}

			if(!m_bSelectItem)
			{
				SetItemInfo(NULL, 0, 0);
				
				if( pt.x>POS_PROW_X && 
					pt.x<POS_PROW_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+POS_PROW_Y &&
					pt.y<nWindowPosY+POS_PROW_Y+INVEN_SLOT_SIZE)
				{
					// ���̴� ����
					if(m_pWearDisplayInfo[POS_PROW])
						SetItemInfo(m_pWearDisplayInfo[POS_PROW], pt.x, pt.y);
					else
						((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, STRMSG_C_TOOLTIP_0015);//"���̴� ��� �����ϴ� ��ġ�Դϴ�."
					return INF_MSGPROC_BREAK;
				}
				else if( //m_pWearDisplayInfo[POS_PROWIN] && 
					pt.x>POS_PROWIN_X && 
					pt.x<POS_PROWIN_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+POS_PROWIN_Y && 
					pt.y<nWindowPosY+POS_PROWIN_Y+INVEN_SLOT_SIZE)
				{				
					if(m_pWearDisplayInfo[POS_PROWIN])
						SetItemInfo(m_pWearDisplayInfo[POS_PROWIN], pt.x, pt.y);
					else
						((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, STRMSG_C_TOOLTIP_0016);//"��ǻ�� �������� �����ϴ� ��ġ�Դϴ�."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x>POS_PROWOUT_X && 
					pt.x<POS_PROWOUT_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+POS_PROWOUT_Y &&
					pt.y<nWindowPosY+POS_PROWOUT_Y+INVEN_SLOT_SIZE)
				{
					// 1�� ���� ����
					if(m_pWearDisplayInfo[POS_PROWOUT])
						SetItemInfo(m_pWearDisplayInfo[POS_PROWOUT], pt.x, pt.y);
					else
						((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, STRMSG_C_TOOLTIP_0017);//"�⺻���⸦ �����ϴ� ��ġ�Դϴ�."
					return INF_MSGPROC_BREAK;
				}
				// 2006-07-20 by dgwoo ��ü ����� ���� ������
				else if(pt.x>POS_WINGIN_X&&
					pt.x<POS_WINGIN_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+POS_WINGIN_Y &&
					pt.y<nWindowPosY+POS_WINGIN_Y+INVEN_SLOT_SIZE)
				{
					if(m_pWearDisplayInfo[POS_WINGIN])
						SetItemInfo(m_pWearDisplayInfo[POS_WINGIN], pt.x, pt.y);
					else
						((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y,STRMSG_C_060720_0100);//"��ü ����� ���� �������� �����ϴ� ��ġ�Դϴ�."
					return INF_MSGPROC_BREAK;

				}
				else if( pt.x>POS_WINGOUT_X &&
				pt.x<POS_WINGOUT_X+INVEN_SLOT_SIZE &&
				pt.y>nWindowPosY+POS_WINGOUT_Y && 
				pt.y<nWindowPosY+POS_WINGOUT_Y+INVEN_SLOT_SIZE)
				{
					// 2�� ���� ����
					if(m_pWearDisplayInfo[POS_WINGOUT])
						SetItemInfo(m_pWearDisplayInfo[POS_WINGOUT], pt.x, pt.y);
					else
						((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, STRMSG_C_TOOLTIP_0018);//"��޹��⸦ �����ϴ� ��ġ�Դϴ�."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x>POS_CENTER_X && 
					pt.x<POS_CENTER_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+POS_CENTER_Y &&
					pt.y<nWindowPosY+POS_CENTER_Y+INVEN_SLOT_SIZE)
				{
					// �Ƹӷ� ������
					if(m_pWearDisplayInfo[POS_CENTER])
						SetItemInfo(m_pWearDisplayInfo[POS_CENTER], pt.x, pt.y);
					else
						((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, STRMSG_C_TOOLTIP_0019);//"�Ƹ� ��� �����ϴ� ��ġ�Դϴ�."
					return INF_MSGPROC_BREAK;
				}
				else if( m_pWearDisplayInfo[POS_REAR] && 
					pt.x>POS_REAR_X && 
					pt.x<POS_REAR_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+POS_REAR_Y && 
					pt.y<nWindowPosY+POS_REAR_Y+INVEN_SLOT_SIZE)
				{
					// ������ ������
					SetItemInfo(m_pWearDisplayInfo[POS_REAR], pt.x, pt.y);
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x>POS_ATTACHMENT_X && 
					pt.x<POS_ATTACHMENT_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+POS_ATTACHMENT_Y && 
					pt.y<nWindowPosY+POS_ATTACHMENT_Y+INVEN_SLOT_SIZE)
				{
					// �����̳ʷ� ������					
					if(m_pWearDisplayInfo[POS_ACCESSORY_UNLIMITED])
						SetItemInfo(m_pWearDisplayInfo[POS_ACCESSORY_UNLIMITED], pt.x, pt.y);
					else
						((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, STRMSG_C_TOOLTIP_0020);//"��Ÿ �����迭 ��� �����ϴ� ��ġ�Դϴ�."
					return INF_MSGPROC_BREAK;
				}
				// 2006-03-30 by ispark, �Ǽ��縮
				else if(pt.x>POS_PET_X && pt.x<POS_PET_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+POS_PET_Y && pt.y<nWindowPosY+POS_PET_Y+INVEN_SLOT_SIZE)
				{
					if(m_pWearDisplayInfo[POS_ACCESSORY_TIME_LIMIT])
						SetItemInfo(m_pWearDisplayInfo[POS_ACCESSORY_TIME_LIMIT], pt.x, pt.y);
					else
						((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, STRMSG_C_061018_0102);//"��Ÿ �����迭 ���(2)�� �����ϴ� ��ġ�Դϴ�."
					return INF_MSGPROC_BREAK;
				}
				else
				{
					int i = (pt.x-INVEN_ITEM_SLOT_START_X)/INVEN_SLOT_INTERVAL;

					if( pt.x > INVEN_ITEM_SLOT_START_X &&
						i >= 0 && i < INVEN_X_NUMBER &&
						pt.x < INVEN_ITEM_SLOT_START_X+INVEN_SLOT_INTERVAL*i+INVEN_SLOT_SIZE)
					{
						int j = (pt.y - nWindowPosY - INVEN_ITEM_SLOT_START_Y)/INVEN_SLOT_INTERVAL;

						if( pt.y > nWindowPosY + INVEN_ITEM_SLOT_START_Y &&
							j >= 0 && j < INVEN_Y_NUMBER &&
							pt.y < nWindowPosY + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*j + INVEN_SLOT_SIZE)
						{
							if(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i])
							{
								// 2010-06-15 by shcho&hslee ��ý��� - �κ��丮 ���� ���� ����, �� ���̴� �ڵ� �ּ�.
								//char buf[256];
								ITEM *item = NULL;
								// set tooltip
// 								if(COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race,RACE_OPERATION|RACE_GAMEMASTER))
// 								{
// 									if(IS_COUNTABLE_ITEM(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem->Kind))
// 									{
// 										item = g_pDatabase->GetServerItemInfo(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem->ItemNum);
// 										if(item)
// 											wsprintf(buf, STRMSG_C_TOOLTIP_0014,item->ItemName,m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem->ItemNum,(int)((ITEM_GENERAL*)m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem)->UniqueNumber,//"%s(%08d)(%08d)(%d ��)"
// 											((ITEM_GENERAL*)m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem)->CurrentCount );
// 									}
// 									else
// 									{
// 										item = g_pDatabase->GetServerItemInfo(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem->ItemNum);
// 										if(item)
// 											wsprintf(buf, "%s(%8d)(%08d)",item->ItemName,m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem->ItemNum, (int)((ITEM_GENERAL*)m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem)->UniqueNumber);
// 									}
// 								}
// 								else
// 								{
// 									if(IS_COUNTABLE_ITEM(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem->Kind))
// 									{
// 										item = g_pDatabase->GetServerItemInfo(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem->ItemNum);
// 										if(item)
// 											wsprintf(buf, STRMSG_C_TOOLTIP_0013,item->ItemName,((ITEM_GENERAL*)m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem)->CurrentCount);//"%s (%d ��)"
// 									}
// 									else
// 									{
// 										item = g_pDatabase->GetServerItemInfo(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem->ItemNum);
// 										if(item)
// 											wsprintf(buf, "%s",item->ItemName);
// 									}
// 								}

								if( item )
								{
									m_iWearPosition = item->Position;
								}
								else
								{
									m_iWearPosition = POS_INVALID_POSITION;
								}
								//((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, buf);//m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->Name);
								SetItemInfo(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i], pt.x, pt.y);								
								return INF_MSGPROC_BREAK;
							}
							else
							{
								m_iWearPosition = POS_INVALID_POSITION;
								if(!m_pSelectItem)
								{
									SetItemInfo(NULL, 0, 0);									
								}
							}
						}
					}
					else
					{
						m_iWearPosition = POS_INVALID_POSITION;
						
						if(!m_pSelectItem)
						{
							//((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
							SetItemInfo(NULL, 0, 0);
						}
					}
				}
			}		
			else
			{
				if(!m_pSelectItem)
				{
					//((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
					SetItemInfo(NULL, 0, 0);
				}
			}
			//((CINFGameMain*)m_pParent)->SetItemInfo(0,0, 0,0);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nWindowPosY = g_pGameMain->m_nLeftWindowY;
			m_bMouseDownState = TRUE;		

//			// 2006-07-27 by ispark, �κ� �ܰ�
//			if(!(pt.x<INVEN_FULL_BACK_W &&
//				pt.y>nWindowPosY && 
//				pt.y<nWindowPosY+INVEN_FULL_BACK_H))
//			{
//				return INF_MSGPROC_NORMAL;
//			}

			if( g_pStoreData->m_mapItemUniqueNumber.size() > INVEN_X_NUMBER &&
				pt.x > INVEN_SCROLL_BUTTON_START_X &&
				pt.x < INVEN_SCROLL_BUTTON_START_X+INVEN_SCROLL_BUTTON_WIDTH &&
				pt.y > nWindowPosY + INVEN_SCROLL_BUTTON_START_Y + m_nInvenCurrentScrollPosition && 
				pt.y < nWindowPosY + INVEN_SCROLL_BUTTON_START_Y + INVEN_SCROLL_BUTTON_HEIGHT + m_nInvenCurrentScrollPosition)
			{
				m_bScrollLock = TRUE;
				m_ptMouse = pt;
			}
			else
			{
				m_bScrollLock = FALSE;
			}

			// 2006-07-26 by ispark, ������ ������
			if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
				((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_INVEN_POS)
			{
				m_bSelectItem = TRUE;
			}

			// ����â ������ ����
			if( m_pWearDisplayInfo[POS_PROW] && pt.x>POS_PROW_X && pt.x<POS_PROW_X+INVEN_SLOT_SIZE &&
				pt.y>nWindowPosY+POS_PROW_Y && pt.y<nWindowPosY+POS_PROW_Y+INVEN_SLOT_SIZE)
			{
				// ���̴�
				if(g_pGameMain->m_pTrade->m_bTrading)
				{
					g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_TRADE_0004, _MESSAGE );//"������ �������� �ŷ� �� �� �����ϴ�."
					return INF_MSGPROC_BREAK;
				}
				m_nRenderMoveIconIntervalWidth  = pt.x - POS_PROW_X;
				m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY+POS_PROW_Y);

				SetSelectItem(m_pWearDisplayInfo[POS_PROW]);
				m_bSelectWearItem = TRUE;
			}
			else if( m_pWearDisplayInfo[POS_PROWIN] && pt.x>POS_PROWIN_X && pt.x<POS_PROWIN_X+INVEN_SLOT_SIZE &&
				pt.y>nWindowPosY+POS_PROWIN_Y && pt.y<nWindowPosY+POS_PROWIN_Y+INVEN_SLOT_SIZE)
			{
				// ��ǻ�� ������ 
				if(g_pGameMain->m_pTrade->m_bTrading)
				{
					g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_TRADE_0004, _MESSAGE );
					return INF_MSGPROC_BREAK;
				}
				m_nRenderMoveIconIntervalWidth  = pt.x - POS_PROWIN_X;
				m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY+POS_PROWIN_Y);

				SetSelectItem(m_pWearDisplayInfo[POS_PROWIN]);
				m_bSelectWearItem = TRUE;
			}
			else if( m_pWearDisplayInfo[POS_PROWOUT] && pt.x>POS_PROWOUT_X && pt.x<POS_PROWOUT_X+INVEN_SLOT_SIZE &&
				pt.y>nWindowPosY+POS_PROWOUT_Y && pt.y<nWindowPosY+POS_PROWOUT_Y+INVEN_SLOT_SIZE)
			{
				// 1��������
				if(g_pGameMain->m_pTrade->m_bTrading)
				{
					g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_TRADE_0004, _MESSAGE );
					return INF_MSGPROC_BREAK;
				}
				m_nRenderMoveIconIntervalWidth  = pt.x - POS_PROWOUT_X;
				m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY+POS_PROWOUT_Y);

				SetSelectItem(m_pWearDisplayInfo[POS_PROWOUT]);
				m_bSelectWearItem = TRUE;
			}
			// 2006-07-20 by dgwoo �߰�
			else if( m_pWearDisplayInfo[POS_WINGIN] && pt.x>POS_WINGIN_X && pt.x<POS_WINGIN_X+INVEN_SLOT_SIZE &&
				pt.y>nWindowPosY+POS_WINGIN_Y && pt.y<nWindowPosY+POS_WINGIN_Y+INVEN_SLOT_SIZE)
			{
				// ��ü ����� ���� ������
				if(g_pGameMain->m_pTrade->m_bTrading)
				{
					g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_TRADE_0004,_MESSAGE);
					return INF_MSGPROC_BREAK;
				}
				m_nRenderMoveIconIntervalWidth  = pt.x - POS_WINGIN_X;
				m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY+POS_WINGIN_Y);

				SetSelectItem(m_pWearDisplayInfo[POS_WINGIN]);
				m_bSelectWearItem = TRUE;
			}
			else if( m_pWearDisplayInfo[POS_WINGOUT] && pt.x>POS_WINGOUT_X && pt.x<POS_WINGOUT_X+INVEN_SLOT_SIZE &&
				pt.y>nWindowPosY+POS_WINGOUT_Y && pt.y<nWindowPosY+POS_WINGOUT_Y+INVEN_SLOT_SIZE)
			{
				// 2�� ������
				if(g_pGameMain->m_pTrade->m_bTrading)
				{
					g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_TRADE_0004, _MESSAGE );
					return INF_MSGPROC_BREAK;
				}
				m_nRenderMoveIconIntervalWidth  = pt.x - POS_WINGOUT_X;
				m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY+POS_WINGOUT_Y);

				SetSelectItem(m_pWearDisplayInfo[POS_WINGOUT]);
				m_bSelectWearItem = TRUE;
			}
			else if( m_pWearDisplayInfo[POS_CENTER] && pt.x>POS_CENTER_X && pt.x<POS_CENTER_X+INVEN_SLOT_SIZE &&
				pt.y>nWindowPosY+POS_CENTER_Y && pt.y<nWindowPosY+POS_CENTER_Y+INVEN_SLOT_SIZE)
			{
				// �Ƹӷ�
				if(g_pGameMain->m_pTrade->m_bTrading)
				{
					g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_TRADE_0004, _MESSAGE );
					return INF_MSGPROC_BREAK;
				}
				m_nRenderMoveIconIntervalWidth  = pt.x - POS_CENTER_X;
				m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY+POS_CENTER_Y);

				SetSelectItem(m_pWearDisplayInfo[POS_CENTER]);
				m_bSelectWearItem = TRUE;
			}
			else if( m_pWearDisplayInfo[POS_REAR] && pt.x>POS_REAR_X && pt.x<POS_REAR_X+INVEN_SLOT_SIZE &&
				pt.y>nWindowPosY+POS_REAR_Y && pt.y<nWindowPosY+POS_REAR_Y+INVEN_SLOT_SIZE)
			{
				// ����������
				if(g_pGameMain->m_pTrade->m_bTrading)
				{
					g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_TRADE_0004, _MESSAGE );
					return INF_MSGPROC_BREAK;
				}
				m_nRenderMoveIconIntervalWidth  = pt.x - POS_REAR_X;
				m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY+POS_REAR_Y);

				SetSelectItem(m_pWearDisplayInfo[POS_REAR]);
				m_bSelectWearItem = TRUE;
			}
			else if( m_pWearDisplayInfo[POS_ACCESSORY_UNLIMITED] && pt.x>POS_ATTACHMENT_X && pt.x<POS_ATTACHMENT_X+INVEN_SLOT_SIZE &&
				pt.y>nWindowPosY+POS_ATTACHMENT_Y && pt.y<nWindowPosY+POS_ATTACHMENT_Y+INVEN_SLOT_SIZE)
			{
				// ������ 
				if(g_pGameMain->m_pTrade->m_bTrading)
				{
					g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_TRADE_0004, _MESSAGE );
					return INF_MSGPROC_BREAK;
				}
				m_nRenderMoveIconIntervalWidth  = pt.x - POS_ATTACHMENT_X;
				m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY+POS_ATTACHMENT_Y);

				SetSelectItem(m_pWearDisplayInfo[POS_ACCESSORY_UNLIMITED]);
				m_bSelectWearItem = TRUE;
			}
			// 2006-03-30 by ispark
			else if( m_pWearDisplayInfo[POS_ACCESSORY_TIME_LIMIT] && pt.x>POS_PET_X && pt.x<POS_PET_X+INVEN_SLOT_SIZE &&
				pt.y>nWindowPosY+POS_PET_Y && pt.y<nWindowPosY+POS_PET_Y+INVEN_SLOT_SIZE)
			{
				// �Ǽ��縮
				if(g_pGameMain->m_pTrade->m_bTrading)
				{
					g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_TRADE_0004, _MESSAGE );
					return INF_MSGPROC_BREAK;
				}
				m_nRenderMoveIconIntervalWidth  = pt.x - POS_PET_X;
				m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY+POS_PET_Y);

				SetSelectItem(m_pWearDisplayInfo[POS_ACCESSORY_TIME_LIMIT]);
				m_bSelectWearItem = TRUE;
			}
//			else
//			{
//				if(m_bSelectItem)
//				{
//					int i = (pt.x - g_pGameMain->m_pQuickSlot->m_nX - 1)/QSLOT_ICON_INTERVAL;
//					if( i >= 0 && 
//						i < QSLOT_NUMBER ) 
//					{
//						SetSelectItem(NULL);
//					}
//					
//					m_bSelectItem = FALSE;
//				}
//			}

			// ������â ������ ����
			int i = (pt.x-INVEN_ITEM_SLOT_START_X)/INVEN_SLOT_INTERVAL;
			if( pt.x > INVEN_ITEM_SLOT_START_X &&
				i >= 0 && i < INVEN_X_NUMBER &&
				pt.x < INVEN_ITEM_SLOT_START_X+INVEN_SLOT_INTERVAL*i+INVEN_SLOT_SIZE)
			{
				int j = (pt.y - nWindowPosY - INVEN_ITEM_SLOT_START_Y)/INVEN_SLOT_INTERVAL;
				if( j >= 0 && 
					j < INVEN_Y_NUMBER &&
					m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i] && pt.y > nWindowPosY + INVEN_ITEM_SLOT_START_Y &&
					pt.y < nWindowPosY + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*j + INVEN_SLOT_SIZE)
				{
					m_nRenderMoveIconIntervalWidth  = pt.x - (INVEN_ITEM_SLOT_START_X+(INVEN_SLOT_INTERVAL*i));
					m_nRenderMoveIconIntervalHeight = pt.y - (nWindowPosY + INVEN_ITEM_SLOT_START_Y + (INVEN_SLOT_INTERVAL*j));
					m_bSelectWearItem = FALSE;

					if(m_bEnchantState && m_pEnchantItem)
					{
						m_bEnchantState = FALSE;
						((CINFGameMain*)m_pParent)->m_pInfWindow->DeleteMsgBox(_Q_USE_ENCHANT_ITEM_CARD);
					}
					else
					{
						if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem == NULL)						
						{
							SetSelectItem(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]);
						}
					}
				}
			}

			// 2005-08-05 by ispark
			// â�ȿ��� ���콺 Ŭ���� ��ȿ
			// �켱 ���� ����
			if( pt.x < INVEN_FULL_BACK_W &&
				pt.y>nWindowPosY && 
				pt.y < nWindowPosY+INVEN_FULL_BACK_H)
			{
				g_pInterface->SetWindowOrder(WNDLeftRightWindow);
				return INF_MSGPROC_BREAK;
			}

			SetItemInfo(NULL, 0, 0);		
			
			// 2006-07-26 by ispark, ���� �������� �ִٸ� ���콺 Ŭ���� ��ȿ
			if(m_bSelectItem)
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
			int nWindowPosY = g_pGameMain->m_nLeftWindowY;
			m_bMouseDownState = FALSE;

			m_bScrollLock = FALSE;
			if( g_pGameMain->m_pQuickSlot->m_nItemType == QSLOT_ITEMTYPE_SKILL || 
				g_pGameMain->m_pQuickSlot->m_nItemType == QSLOT_ITEMTYPE_ITEM)
			{
				// ������ ���� �����ڵ�
				// 2006-05-30 by ispark
				break;
			}

			CItemInfo* pSelectItem = NULL;
			if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem)
			{
				pSelectItem = (CItemInfo*)(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem); 
			}

			// 2006-07-26 by ispark, �κ��� �������� �ƴ϶�� ������.
			if(((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType != ITEM_INVEN_POS)
			{
				break;
			}

			// 2005-12-01 by ispark, �����Կ��� LBUTTONUP�� ��
			if(g_pGameMain->m_pQuickSlot->LButtonUpQuickSlot(pt))
			{
				// �����Կ����� SelectItem ���� ����
				break;
			}
			// ������ -> ����â
			else if(pSelectItem && pt.x>16 && pt.x<295 && pt.y>nWindowPosY + 32 && pt.y<nWindowPosY + 202)
			{
				if( pt.x>POS_PROW_X && pt.x<POS_PROW_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+POS_PROW_Y && pt.y<nWindowPosY+POS_PROW_Y+INVEN_SLOT_SIZE)
				{
					SendChangeWearWindowPos(POS_PROW);
				}
				else if( pt.x>POS_PROWIN_X && pt.x<POS_PROWIN_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+POS_PROWIN_Y && pt.y<nWindowPosY+POS_PROWIN_Y+INVEN_SLOT_SIZE)
				{
					SendChangeWearWindowPos(POS_PROWIN);
				}
				else if( pt.x>POS_PROWOUT_X && pt.x<POS_PROWOUT_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+POS_PROWOUT_Y && pt.y<nWindowPosY+POS_PROWOUT_Y+INVEN_SLOT_SIZE)
				{
					SendChangeWearWindowPos(POS_PROWOUT);
				}
				else if( pt.x>POS_WINGIN_X && pt.x<POS_WINGIN_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+POS_WINGIN_Y && pt.y<nWindowPosY+POS_WINGIN_Y+INVEN_SLOT_SIZE)
				{
					// 2006-07-20 by dgwoo
					SendChangeWearWindowPos(POS_WINGIN);
				}
				else if( pt.x>POS_WINGOUT_X && pt.x<POS_WINGOUT_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+POS_WINGOUT_Y && pt.y<nWindowPosY+POS_WINGOUT_Y+INVEN_SLOT_SIZE)
				{
					SendChangeWearWindowPos(POS_WINGOUT);
				}
				else if( pt.x>POS_CENTER_X && pt.x<POS_CENTER_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+POS_CENTER_Y && pt.y<nWindowPosY+POS_CENTER_Y+INVEN_SLOT_SIZE)
				{
					SendChangeWearWindowPos(POS_CENTER);
				}
				else if( pt.x>POS_REAR_X && pt.x<POS_REAR_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+POS_REAR_Y && pt.y<nWindowPosY+POS_REAR_Y+INVEN_SLOT_SIZE)
				{
					SendChangeWearWindowPos(POS_REAR);
				}
				else if( pt.x>POS_ATTACHMENT_X && pt.x<POS_ATTACHMENT_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+POS_ATTACHMENT_Y && pt.y<nWindowPosY+POS_ATTACHMENT_Y+INVEN_SLOT_SIZE)
				{
					SendChangeWearWindowPos(POS_ACCESSORY_UNLIMITED);
				}
				// 2006-03-30 by ispark, �Ǽ��縮
				else if( pt.x>POS_PET_X && pt.x<POS_PET_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+POS_PET_Y && pt.y<nWindowPosY+POS_PET_Y+INVEN_SLOT_SIZE)
				{
					SendChangeWearWindowPos(POS_ACCESSORY_TIME_LIMIT);
				}
			}
			else if(pSelectItem && pt.x>INVEN_ITEM_SLOT_START_X && pt.x<INVEN_ITEM_SLOT_START_X+INVEN_SLOT_INTERVAL*INVEN_X_NUMBER && 
				pt.y>nWindowPosY+INVEN_ITEM_SLOT_START_Y && pt.y<nWindowPosY+INVEN_ITEM_SLOT_START_Y+INVEN_SLOT_INTERVAL*INVEN_Y_NUMBER)
			{
				int i = (pt.x-INVEN_ITEM_SLOT_START_X)/INVEN_SLOT_INTERVAL;
				int j = (pt.y - nWindowPosY - INVEN_ITEM_SLOT_START_Y)/INVEN_SLOT_INTERVAL;
				int nWindowPosition = j*INVEN_X_NUMBER+i+m_nInvenCurrentScroll*INVEN_X_NUMBER+POS_ITEMWINDOW_OFFSET;
				if(	pSelectItem->ItemWindowIndex
					&& nWindowPosition != pSelectItem->ItemWindowIndex &&
					pSelectItem->ItemWindowIndex >= POS_ITEMWINDOW_OFFSET &&
					pt.x > INVEN_ITEM_SLOT_START_X &&
					i >= 0 && i < INVEN_X_NUMBER &&
					pt.x < INVEN_ITEM_SLOT_START_X+INVEN_SLOT_INTERVAL*i+INVEN_SLOT_SIZE &&
					pt.y > nWindowPosY + INVEN_ITEM_SLOT_START_Y &&
					j >= 0 && j < INVEN_Y_NUMBER &&
					pt.y < nWindowPosY + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*j + INVEN_SLOT_SIZE )
				{	// ������ -> ������
					if( pSelectItem->ItemWindowIndex < nWindowPosition )
					{
						INVEN_DISPLAY_INFO* pSelectInvenDisplayInfo = m_pInvenDisplayInfo[pSelectItem->ItemWindowIndex];
						for(int i=pSelectItem->ItemWindowIndex+1; i<= nWindowPosition; i++)
						{
							CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(i);
							if(it != g_pStoreData->m_mapItemWindowPosition.end())
							{
								CItemInfo* pSwapItem = it->second;
								pSwapItem->ItemWindowIndex = i-1;
								g_pStoreData->m_mapItemWindowPosition[i-1] = pSwapItem;
								SetSingleInvenIconInfo(pSwapItem);
							} 
							else 
							{
								pSelectItem->ItemWindowIndex = i-1;
								g_pStoreData->m_mapItemWindowPosition[i-1] = pSelectItem;
								SetAllIconInfo();
								SetSelectItem(NULL);
								return INF_MSGPROC_NORMAL;
							}
						}
						pSelectItem->ItemWindowIndex = nWindowPosition;
						g_pStoreData->m_mapItemWindowPosition[nWindowPosition] = pSelectItem;
						SetSingleInvenIconInfo(pSelectItem);
						SetAllIconInfo();
					}
					else if( pSelectItem->ItemWindowIndex > nWindowPosition )
					{
						INVEN_DISPLAY_INFO* pSelectInvenDisplayInfo = m_pInvenDisplayInfo[pSelectItem->ItemWindowIndex];
						for(int i=pSelectItem->ItemWindowIndex-1; i >= nWindowPosition; i--)
						{
							CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(i);
							if(it != g_pStoreData->m_mapItemWindowPosition.end())
							{
								CItemInfo* pSwapItem = it->second;
								pSwapItem->ItemWindowIndex = i+1;
								g_pStoreData->m_mapItemWindowPosition[i+1] = pSwapItem;
								SetSingleInvenIconInfo(pSwapItem);
							} else {
								DBGOUT("ERROR : CINFInven::WndProc() Item List crashed!!!!!\n");
								SetSelectItem(NULL);
								return INF_MSGPROC_NORMAL;// ������ ����Ʈ�� �������
							}
						}
						pSelectItem->ItemWindowIndex = nWindowPosition;
						g_pStoreData->m_mapItemWindowPosition[nWindowPosition] = pSelectItem;
						SetSingleInvenIconInfo(pSelectItem);
						SetAllIconInfo();
					}
					// 2006-07-26 by ispark, ������
				}
				// ����â -> ������
				else if(pSelectItem->ItemWindowIndex >= 0 && pSelectItem->ItemWindowIndex < POS_ITEMWINDOW_OFFSET)
				{
					// 2004-12-10 by jschoi
					if(IsAbleReleaseItem(pSelectItem,pSelectItem->ItemWindowIndex))
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
						int count=0;

						// 2010-06-15 by shcho&hslee ��ý��� - ����ǰ �ִ� ���� ����.
						//for(int i=0;i<WEAR_ITEM_NUMBER;i++)
						for(int i=0;i<MAX_EQUIP_POS;i++)
						{
							if(m_pWearDisplayInfo[i])
								count++;
						}
						i=POS_ITEMWINDOW_OFFSET+g_pStoreData->m_mapItemWindowPosition.size();//-count-1;
						pMsg->ToItemWindowIndex = i;

						// ���� �������� �κ��丮�� ������ �ش� ��ų�� ���⼭ ������ ��..���� ��û
						if(pSelectItem)
						{
							g_pShuttleChild->m_pSkill->DeleteSkillFromWearItem(pSelectItem->Kind);
						}
						g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_CHANGE_WINDOW_POSITION));
//						g_pD3dApp->m_bRequestEnable = FALSE;			// 2006-06-19 by ispark, �޼��� ������ ��ٸ���.
					}
					else
					{
						char buf[128];
						wsprintf(buf,STRMSG_C_ITEM_0007);//"�䱸 �������� ���Ͽ� �ش� �������� ���� �� �� �����ϴ�."
						g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
					}
				}
			}
			// ������ -> �ı�
			else if(pSelectItem && pt.x>INVEN_GARBAGE_START_X && pt.x<INVEN_GARBAGE_START_X+INVEN_GARBAGE_SIZE &&
				pt.y>nWindowPosY+INVEN_GARBAGE_START_Y && pt.y<nWindowPosY+INVEN_GARBAGE_START_Y + INVEN_GARBAGE_SIZE)
			{
				if( !((CINFGameMain*)m_pParent)->m_pInfWindow->IsExistMsgBox(_Q_ITEM_DELETE) &&
					!((CINFGameMain*)m_pParent)->m_pInfWindow->IsExistMsgBox(_Q_ITEM_DELETE_NUM))
				{
					char buf[256];
					ITEM *item = g_pDatabase->GetServerItemInfo(pSelectItem->ItemNum);
					if(item)
					{
						if(pSelectItem->ItemWindowIndex < POS_ITEMWINDOW_OFFSET)
						{
							((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox(STRMSG_C_ITEM_0008, _MESSAGE);//"������ �������� ���� �� �����ϴ�."
							pSelectItem = NULL;
						}
						else
						{
							if(IS_COUNTABLE_ITEM(pSelectItem->Kind) && pSelectItem->CurrentCount>1)
							{
								wsprintf(buf, STRMSG_C_ITEM_0009, item->ItemName);//"������ %s ��� �����ðڽ��ϱ�?"
								((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox(buf, 
									_Q_ITEM_DELETE_NUM, (DWORD)pSelectItem, pSelectItem->CurrentCount);
							}
							else
							{
								wsprintf(buf, STRMSG_C_ITEM_0010, item->ItemName);//"������ %s ��(��)  �����ðڽ��ϱ�?"
								((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox(buf, _Q_ITEM_DELETE);
							}

							// 2007-06-20 by bhsohn ������ ������, �޸� ���� ����
							//m_pDeleteItem = pSelectItem;
							SetDeleteItemInfo(pSelectItem);							
						}
						
					}
				}
			}
// 2006-07-26 by ispark, ������ ������
//			if(m_bSelectWearItem && m_bSelectItem == FALSE)
//			{				
//				SetSelectItem(NULL);
//				((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
//				SetItemInfo(NULL, 0, 0);
//				break;
//			}
//
//			if(m_bSelectItem && m_bSelectWearItem == FALSE)
//				m_bSelectItem = FALSE;
//			else if(m_bSelectWearItem == FALSE)
//				SetSelectItem(NULL);
			if(m_bSelectItem)
			{				
				SetSelectItem(NULL);
				m_bSelectItem = FALSE;
				m_bSelectWearItem = FALSE;
				break;
			}
			
			((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
			SetItemInfo(NULL, 0, 0);
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			if(((CINFGameMain*)m_pParent)->m_pTrade->m_bTrading)
				return INF_MSGPROC_NORMAL;
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nWindowPosY = g_pGameMain->m_nLeftWindowY;
//			int nWindowPosY = POS_LEFT_WINDOW_Y;
//			if(nWindowPosY < SIZE_ROLL_Y )
//				nWindowPosY = SIZE_ROLL_Y;
//			if(nWindowPosY > LEFT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y)
//				nWindowPosY = LEFT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y;
			int i = (pt.x-INVEN_ITEM_SLOT_START_X)/INVEN_SLOT_INTERVAL;
			if( pt.x > INVEN_ITEM_SLOT_START_X &&
				i >= 0 && i < INVEN_X_NUMBER &&
				pt.x < INVEN_ITEM_SLOT_START_X+INVEN_SLOT_INTERVAL*i+INVEN_SLOT_SIZE)
			{
				int j = (pt.y - nWindowPosY - INVEN_ITEM_SLOT_START_Y)/INVEN_SLOT_INTERVAL;
				if( pt.y > nWindowPosY + INVEN_ITEM_SLOT_START_Y &&
					j >= 0 && j < INVEN_Y_NUMBER &&
					pt.y < nWindowPosY + INVEN_ITEM_SLOT_START_Y + INVEN_SLOT_INTERVAL*j + INVEN_SLOT_SIZE)
				{
					// 2007-06-20 by bhsohn ������ ������, �޸� ���� ����
					//if(!m_pDeleteItem && m_pInvenDisplayInfo[(j*INVEN_X_NUMBER)+i])
					if(0 == m_struDeleteItem.UniqueNumber && m_pInvenDisplayInfo[(j*INVEN_X_NUMBER)+i])
					{
						// set tooltip
						ITEM_BASE* pItem = m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]->pItem;
						ITEM *item = g_pDatabase->GetServerItemInfo( pItem->ItemNum);
						if(item)
						{
							// 2008-06-03 by dgwoo ������ �����ִ� ���¿��� ����Ŭ������ ������ ��ü�� ����.
							if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC())
							{
								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080603_0100,COLOR_ERROR);
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
										SendUseItem(pItem);
									}
									break;
								case POS_PROWIN:
								case POS_PROWOUT:
								case POS_WINGIN:
								case POS_WINGOUT:
								case POS_PROW:
								case POS_CENTER:
								case POS_REAR:
								case POS_ACCESSORY_UNLIMITED:
								case POS_ACCESSORY_TIME_LIMIT :	// 2006-03-31 by ispark
								case POS_PET:					// 2010-06-15 by shcho&hslee ��ý��� - ���� ����.
									{
										SetSelectItem(m_pInvenDisplayInfo[j*INVEN_X_NUMBER+i]);
										SendChangeWearWindowPos(item->Position);
									}
									break;
								default:
									{
										char buf[128];
										wsprintf(buf,STRMSG_C_ITEM_0011,pItem->ItemNum);//"item [ %08d ] �� ���� �� �� ���� �������Դϴ�."
										g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
									}
								}
							}
						}
						SetSelectItem(NULL);
						((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
					}
				}
			}
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

void CINFInven::DeleteSelectItem(int count)
{
	FLOG( "CINFInven::DeleteSelectItem(int count)" );
	// 2007-06-20 by bhsohn ������ ������, �޸� ���� ����
//	if(m_pDeleteItem )//&& m_pDeleteItem->Count >= count)
//	{
//		if(IS_COUNTABLE_ITEM(m_pDeleteItem->Kind) && m_pDeleteItem->CurrentCount < count)
//		{
//			m_pDeleteItem = NULL;
//			return;
//		}		
//		MSG_FC_ITEM_THROW_AWAY_ITEM sMsg;
//		memset(&sMsg,0x00,sizeof(sMsg));
//		char buffer[SIZE_MAX_PACKET];
//		int nType = T_FC_ITEM_THROW_AWAY_ITEM;
//		sMsg.Amount = count;// ���� ���� �ʿ�
//		sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
//		sMsg.ItemUniqueNumber = m_pDeleteItem->UniqueNumber;
//		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//		g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
//	}
//	m_pDeleteItem = NULL;
	
	if(0 == m_struDeleteItem.UniqueNumber)
	{
		return;
	}
	
	CItemInfo* pDeleteItem = g_pStoreData->FindItemInInventoryByUniqueNumber( m_struDeleteItem.UniqueNumber );
	if(NULL == pDeleteItem)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRERR_ERROR_0025,COLOR_ERROR);// "�ش� �������� �����ϴ�."
		InitDeleteItemInfo();
		return;
	}
				
	if(IS_COUNTABLE_ITEM(pDeleteItem->Kind) && pDeleteItem->CurrentCount < count)
	{
		//m_pDeleteItem = NULL;
		InitDeleteItemInfo();
			return;
	}		
	MSG_FC_ITEM_THROW_AWAY_ITEM sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	int nType = T_FC_ITEM_THROW_AWAY_ITEM;
	sMsg.Amount = count;// ���� ���� �ʿ�
	sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
	sMsg.ItemUniqueNumber = pDeleteItem->UniqueNumber;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
		
	InitDeleteItemInfo();
	// end 2007-06-20 by bhsohn ������ ������, �޸� ���� ����
}

void CINFInven::SendUseItem(ITEM_BASE* pItem)
{
	FLOG( "CINFInven::SendUseItem(ITEM_BASE* pItem)" );
	switch(pItem->Kind)
	{
	case ITEMKIND_ENERGY:
		{
			bool bSend = false;
			ITEM * pIteminfo = g_pDatabase->GetServerItemInfo(pItem->ItemNum);
			CAppEffectData * pEff = NULL; 
			// 2009-04-21 by bhsohn ������ DesParam�߰�
			//switch(pIteminfo->DestParameter1)
			switch(pIteminfo->ArrDestParameter[0])
			{
			case DES_CASH_STAT_ALL_INITIALIZE:
				{// ��ü ���� �ʱ�ȭ
					MSG_FC_ITEM_USE_ENERGY sMsg;
					sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
					sMsg.ItemUniqueNumber = pItem->UniqueNumber;
					g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_ENERGY, (char*)&sMsg, sizeof(sMsg) );
					((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
					bSend = true;
				}
				break;
			case DES_CASH_STAT_PART_INITIALIZE:
				{// �κ� ���� �ʱ�ȭ
					// 2005-03-28 by jschoi
					bSend = true;
					// 2006-04-17 by ispark, ���� �޼����� �ִ��� Ȯ���ؼ� ó���Ѵ�.
					if(g_pGameMain->m_pInfWindow->SearchMsgBox(_Q_PART_STAT_RESET))
						break;
					g_pGameMain->m_pInfWindow->RadioClear();
					g_pGameMain->m_pInfWindow->AddRadioOption(STRMSG_C_STAT_0001);//"����"
					g_pGameMain->m_pInfWindow->AddRadioOption(STRMSG_C_050517_0001);//"���"
					g_pGameMain->m_pInfWindow->AddRadioOption(STRMSG_C_STAT_0003);//"����"
					g_pGameMain->m_pInfWindow->AddRadioOption(STRMSG_C_STAT_0004);//"����"
					g_pGameMain->m_pInfWindow->AddRadioOption(STRMSG_C_STAT_0005);//"ȸ��"
					g_pGameMain->m_pInfWindow->AddRadioOption(STRMSG_C_STAT_0006);//"����"

					g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_STAT_0007,_Q_PART_STAT_RESET,0,0,0,pItem->UniqueNumber);//"�ʱ�ȭ�� ���ϴ� ������ �����ϼ���."

				}
				break;
			case DES_GRADUAL_EP_UP:		// ��������, ���� EP ���� �ð�(ITEM.Time�� ����)���� ���������� VALUE��ŭ �÷���, �߰��� ��(?)������ cancel
			case DES_IN_TIME_EP_UP:		// ��������, ���� EP ���� �ð�(ITEM.Time�� ����)�� ���� �� VALUE��ŭ �÷���
			case DES_IMMEDIATE_EP_UP:	// ��������, ���� EP ��� VALUE��ŭ �÷���
				{
//					if(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.EP > g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CurrentEP)
//					{
//						// ���� ���� ����
//						g_pD3dApp->m_pSound->PlayD3DSound(SOUND_USEITEM_REPAIR_HP, g_pShuttleChild->m_vPos, FALSE);
//						pEff = new CAppEffectData(g_pD3dApp->m_pShuttleChild,RC_EFF_USEITEM_REPAIR_EP,D3DXVECTOR3(0,0,0));
//						pEff = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pEff);
//						MSG_FC_CHARACTER_SHOW_EFFECT sMsg;
//						sMsg.EffectIndex = RC_EFF_USEITEM_REPAIR_EP;
//						g_pFieldWinSocket->SendMsg( T_FC_CHARACTER_SHOW_EFFECT, (char*)&sMsg, sizeof(sMsg) );
//					}
//					else
//					{
//						bSend = true;
//					}					
				}
				break;
			case DES_GRADUAL_SP_UP:		// ��������, ���� SP�� ���� �ð�(ITEM.Time�� ����)���� ���������� VALUE��ŭ �÷���, �߰��� ��(?)������ cancel
			case DES_IN_TIME_SP_UP:		// ��������, ���� SP�� ���� �ð�(ITEM.Time�� ����)�� ���� �� VALUE��ŭ �÷���
			case DES_IMMEDIATE_SP_UP:	// ��������, ���� SP�� ��� VALUE��ŭ �÷���
				break;
			case DES_GRADUAL_HP_UP:		// ��������, ���� HP�� ���� �ð�(ITEM.Time�� ����)���� ���������� VALUE��ŭ �÷���, �߰��� �ν��� �Ѱų� ���� ������ cancel
			case DES_IN_TIME_HP_UP:		// ��������, ���� HP�� ���� �ð�(ITEM.Time�� ����)�� ���� �� VALUE��ŭ �÷���
			case DES_IMMEDIATE_HP_UP:	// ��������, ���� HP�� ��� VALUE��ŭ �÷���
				{
//					if(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.HP > g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CurrentHP)
//					{
//						int nEffNum = RC_EFF_USEITEM_REPAIR_HP;
//						switch(pItem->ItemNum)
//						{
//						case  ITEM_NUM_APLUS_GRADE_HP_KIT:		nEffNum = RC_EFF_USEITEM_REPAIR_HPA;	break;
//						case  ITEM_NUM_S_GRADE_HP_KIT:			nEffNum = RC_EFF_USEITEM_REPAIR_HPS;	break;
//						}
//
//						g_pD3dApp->m_pSound->PlayD3DSound(SOUND_USEITEM_REPAIR_HP, g_pShuttleChild->m_vPos, FALSE);
//						pEff = new CAppEffectData(g_pD3dApp->m_pShuttleChild,nEffNum,D3DXVECTOR3(0,0,0));
//						pEff = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pEff);
//						MSG_FC_CHARACTER_SHOW_EFFECT sMsg;
//						sMsg.EffectIndex = nEffNum;
//						g_pFieldWinSocket->SendMsg( T_FC_CHARACTER_SHOW_EFFECT, (char*)&sMsg, sizeof(sMsg) );
//					}
//					else
//					{
//						bSend = true;
//					}
				}
				break;
			case DES_GRADUAL_DP_UP:		// ��������, ���� DP ���� �ð�(ITEM.Time�� ����)���� ���������� VALUE��ŭ �÷���, �߰��� ��(?)������ cancel
			case DES_IN_TIME_DP_UP:		// ��������, ���� DP ���� �ð�(ITEM.Time�� ����)�� ���� �� VALUE��ŭ �÷���
			case DES_IMMEDIATE_DP_UP:	// ��������, ���� DP ��� VALUE��ŭ �÷���
				{
//					if(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.DP > g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CurrentDP)
//					{
//						int nEffNum = RC_EFF_USEITEM_REPAIR_DP;
//						switch(pItem->ItemNum)
//						{
//						case  ITEM_NUM_APLUS_GRADE_DP_KIT:		nEffNum = RC_EFF_USEITEM_REPAIR_DPA;	break;
//						case  ITEM_NUM_S_GRADE_DP_KIT:			nEffNum = RC_EFF_USEITEM_REPAIR_DPS;	break;
//						}
//
//						g_pD3dApp->m_pSound->PlayD3DSound(SOUND_USEITEM_REPAIR_DP, g_pShuttleChild->m_vPos, FALSE);
//						pEff = new CAppEffectData(g_pD3dApp->m_pShuttleChild,nEffNum,D3DXVECTOR3(0,0,0));
//						pEff = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pEff);
//						MSG_FC_CHARACTER_SHOW_EFFECT sMsg;
//						sMsg.EffectIndex = nEffNum;
//						g_pFieldWinSocket->SendMsg( T_FC_CHARACTER_SHOW_EFFECT, (char*)&sMsg, sizeof(sMsg) );
//					}
//					else
//					{
//						bSend = true;
//					}
				}
				break;
			// 2007-12-05 by bhsohn ��Ż ����, ��� ���� ����
			case DES_WARP:
			case DES_WARP_OUTPOST:
				{				
					if(g_pShuttleChild 						
						&& g_pShuttleChild->m_pClientParty->IsFormationFlight())
					{
						if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER )
						{
							// 2008-06-17 by bhsohn ��� ���� ó��
							// ������� ������������ ���� 
//							g_pShuttleChild->m_pClientParty->ISendPartyChangeFlightFormation(FLIGHT_FORM_NONE);
//							
//							PARTYINFO partyTmp = g_pShuttleChild->m_pClientParty->GetPartyInfo();
//							partyTmp.bFormationFlyingType = FLIGHT_FORM_NONE;
//							g_pShuttleChild->m_pClientParty->SetPartyInfo(partyTmp);

							// 2008-06-17 by bhsohn ��� ���� ó��
							// ������ ���� ��������
							g_pShuttleChild->m_pClientParty->TempPartyFormation(FLIGHT_FORM_NONE);
							// end 2008-06-17 by bhsohn ��� ���� ó��

						}
						else
						{
							// 2007-12-27 by bhsohn ��� ����� ������ ��Ż����ϰ� �ٽ� ���� �� �̻����ϴ� ���� ����
							// ��� ���� ����
							g_pShuttleChild->CheckFormationMoveDelete();
						}
						
					}					
				}
				break;
			// end 2007-12-05 by bhsohn ��Ż ����, ��� ���� ����
			}
			if(!bSend)
			{
				MSG_FC_ITEM_USE_ENERGY sMsg;
				sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
				sMsg.ItemUniqueNumber = pItem->UniqueNumber;
				g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_ENERGY, (char*)&sMsg, sizeof(sMsg) );
				((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
			}
		}
		break;
	case ITEMKIND_CARD:
		{
			ITEM * pIteminfo = g_pDatabase->GetServerItemInfo(pItem->ItemNum);
			// 2006-02-10 by ispark, ų��ũ ������ ��� �׳� ����
			// 2006-08-14 by ispark, ����
			// 2006-08-22 by ispark, ��Ʈ�� �� ������ ����
			// 2007-10-04 by bhsohn ų��ũ ����ġ ���� ó��
//#if defined(_DEBUG) || defined(LANGUAGE_KOREA) || defined(LANGUAGE_ENGLISH) || (defined(LANGUAGE_VIETNAM) && defined(_TEST_SERVER)) || defined(LANGUAGE_CHINA)
//#else
//			if(COMPARE_BIT_FLAG(pIteminfo->ItemAttribute, ITEM_ATTR_KILL_MARK_ITEM))
//			{
//				return;
//			}
//
//#endif
			if(NULL == pIteminfo)
			{
				return;
			}
			// end 2007-10-04 by bhsohn ų��ũ ����ġ ���� ó��
			// 2009-04-21 by bhsohn ������ DesParam�߰�
			//switch(pIteminfo->DestParameter1)
			switch(pIteminfo->ArrDestParameter[0])
			{	
			case DES_CASH_NORMAL_RESTORE:	// �Ϲ� ��Ȱ ī��
				{
					// ������ ��Ȱ ī�� ����Ѵ�.
					MSG_FC_ITEM_USE_CARDITEM_RESTORE sMsg;
					memset(&sMsg,0x00,sizeof(sMsg));
					sMsg.ItemUniqueNumber = pItem->UniqueNumber;
					g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_CARDITEM_RESTORE, (char*)&sMsg, sizeof(sMsg) );
				}
				break;
			case DES_CASH_SUPER_RESTORE:	// ���� ��Ȱ ī��
				{
					// ���� ������ ���������� ��Ȱ ī�带 ����Ѵ�.
					if(g_pGameMain && g_pGameMain->m_pInfWindow)
					{
						g_pGameMain->m_bChangeMousePoint = TRUE;
						g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_CARD_0006, _Q_SUPER_RESTORE,0,0,0,pItem->UniqueNumber );//"��Ȱ�� ���ϴ� ������ �Է��Ͻÿ�."
					}			
				}
				break;
			case DES_CASH_GUILD_ALL_MEMBERS_SUMMON:	// ���ܿ� ��ü ��ȯ ī��
				{
					// ���ܿ� ��ü ��ȯ
					MSG_FC_ITEM_USE_CARDITEM_GUILDSUMMON sMsg;
					memset(&sMsg,0x00,sizeof(sMsg));
					sMsg.ItemUniqueNumber = pItem->UniqueNumber;
					g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_CARDITEM_GUILDSUMMON, (char*)&sMsg, sizeof(sMsg) );
				}
				break;
			case DES_CASH_GUILD_MEMBER_SUMMON:		// ���ܿ� �� �� ��ȯ ī��
				{
					// ���õ� ���ܿ� ��ȯ	
					if(g_pGameMain && g_pGameMain->m_pInfWindow)
					{
						// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
						//g_pGameMain->m_pCommunity->SetCommunityType(2);
						//g_pGameMain->m_pCommunity->SetCommunityType(COMMUNITY_GUILD);						
						
						//g_pGameMain->LeftWindowShow(TRUE, LEFT_WINDOW_PARTY);
						//g_pGameMain->OnClickCommunity();
						//g_pGameMain->m_bChangeMousePoint = TRUE;

						g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_CARD_0007, _Q_MEMBER_SUMMON,0,0,0,pItem->UniqueNumber );//"��ȯ�� ���ϴ� ������ �Է��Ͻÿ�."
					}
				}
				break;
			case DES_CASH_GUILD:
				{
					if(g_pGameMain && g_pGameMain->m_pInfWindow)
					{
						// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
						//g_pGameMain->m_pCommunity->SetCommunityType(2);
						//g_pGameMain->m_pCommunity->SetCommunityType(COMMUNITY_GUILD);

						
						//g_pGameMain->LeftWindowShow(TRUE, LEFT_WINDOW_PARTY);
						//g_pGameMain->OnClickCommunity();
						//g_pGameMain->m_bChangeMousePoint = TRUE;

						MSG_FC_ITEM_USE_CARDITEM_GUILD sMsg;
						sMsg.ItemUniqueNumber = pItem->UniqueNumber;
						g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_CARDITEM_GUILD, (char*)&sMsg, sizeof(sMsg) );
					}
				}
				break;
			case DES_CASH_MONSTER_SUMMON:
				{
					MSG_FC_ITEM_USE_CARDITEM_MONSTERSUMMON sMsg;
					sMsg.ItemUniqueNumber = pItem->UniqueNumber;
					g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_CARDITEM_MONSTERSUMMON, (char*)&sMsg, sizeof(sMsg) );
				}
				break;
			case DES_CASH_CHANGE_CHARACTERNAME:
				{
					g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_CARD_0008, _Q_USEITEM_NAME_CHANGE,0,0,0, pItem->UniqueNumber);	//"��ĳ���� �̸��� �Է��� �ֽʽÿ�(\\r����� �α׿��� �˴ϴ�\\r)"
				}
				break;
			case DES_KILLMARK_EXP:
				{
					char chMsg[256] = {0,};
					// 2008-12-30 by bhsohn ų��ũ ���� ��Ʈ�� ����
					//wsprintf(chMsg, STRMSG_C_060814_0001, pItem->ItemInfo->ItemName);
					wsprintf(chMsg, STRMSG_C_081230_0207, pItem->ItemInfo->ItemName);
					g_pGameMain->m_pInfWindow->AddMsgBox( chMsg, _Q_USEITEM_KILL_MARK,0,0,0, pItem->UniqueNumber);	// "%s ų ��ũ�� EXP�� ��ȯ�ϰڽ��ϱ�?"
				}
				break;
				// 2007-08-07 by bhsohn ����Ŀ ������ �߰�
			case DES_CHAT_ALL_INFLUENCE:
				{	
					// ����Ŀ ������ â ���
					DoModalChatAllInfluence(pItem->UniqueNumber);
				}
				break;
				// end 2007-08-07 by bhsohn ����Ŀ ������ �߰�
			// 2007-11-21 by dgwoo ĳ���� ���� ī��.
			case DES_CASH_CHANGE_PILOTFACE:
				{
					if(g_pD3dApp->m_bCharacter)
					{
						g_pGameMain->m_pChangeCharactor->SetActive(TRUE,pItem->UniqueNumber);
					}
					else
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_071121_0100,COLOR_SYSTEM);//"�ش� �������� ĳ���� ���¿����� ����� �� �ֽ��ϴ�."						
					}
				}
				break;

			default:
				{
					MSG_FC_ITEM_USE_ITEM sMsg;
					memset(&sMsg,0x00,sizeof(sMsg));
					char buffer[SIZE_MAX_PACKET];
					sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
					sMsg.ItemUniqueNumber = pItem->UniqueNumber;
					int nType = T_FC_ITEM_USE_ITEM;
					memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
					memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
					g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
					((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
				}
				break;
			}
		}
		break;
	case ITEMKIND_INFLUENCE_BUFF:
		{
			// 2006-04-24 by ispark
			MSG_FC_ITEM_USE_INFLUENCE_BUFF sMsg;
			sMsg.ItemUniqueNumber = pItem->UniqueNumber;
			g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_INFLUENCE_BUFF, (char*)&sMsg, sizeof(sMsg) );
		}
		break;
	case ITEMKIND_INFLUENCE_GAMEEVENT:
		{
			// 2006-04-24 by ispark
			MSG_FC_ITEM_USE_INFLUENCE_GAMEEVENT sMsg;
			sMsg.ItemUniqueNumber = pItem->UniqueNumber;
			g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_INFLUENCE_GAMEEVENT, (char*)&sMsg, sizeof(sMsg) );
		}
		break;
	case ITEMKIND_BULLET:
		{
//			if(g_pD3dApp->m_pShuttleChild->m_pItemPrimary || g_pD3dApp->m_pShuttleChild->m_pItemSecondary)
			if(g_pShuttleChild->m_pPrimaryWeapon || g_pShuttleChild->m_pSecondaryWeapon)
			{
				bool bSend = false;
				// 2007-08-07 by bhsohn �Ѿ� ������ 1�� á�� ��, 2�� ���� �ȵǴ� ���� ����
				BOOL bPrimaryFullWeapon, bSecondaryFullWeapon;
				bPrimaryFullWeapon = bSecondaryFullWeapon = FALSE;
				// end 2007-08-07 by bhsohn �Ѿ� ������ 1�� á�� ��, 2�� ���� �ȵǴ� ���� ����

				CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(POS_PROWOUT);
				if(it != g_pStoreData->m_mapItemWindowPosition.end())
				{
					ITEM * p = it->second->GetItemInfo();//g_pDatabase->GetServerItemInfo(it->second->ItemNum);
					ITEM * p2 = g_pDatabase->GetServerItemInfo(pItem->ItemNum);
					// 2007-08-07 by dgwoo 1,2�� ��� ä��� �Ѿ� ������ RECHARGE_BULLET_ALL_CALIBER == p2->Caliber
					if( p && p2 && (RECHARGE_BULLET_ALL_CALIBER == p2->Caliber || p->Caliber == p2->Caliber))
					{
						// 2007-10-15 by bhsohn �Ѿ� ������ �߰� ó��
						int nMaxCharge = p->Charging;
						nMaxCharge = g_pGameMain->GetMaxBulletItem(p->Charging);

						//if( it->second->CurrentCount == p->Charging )
						if( it->second->CurrentCount == nMaxCharge )
						{
							// 2007-08-07 by bhsohn �Ѿ� ������ 1�� á�� ��, 2�� ���� �ȵǴ� ���� ����
							bPrimaryFullWeapon = TRUE;
							g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_ITEM_0012,COLOR_SYSTEM);//"�̹� �Ѿ��� �����Ǿ� �ֽ��ϴ�."
							//break;
						}
						else
						{
							bSend = true;
						}
					}
					//					if(p && p2 && p->Caliber == p2->Caliber && it->second->CurrentCount != p->Charging)
					//					{// �������� �˻�
					//						bSend = true;
					//					}
				}
				//if(!bSend)
				{
					it = g_pStoreData->m_mapItemWindowPosition.find(POS_WINGOUT);
					if(it != g_pStoreData->m_mapItemWindowPosition.end())
					{
						ITEM * p = it->second->GetItemInfo();//g_pDatabase->GetServerItemInfo(it->second->ItemNum);
						ITEM * p2 = g_pDatabase->GetServerItemInfo(pItem->ItemNum);
						// 2007-08-07 by dgwoo 1,2�� ��� ä��� �Ѿ� ������ RECHARGE_BULLET_ALL_CALIBER == p2->Caliber
						if( p && p2 && (RECHARGE_BULLET_ALL_CALIBER == p2->Caliber || p->Caliber == p2->Caliber))
						{
							// 2007-10-15 by bhsohn �Ѿ� ������ �߰� ó��
							int nMaxCharge = p->Charging;
							nMaxCharge = g_pGameMain->GetMaxBulletItem(p->Charging);

							//if( it->second->CurrentCount == p->Charging )
							if( it->second->CurrentCount == nMaxCharge )
							{
								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_ITEM_0013,COLOR_SYSTEM);//"�̹� ź�ΰ� �����Ǿ� �ֽ��ϴ�."
								// 2007-08-07 by bhsohn �Ѿ� ������ 1�� á�� ��, 2�� ���� �ȵǴ� ���� ����
								bSecondaryFullWeapon = TRUE;
								//break;
							}
							else
							{
								bSend = true;
							}
	//						if(p && p2 && p->Caliber == p2->Caliber && it->second->CurrentCount != p->Charging)
	//						{// �������� �˻�
	//							bSend = true;
	//						}
						}
					}
				}

				// 2007-08-07 by bhsohn �Ѿ� ������ 1�� á�� ��, 2�� ���� �ȵǴ� ���� ����
				bSend = true; 
				if((TRUE == bPrimaryFullWeapon)
					&&(TRUE == bSecondaryFullWeapon))
				{
					bSend = false;					
				}
				// end 2007-08-07 by bhsohn �Ѿ� ������ 1�� á�� ��, 2�� ���� �ȵǴ� ���� ����
				
				if(bSend)
				{
					MSG_FC_ITEM_USE_ITEM sMsg;
					memset(&sMsg,0x00,sizeof(sMsg));
					char buffer[SIZE_MAX_PACKET];
					sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
					sMsg.ItemUniqueNumber = pItem->UniqueNumber;
					int nType = T_FC_ITEM_USE_ITEM;
					memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
					memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
					g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
					((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
				}
				// 2007-08-07 by bhsohn �Ѿ� ������ 1�� á�� ��, 2�� ���� �ȵǴ� ���� ����
				else if((TRUE != bPrimaryFullWeapon)&&(TRUE != bSecondaryFullWeapon))
				{
//					g_pD3dApp->m_pChat->CreateChatChild("�ش� �������� ������ �������� ���ų�, ���������� �����մϴ�.",COLOR_SYSTEM);
//					g_pD3dApp->m_pChat->CreateChatChild("������ ������ �����Ͽ����ϴ�.",COLOR_SYSTEM);
//					g_pD3dApp->m_pChat->CreateChatChild("�̹� �����Ǿ� �ֽ��ϴ�.",COLOR_SYSTEM);
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_ITEM_0014,COLOR_SYSTEM);//"�� źȯ�� ������ ���Ⱑ �����ϴ�."
				}
			}
			else
			{
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_ITEM_0015,COLOR_SYSTEM);//"������ �������� �����ϴ�."
//				g_pD3dApp->m_pChat->CreateChatChild("������ ������ �����Ͽ����ϴ�.",COLOR_SYSTEM);
			}
		}
		break;
	case ITEMKIND_ENCHANT:
	case ITEMKIND_GAMBLE:
		{
//			if(!((CINFGameMain*)m_pParent)->m_pInfWindow->IsExistMsgBox(_Q_USE_ENCHANT_ITEM_CARD))
//			{
//				((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox("����� �������� Ŭ���Ͻÿ�",_Q_USE_ENCHANT_ITEM_CARD);
//				m_bEnchantState = TRUE;
//				m_pEnchantItem = (CItemInfo *)pItem;
//			}
		}
		break;
	case ITEMKIND_TANK:
		break;
//  2006-08-10 by dgwoo �ǵ����� ���� �̺�Ʈ
	case ITEMKIND_RANDOMBOX:
		{
			INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_USE_RANDOMBOX,T_FC_ITEM_USE_RANDOMBOX,pSRandombox,Sendbuf);
			pSRandombox->ItemUID	= pItem->UniqueNumber;
			g_pD3dApp->m_pFieldWinSocket->Write(Sendbuf, MSG_SIZE(MSG_FC_ITEM_USE_RANDOMBOX));
			g_pD3dApp->m_bRequestEnable = FALSE;			// ���� �޼��� ��ٸ���
		}
		break;
// 2006-10-02 by ispark, ��ų����������
	case ITEMKIND_SKILL_SUPPORT_ITEM:
		{
			INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_USE_SKILL_SUPPORT_ITEM, T_FC_ITEM_USE_SKILL_SUPPORT_ITEM, pSkillSupportItem, Sendbuf);
			pSkillSupportItem->ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
			pSkillSupportItem->ItemUniqueNumber = pItem->UniqueNumber;
			g_pD3dApp->m_pFieldWinSocket->Write(Sendbuf, MSG_SIZE(MSG_FC_ITEM_USE_SKILL_SUPPORT_ITEM));
		}
		break;
	default:
		{
			char buf[128];
			wsprintf(buf,STRMSG_C_ITEM_0016,pItem->ItemInfo->ItemName);//"������ [ %s ]�� ��� �� �� ���� �������Դϴ�."
			g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
		}
		break;
	}
}

void CINFInven::SendChangeWearWindowPos(int nWindowPosition)
{
	// 2007-03-02 by bhsohn ���� ���� �߰� ����
	int nSelItemCnt = g_pD3dApp->GetMultiSelectItem();
	if(nSelItemCnt > 0)
	{
		return;
	}
	// end 2007-03-02 by bhsohn ���� ���� �߰� ����


	FLOG( "CINFInven::SendChangeWearWindowPos(int nWindowPosition)" );
	if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC() && IS_TUNING_COLOR_SHOP(g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind))
	{
		// Į�� ���������� ������ ��ü ����
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051207_0003,COLOR_ERROR);
		return;
	}
	// 2006-08-02 by ispark, ���λ������� ������ ��ü ����
	if(g_pInterface->m_pBazaarShop)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_060802_0000,COLOR_ERROR);
		return;
	}

// 2006-07-26 by ispark, ���� ������ �� ����
	CItemInfo* pSelectItem = NULL;
	if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem)
	{
		pSelectItem = (CItemInfo*)(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem);
	}
//	if(m_pSelectItem && m_pSelectItem->ItemWindowIndex != nWindowPosition)
	if(pSelectItem && pSelectItem->ItemWindowIndex != nWindowPosition)
	{

		// 2008-07-23 by dgwoo �����Ҽ� �ִ� �����ǰ��� �������� ������ ��ü ���������� ������.
		if(pSelectItem->GetItemInfo()->Position != nWindowPosition)
		{
			return;
		}
		// 2004-12-10 by jschoi
		// ��ü�ϸ� ���� �������� ����� �� ���� ���� ��ü �Ұ� �޽��� ��¸� �Ѵ�.
		CItemInfo* pWearItemInfo = NULL;
		CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(nWindowPosition);
		if(it != g_pStoreData->m_mapItemWindowPosition.end())
		{
			pWearItemInfo = it->second;
		}
		if(	m_pWearDisplayInfo[nWindowPosition] == NULL || 
			(pWearItemInfo && IsAbleChangeItem(pSelectItem,pWearItemInfo,nWindowPosition))  )
		{
			// 2007-07-16 by dgwoo Ʃ�丮�� �ʿ��� ������ ���� ������ ������ �ʰ� ���� ����.
			if(g_pTutorial->IsTutorialMode() &&
				g_pTutorial->GetLesson() == L6)
			{// 
				// 2008-08-22 by bhsohn EP3 �κ��丮 ó��
				//g_pStoreData->TutorialCompulsionItemSetting(pSelectItem);

			}
			else
			{
				MSG_FC_ITEM_CHANGE_WINDOW_POSITION* pMsg;
				char buffer[SIZE_MAX_PACKET];
				*(MessageType_t*)buffer = T_FC_ITEM_CHANGE_WINDOW_POSITION;
				pMsg = (MSG_FC_ITEM_CHANGE_WINDOW_POSITION*)(buffer+SIZE_FIELD_TYPE_HEADER);
				pMsg->CharacterUniqueNumber = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
				pMsg->FromItemUniqueNumber = pSelectItem->UniqueNumber;
				pMsg->FromItemWindowIndex = pSelectItem->ItemWindowIndex;
				if(m_pWearDisplayInfo[nWindowPosition])
				{
					pMsg->ToItemUniqueNumber = m_pWearDisplayInfo[nWindowPosition]->pItem->UniqueNumber;
				} else 
				{
					pMsg->ToItemUniqueNumber = 0;
				}
				pMsg->ToItemWindowIndex = nWindowPosition;

				// �� ���Կ� �������� ������ ��, �������� ��ü�� �� ���⼭ ������ ��... ���� ��û
				if(m_pWearDisplayInfo[nWindowPosition])
				{
					g_pShuttleChild->m_pSkill->DeleteSkillFromWearItem(m_pWearDisplayInfo[nWindowPosition]->pItem->Kind);
				}
				g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_CHANGE_WINDOW_POSITION));
	//			g_pD3dApp->m_bRequestEnable = FALSE;			// 2006-06-19 by ispark, �޼��� ������ ��ٸ���.
				// 2006-07-24 by dgwoo Ŭ�������� ������ ��ü�� �ȵǴ°��� �����ϱ� ���� �߰�
	//			m_bSelectItem = FALSE;
			}
		}
		else
		{
			char buf[128];
			wsprintf(buf,STRMSG_C_ITEM_0007);//"�䱸 �������� ���Ͽ� �ش� �������� ���� �� �� �����ϴ�."
			g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
		}
	}
}


void CINFInven::SetSelectItem(INVEN_DISPLAY_INFO* pDisplayInfo)
{
	FLOG( "CINFInven::SetSelectItem(INVEN_DISPLAY_INFO* pDisplayInfo)" );

	// 2006-07-27 by ispark, ������ ���� ��ġ���� ���콺 ������ ���� �ֱ�
	POINT ptIcon;
	ptIcon.x = m_nRenderMoveIconIntervalWidth;
	ptIcon.y = m_nRenderMoveIconIntervalHeight;

	((CINFGameMain*)m_pParent)->SetSelectItem(pDisplayInfo, ptIcon, ITEM_INVEN_POS);

//	m_bSelectWearItem = FALSE;
//	if(pDisplayInfo && !m_pDeleteItem)
//	{
//		m_pSelectIcon = ((CINFGameMain*)m_pParent)->m_pIcon->FindIcon(pDisplayInfo->IconName);
//		m_pSelectItem = (CItemInfo*)pDisplayInfo->pItem;
////		m_nSelectItemWindowPosition = ((CItemInfo*)pDisplayInfo->pItem)->ItemWindowIndex;
//		g_pGameMain->m_pQuickSlot->m_pSelectItem = (ITEM_BASE*)m_pSelectItem;
//
//		// set tooltip
//        POINT pt;
//		char buf[128];
//        GetCursorPos( &pt );
//        ScreenToClient( g_pD3dApp->GetHwnd(), &pt );
//		CheckMouseReverse(&pt);
//
//		if(IS_COUNTABLE_ITEM(pDisplayInfo->pItem->Kind))
////		if(pDisplayInfo->pItem->Kind == ITEMKIND_ENERGY || pDisplayInfo->pItem->Kind == ITEMKIND_MINE)
//		{
//			ITEM *item = g_pDatabase->GetServerItemInfo(pDisplayInfo->pItem->ItemNum);
//			if(item)
//				wsprintf(buf, STRMSG_C_TOOLTIP_0013,item->ItemName,((ITEM_GENERAL*)pDisplayInfo->pItem)->CurrentCount);
//		}
//		else
//		{
//			ITEM *item = g_pDatabase->GetServerItemInfo(pDisplayInfo->pItem->ItemNum);
//			if(item)
//				wsprintf(buf, "%s",item->ItemName);
//		}
//		((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, pDisplayInfo->Name);
//	}
//	else
//	{
////		m_nSelectItemWindowPosition = -1;
//		// 2006-05-30 by ispark, �κ��丮 �������� ������ �Ѱ��� �ƴ϶�� -1�� �Ժη� �ָ� �ȉ´?
//		if(g_pGameMain->m_pQuickSlot->m_pSelectItem)
//			g_pGameMain->m_pQuickSlot->m_nSelectSlotNumber = -1;
//		g_pGameMain->m_pQuickSlot->m_pSelectItem = NULL;
//		m_pSelectItem = NULL;
//		m_pSelectIcon = NULL;
//	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void SetMultiSelectItem(INVEN_DISPLAY_INFO* pDisplayInfo)
/// \brief		���� ���� ó��
/// \author		bhsohn
/// \date		2007-02-12 ~ 2007-02-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInven::SetMultiSelectItem(INVEN_DISPLAY_INFO* pDisplayInfo)
{	
	POINT ptIcon;
	ptIcon.x = m_nRenderMoveIconIntervalWidth;
	ptIcon.y = m_nRenderMoveIconIntervalHeight;
	((CINFGameMain*)m_pParent)->SetMultiSelectItem(pDisplayInfo, 
									((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem, ptIcon);
}

void CINFInven::DeleteEnchantItem(UINT nUniqueNumber)
{
	FLOG( "CINFInven::DeleteEnchantItem(UINT nUniqueNumber)" );
	m_pEnchantItem = NULL;
/*	if(m_pEnchantItem)
	{
		map<int, ITEM_GENERAL*>::iterator it = g_pD3dApp->m_pShuttleChild->m_mapItem.begin();
		while(it != g_pD3dApp->m_pShuttleChild->m_mapItem.end())
		{
			ITEM_GENERAL* pItem = it->second;
			
			if(pItem->UniqueNumber == m_pEnchantItem->UniqueNumber)
			{
				g_pD3dApp->m_pShuttleChild->m_mapItem.erase(it);//pItem->ItemWindowIndex);
				util::del(pItem);
				g_pD3dApp->m_pShuttleChild->ResortingItem();
				g_pD3dApp->m_pShuttleChild->SetShuttleData(5);
				SetAllIconInfo();
				break;
			}
			it++;
		}
		m_pEnchantItem = NULL;
	}
#ifdef _DEBUG
	else
	{
		DBGOUT("ERROR : void CINFInven::DeleteEnchantItem(UINT nUniqueNumber) m_pEnchantItem == NULL\n");
	}
#endif
*/
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFInven::IsAbleSetItem(CItemInfo* pItemInfo,int nWindowPosition)
/// \brief		�ش� �������� ������ �� �ִ°�?
/// \author		jschoi
/// \date		2004-12-10 ~ 2004-12-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFInven::IsAbleSetItem(CItemInfo* pItemInfo,int nWindowPosition)
{
//	if(nWindowPosition != POS_REAR)
//	{
//		return TRUE;
//	}
	GEAR_STAT sGearStat = g_pShuttleChild->m_myShuttleInfo.TotalGearStat;
	ITEM* pItem = pItemInfo->GetRealItemInfo();

	// 2009-04-21 by bhsohn ������ DesParam�߰�
// 	SetGearStatByItem(sGearStat,pItem->DestParameter1,pItem->ParameterValue1);
// 	SetGearStatByItem(sGearStat,pItem->DestParameter2,pItem->ParameterValue2);
// 	SetGearStatByItem(sGearStat,pItem->DestParameter3,pItem->ParameterValue3);
// 	SetGearStatByItem(sGearStat,pItem->DestParameter4,pItem->ParameterValue4);
	int nArrParamCnt = 0;
	for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
	{
		SetGearStatByItem(sGearStat,
							pItem->ArrDestParameter[nArrParamCnt],
							pItem->ArrParameterValue[nArrParamCnt]);
	}	
	// end 2009-04-21 by bhsohn ������ DesParam�߰�
	
	CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(POS_REAR);
	if(it != g_pStoreData->m_mapItemWindowPosition.end())
	{
		CItemInfo* pWearItemInfo = it->second;
		if(	pWearItemInfo->GetRealItemInfo()->ReqGearStat.AttackPart > sGearStat.AttackPart ||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.DefensePart > sGearStat.DefensePart ||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.DodgePart > sGearStat.DodgePart ||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.FuelPart > sGearStat.FuelPart ||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.ShieldPart > sGearStat.ShieldPart||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.SoulPart > sGearStat.SoulPart )
		{
			return FALSE;
		}
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFInven::IsAbleReleaseItem(CItemInfo* pItemInfo,int nWindowPosition)
/// \brief		�ش� �������� ������ �� �ִ°�?
/// \author		jschoi
/// \date		2004-12-10 ~ 2004-12-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFInven::IsAbleReleaseItem(CItemInfo* pItemInfo,int nWindowPosition)
{
	if(nWindowPosition == POS_REAR)
	{
		return TRUE;
	}
	
	GEAR_STAT sGearStat = g_pShuttleChild->m_myShuttleInfo.TotalGearStat;
	ITEM* pItem = pItemInfo->GetRealItemInfo();
	// 2009-04-21 by bhsohn ������ DesParam�߰�
// 	ReleaseGearStatByItem(sGearStat,pItem->DestParameter1,pItem->ParameterValue1);
// 	ReleaseGearStatByItem(sGearStat,pItem->DestParameter2,pItem->ParameterValue2);
// 	ReleaseGearStatByItem(sGearStat,pItem->DestParameter3,pItem->ParameterValue3);
// 	ReleaseGearStatByItem(sGearStat,pItem->DestParameter4,pItem->ParameterValue4);
	int nArrParamCnt = 0;
	for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
	{
		ReleaseGearStatByItem(sGearStat,pItem->ArrDestParameter[nArrParamCnt],
										pItem->ArrParameterValue[nArrParamCnt]);

	}	
	// end 2009-04-21 by bhsohn ������ DesParam�߰�

	CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(POS_REAR);
	if(it != g_pStoreData->m_mapItemWindowPosition.end())
	{
		CItemInfo* pWearItemInfo = it->second;
		if(	pWearItemInfo->GetRealItemInfo()->ReqGearStat.AttackPart > sGearStat.AttackPart ||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.DefensePart > sGearStat.DefensePart ||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.DodgePart > sGearStat.DodgePart ||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.FuelPart > sGearStat.FuelPart ||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.ShieldPart > sGearStat.ShieldPart||
			pWearItemInfo->GetRealItemInfo()->ReqGearStat.SoulPart > sGearStat.SoulPart )
		{
			return FALSE;
		}
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFInven::IsAbleChangeItem(CItemInfo* pSetItemInfo,CItemInfo* pReleaseItemInfo,int nWindowPosition)
/// \brief		�ش� �������� ��ü�� �� �ִ°�?
/// \author		jschoi
/// \date		2004-12-10 ~ 2004-12-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFInven::IsAbleChangeItem(CItemInfo* pSetItemInfo,CItemInfo* pReleaseItemInfo,int nWindowPosition)
{
//	if(nWindowPosition != POS_REAR)
//	{
//		return TRUE;
//	}
	GEAR_STAT sGearStat = g_pShuttleChild->m_myShuttleInfo.TotalGearStat;
	ITEM* pSetItem = pSetItemInfo->GetRealItemInfo();
	ITEM* pReleaseItem = pReleaseItemInfo->GetRealItemInfo();

	// 2009-04-21 by bhsohn ������ DesParam�߰�
// 	SetGearStatByItem(sGearStat,pSetItem->DestParameter1,pSetItem->ParameterValue1);
// 	SetGearStatByItem(sGearStat,pSetItem->DestParameter2,pSetItem->ParameterValue2);
// 	SetGearStatByItem(sGearStat,pSetItem->DestParameter3,pSetItem->ParameterValue3);
// 	SetGearStatByItem(sGearStat,pSetItem->DestParameter4,pSetItem->ParameterValue4);
// 	ReleaseGearStatByItem(sGearStat,pReleaseItem->DestParameter1,pReleaseItem->ParameterValue1);
// 	ReleaseGearStatByItem(sGearStat,pReleaseItem->DestParameter2,pReleaseItem->ParameterValue2);
// 	ReleaseGearStatByItem(sGearStat,pReleaseItem->DestParameter3,pReleaseItem->ParameterValue3);
// 	ReleaseGearStatByItem(sGearStat,pReleaseItem->DestParameter4,pReleaseItem->ParameterValue4);
	int nArrParamCnt = 0;
	for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
	{
		SetGearStatByItem(sGearStat,pSetItem->ArrDestParameter[nArrParamCnt],
									pSetItem->ArrParameterValue[nArrParamCnt]);
	}	
	for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
	{
		ReleaseGearStatByItem(sGearStat,pReleaseItem->ArrDestParameter[nArrParamCnt],
										pReleaseItem->ArrParameterValue[nArrParamCnt]);
	}		
	// end 2009-04-21 by bhsohn ������ DesParam�߰�

	ITEM* pItem = NULL;
	if( nWindowPosition == POS_REAR )
	{
		pItem = pSetItemInfo->GetRealItemInfo();
	}
	else
	{
		CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(POS_REAR);
		if(it != g_pStoreData->m_mapItemWindowPosition.end())
		{
			pItem = it->second->GetRealItemInfo();
		}
		else
		{
			return TRUE;
		}
	}

	if(	pItem->ReqGearStat.AttackPart > sGearStat.AttackPart ||
		pItem->ReqGearStat.DefensePart > sGearStat.DefensePart ||
		pItem->ReqGearStat.DodgePart > sGearStat.DodgePart ||
		pItem->ReqGearStat.FuelPart > sGearStat.FuelPart ||
		pItem->ReqGearStat.ShieldPart > sGearStat.ShieldPart||
		pItem->ReqGearStat.SoulPart > sGearStat.SoulPart )
	{
		return FALSE;
	}

//	CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(POS_REAR);
//	if(it != g_pStoreData->m_mapItemWindowPosition.end())
//	{
//		CItemInfo* pWearItemInfo = it->second;
//		if(	pWearItemInfo->GetRealItemInfo()->ReqGearStat.AttackPart > sGearStat.AttackPart ||
//			pWearItemInfo->GetRealItemInfo()->ReqGearStat.DefensePart > sGearStat.DefensePart ||
//			pWearItemInfo->GetRealItemInfo()->ReqGearStat.DodgePart > sGearStat.DodgePart ||
//			pWearItemInfo->GetRealItemInfo()->ReqGearStat.FuelPart > sGearStat.FuelPart ||
//			pWearItemInfo->GetRealItemInfo()->ReqGearStat.ShieldPart > sGearStat.ShieldPart||
//			pWearItemInfo->GetRealItemInfo()->ReqGearStat.SoulPart > sGearStat.SoulPart )
//		{
//			return FALSE;
//		}
//	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFInven::SetGearStatByItem(GEAR_STAT& sGearStat, int nDestParam, float fDestValue)
/// \brief		�����ۿ� ���� ������ �߰��Ѵ�.
/// \author		jschoi
/// \date		2004-12-10 ~ 2004-12-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInven::SetGearStatByItem(GEAR_STAT& sGearStat, int nDestParam, float fDestValue)
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
/// \fn			CINFInven::ReleaseGearStatByItem(GEAR_STAT& sGearStat, int nDestParam, float fDestValue)
/// \brief		�����ۿ� ���� ������ �����Ѵ�.
/// \author		jschoi
/// \date		2004-12-10 ~ 2004-12-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInven::ReleaseGearStatByItem(GEAR_STAT& sGearStat, int nDestParam, float fDestValue)
{
	switch(nDestParam)
	{
	case DES_ATTACK_PART:				// ���� ����̽� ��Ʈ
		sGearStat.AttackPart -= fDestValue;
		break;
	case DES_DEFENSE_PART:				// ���� ����̽� ��Ʈ
		sGearStat.DefensePart -= fDestValue;
		break;
	case DES_FUEL_PART:					// ���� ����̽� ��Ʈ
		sGearStat.FuelPart -= fDestValue;
		break;
	case DES_SOUL_PART:					// ���� ����̽� ��Ʈ
		sGearStat.SoulPart -= fDestValue;
		break;
	case DES_SHIELD_PART:				// ��� ����̽� ��Ʈ
		sGearStat.ShieldPart -= fDestValue;
		break;
	case DES_DODGE_PART	:				// ȸ�� ����̽� ��Ʈ
		sGearStat.DodgePart -= fDestValue;
		break;
	}
}

int	CINFInven::GetScrollLine()
{
	int nInvenTotalNum = g_pStoreData->m_mapItemUniqueNumber.size();
	int nWearItemNum = GetWearItemNum();
	int ntemp = int((nInvenTotalNum - nWearItemNum) / INVEN_X_NUMBER) - 6;
	if(0 != ((nInvenTotalNum - nWearItemNum) % INVEN_X_NUMBER))
		ntemp++;

	if(ntemp < 0)
	{
		ntemp = 0;
	}
	return ntemp;
}

int	CINFInven::GetScrollLineNumber(float nLength)
{
	int ntemp = GetScrollLine();
	float fLineNum = (nLength * ntemp) / INVEN_SCROLL_LINE_MOVE_LENGTH;

	return (int)fLineNum;
}


int CINFInven::ProcessRotationUnitWnd(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// �⺻ ���� X ��ġ = 16
	// �⺻ ���� Y ��ġ = nWindowPosY + 207	
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);
			if(ProcessRotationUnit(pt, uMsg) == INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;
		}
		break;
	}	
	return INF_MSGPROC_NORMAL;
}

int CINFInven::ProcessRotationUnit(POINT pt, UINT uMsg)
{
	if(uMsg == WM_LBUTTONUP)	SetByPushButton(FALSE);

	m_nRotationState = ROTATION_NONE;
	m_nButtonState	 = ROTATION_STATE_N;
	
	m_bTurnLeft	= FALSE;
	m_bTurnRight = FALSE;
	m_bTurnUp = FALSE;
	m_bTurnDown = FALSE;

	// �⺻ ���� X ��ġ = 16
	// �⺻ ���� Y ��ġ = nWindowPosY + 207

	int PosX = 16;
	int PosY = g_pGameMain->m_nLeftWindowY +207;
	if(g_pD3dApp->m_dwGameState == _SHOP)
	{
		PosX += CITY_BASE_NPC_BOX_START_X;

	}


	if( pt.x>PosX && pt.x< PosX+12 && pt.y>PosY+8 && pt.y<PosY+30)
	{	// ���� ������
		switch(uMsg)
		{
		case WM_MOUSEMOVE:
			m_nRotationState = ROTATION_LEFT;
			m_nButtonState = ROTATION_STATE_O;
			if(m_bByPush[0] == TRUE)
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
	else if( pt.x>PosX+34 && pt.x< PosX+46 && pt.y>PosY+8 && pt.y<PosY+30)
	{	// ������ ������
		switch(uMsg)
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
			if(m_bByPush[1] == TRUE)
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
	else if( pt.x>PosX+13 && pt.x< PosX+33 && pt.y>PosY && pt.y<PosY+10)
	{	// ���� ������
		switch(uMsg)
		{
		case WM_MOUSEMOVE:
			m_nRotationState = ROTATION_UP;
			m_nButtonState = ROTATION_STATE_O;
			if(m_bByPush[2] == TRUE)
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
	else if( pt.x>PosX+13 && pt.x<PosX+33 && pt.y>PosY+26 && pt.y<PosY+38)
	{	// �Ʒ��� ������
		switch(uMsg)
		{
		case WM_MOUSEMOVE:
			m_nRotationState = ROTATION_DOWN;
			m_nButtonState = ROTATION_STATE_O;
			if(m_bByPush[3] == TRUE)
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
	else if( pt.x>PosX+13 && pt.x<PosX+33 && pt.y>PosY+13 && pt.y<PosY+25)
	{	// �߾� ������
		switch(uMsg)
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

void CINFInven::SetByPushButton(BOOL bFlag)
{
	for(int i=0; i<4; i++)
	{
		m_bByPush[i] = FALSE;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFInven::GetWearItemNum()
/// \brief		������ ������ ���� Ȯ��
/// \author		ispark
/// \date		2005-12-08 ~ 2005-12-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFInven::GetWearItemNum()
{
	int count = 0;
	for(int i = 0; i < SIZE_MAX_POS; i++)
	{
		if(m_pWearDisplayInfo[i])
		{
			count++;
		}
	}
	
	return count;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFInven::GetScrollLineNumber(int nLength)
/// \brief		���� ��ũ�� ��ġ ���� ��ġ ��������
/// \author		ispark
/// \date		2005-12-08 ~ 2005-12-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CINFInven::GetScrollLineInterval(int nScrollLineNum)
{
	int ntemp = GetScrollLine();
	float fLineNum = 0.0f;
	if(ntemp)
		fLineNum = (INVEN_SCROLL_LINE_MOVE_LENGTH * nScrollLineNum) / ntemp;

	return fLineNum;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFInven::SetScrollEndLine()
/// \brief		��ũ���� ������ �������� �ű��
/// \author		ispark
/// \date		2006-06-21 ~ 2006-06-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInven::SetScrollEndLine()
{
	m_nInvenCurrentScroll = GetScrollLine();
	m_nInvenCurrentScrollPosition = INVEN_SCROLL_LINE_MOVE_LENGTH;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFInven::InitInven()
/// \brief		�κ��丮 �ʱ� ����, ��ũ����ġ�� ���� ������ ����
/// \author		ispark
/// \date		2005-12-12 ~ 2005-12-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInven::InitInven()
{
	m_nInvenCurrentScroll = 0;
	m_nInvenCurrentScrollPosition = 0;
	SetAllIconInfo();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFInven::RenderInvenBack()
/// \brief		�κ��丮 ȭ�� ���
/// \author		ispark
/// \date		2006-04-04 ~ 2006-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInven::RenderInvenBack(BOOL bShop)
{
	D3DXMATRIX pTemp, pMatrix, pMatBack, pMatOldProj, pMatOldView, pMatPresView;
	D3DXMatrixIdentity(&pMatrix);
	D3DXMatrixIdentity(&pMatBack);
	D3DXMatrixIdentity(&pTemp);
	D3DXMatrixIdentity(&pMatOldView);
	D3DXMatrixIdentity(&pMatPresView);
	

	RECT WindowRect;
	GetClientRect(g_pD3dApp->GetHwnd(), &WindowRect);
	D3DXMatrixOrthoLH(&m_pMatInvenBackProj, 
		             (float)WindowRect.right, 
					 (float)WindowRect.bottom, 
					 0.0f, 100.0f);

	g_pD3dDev->GetTransform( D3DTS_VIEW,		&pMatOldView);
	g_pD3dDev->GetTransform( D3DTS_PROJECTION, &pMatOldProj );	
	g_pD3dDev->SetTransform( D3DTS_VIEW,		&pMatPresView);
	g_pD3dDev->SetTransform( D3DTS_PROJECTION, &m_pMatInvenBackProj);	
	
	// ��׶��� ������
	// Restore the modified render states
	g_pD3dDev->SetRenderState( D3DRS_CLIPPLANEENABLE, 0x00 );
	g_pD3dDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	
	// Finally, render the mirror itself (as an alpha-blended quad)
	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
	g_pD3dDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, FALSE );	
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE, FALSE );
	
	int nScreenWidth = -(int)g_pD3dApp->GetBackBufferDesc().Width / 2;
	int nScreenHeight = ((float)g_pD3dApp->GetBackBufferDesc().Height / 2) + 0.5f;
	int nWindowPosY = g_pGameMain->m_nLeftWindowY;
	D3DXMatrixScaling(&pMatBack, INVEN_BACK_W, INVEN_BACK_H, 1.0f);

	if(bShop)
	{
		int nStart = CITY_BASE_NPC_BOX_START_X;
		D3DXMatrixTranslation(&pTemp, 
								nScreenWidth + nStart + INVEN_BACK_POS_X, 
								nScreenHeight - (nWindowPosY + INVEN_BACK_POS_Y), 
								0.0f);
	}
	else
	{
		D3DXMatrixTranslation(&pTemp, 
								nScreenWidth + INVEN_BACK_POS_X, 
								nScreenHeight - (nWindowPosY + INVEN_BACK_POS_Y), 
								0.0f);
	}
	pMatBack = pMatBack * pTemp;

	// ��׶���		
	g_pD3dDev->SetTransform( D3DTS_WORLD, &pMatBack );
	g_pD3dDev->SetTexture( 0, m_pMirrorTexture );
	g_pD3dDev->SetStreamSource( 0, m_pMirrorVB, 0, sizeof(MIRRORVERTEX) );
	g_pD3dDev->SetFVF( MIRRORVERTEX::FVF );
	g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE );
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );

	g_pD3dDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	g_pD3dDev->SetTransform( D3DTS_VIEW, &pMatOldView);	
	g_pD3dDev->SetTransform( D3DTS_PROJECTION, &pMatOldProj);	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFInven::ReSetScrollCurrentLine()
/// \brief		���� ��ũ�� ������ üũ�� �ؼ� �� �����Ѵ�.
/// \author		ispark
/// \date		2006-09-26 ~ 2006-09-26
/// \warning	
///
/// \param		
/// \return		
//////////////////////////////////////////////////////////////////////////////
void CINFInven::ReSetScrollCurrentLine()
{
	int nCurrentLine = GetScrollLine();

	// ������ �κ� ������ ������
	if(m_nInvenCurrentScroll > nCurrentLine)
	{
		m_nInvenCurrentScroll = nCurrentLine;
		m_nInvenCurrentScrollPosition = GetScrollLineInterval(m_nInvenCurrentScroll);
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int GetInvenFreeSize()
/// \brief		�κ��� ����ִ� ���� �˷��ش�.
/// \author		bhsohn
/// \date		2007-02-22 ~ 2007-02-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFInven::GetInvenFreeSize()
{
	
	// 2009. 11. 3 by jskim ĳ��(�κ�/â�� Ȯ��) ������ �߰� ����
	//int nFreeSize = (CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1) - 1) 
	// 2009. 12. 17 by jskim ĳ��(�κ�/â�� Ȯ��) ����� 
// 	CHARACTER* pMainInfo = g_pD3dApp->GetMFSMyShuttleInfo();	
// 	int nFreeSize = (CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1, pMainInfo.GetAddedPermanentInventoryCount()) - 1) 
	int nFreeSize = (CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1, g_pShuttleChild->m_myShuttleInfo.GetAddedPermanentInventoryCount()) - 1) 
	//end 2009. 12. 17 by jskim ĳ��(�κ�/â�� Ȯ��) ����� 
	//end 2009. 11. 3 by jskim ĳ��(�κ�/â�� Ȯ��) ������ �߰� ����
					- g_pStoreData->GetTotalUseInven();	

	return nFreeSize;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFInven::ClearMultiSeletItem()
/// \brief		�κ��� ����ִ� ���� �˷��ش�.
/// \author		// 2007-03-02 by bhsohn ���� ���� �߰� ����
/// \date		2007-03-05 ~ 2007-03-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInven::ClearMultiSeletItem()
{
	int nSelItemCnt = g_pD3dApp->GetMultiSelectItem();
	if(nSelItemCnt > 0)
	{
		SetMultiSelectItem(NULL);	// ���� ó�� ����
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL IsInvenHave(int nItemNum)
/// \brief		�κ��� �ִ��� ���θ� �Ǵ��Ѵ�. 
/// \author		// 2007-03-09 by bhsohn ���� ���� â�� �κ� ����
/// \date		2007-03-09 ~ 2007-03-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFInven::IsInvenHave(int nItemNum)
{
	BOOL bHave = FALSE;
	for(int nCnt = 0; nCnt < INVEN_NUMBER; nCnt++)
	{
		if(NULL == m_pInvenDisplayInfo[nCnt])
		{
			continue;
		}
		if(nItemNum == m_pInvenDisplayInfo[nCnt]->pItem->ItemNum )
		{
			bHave = TRUE;
			break;
		}
	}
	return bHave;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-06-20 by bhsohn ������ ������, �޸� ���� ����
/// \date		2007-06-20 ~ 2007-06-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInven::InitDeleteItemInfo()
{
	memset(&m_struDeleteItem, 0x00, sizeof(structDelItemInfo));
}


void CINFInven::SetDeleteItemInfo(CItemInfo* pSelectItem)
{
	if(NULL == pSelectItem)
	{
		InitDeleteItemInfo();
		return;
	}
	m_struDeleteItem.UniqueNumber = pSelectItem->UniqueNumber;
	m_struDeleteItem.CurrentCount = pSelectItem->CurrentCount;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-08-07 by bhsohn ����Ŀ ������ �߰�
/// \date		2007-08-07 ~ 2007-08-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInven::DoModalChatAllInfluence(UID64_t		UniqueNumber)
{	
	if(g_pGameMain && g_pGameMain->m_pInfWindow)
	{		
		g_pGameMain->m_bChangeMousePoint = TRUE;
		g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_070810_0201, 
												_Q_USEITEM_SPEAKER_MSG,
												0,
												0,
												0, 
												UniqueNumber);	//"��ĳ���� �̸��� �Է��� �ֽʽÿ�(\\r����� �α׿��� �˴ϴ�\\r)"
	}
}
