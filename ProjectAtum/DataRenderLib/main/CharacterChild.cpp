// CharacterChild.cpp: implementation of the CCharacterChild class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharacterChild.h"
#include "ShuttleChild.h"
#include "AtumApplication.h"
#include "AtumSound.h"
#include "EnemyData.h"
#include "ChatMoveData.h"
#include "AtumApplication.h"
#include "FieldWinSocket.h"
#include "Chat.h"
#include "Cinema.h"								// 2005-07-25 by ispark
#include "Background.h"
#include "SceneData.h"
#include "Camera.h"
#include "Weapon.h"
#include "INFGameMain.h"						// 2005-07-25 by ispark
#include "Skill.h"								// 2005-08-02 by ispark
#include "AtumDatabase.h"
#include "WeaponShieldData.h"
#include "CharacterRender.h"					// 2005-07-21 by ispark
#include "Frustum.h"
#include "ObjectChild.h"
#include "ClientParty.h"
#include "ObjRender.h"
#include "SkillEffect.h"
#include "dxutil.h"
#include "Interface.h"
#include "INFCityBase.h"
#include "INFTarget.h"
#include "INFMissionInfo.h"
#include "PkNormalTimer.h"						// 2005-08-02 by ispark
#include "INFCommunity.h"
#include "INFCommunityGuild.h"
#include "EffectRender.h"
#include "StoreData.h"	
#include "CustomOptimizer.h"

// 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)
#include "INFCityInfinityField.h"
#include "INFGameArena.h"
// end 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)

// 2011-07-22 by hsson �?�ߺ� �������� ��æƮ �Ǵ� ���� ����
#include "INFInvenExtend.h"
// end 2011-07-22 by hsson �?�ߺ� �������� ��æƮ �Ǵ� ���� ����


#include "PetManager.h"			// 2010-06-15 by shcho&hslee ��ý��� - �� �����͸� �������� �Լ�
#include "INFItemInfo.h"
#include "WeaponItemInfo.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#define CHARACTER_HEIGHT				7.087f

// 2009-02-05 by bhsohn ī�޶� ������ ���� ó��
// #define CHARACTER_CAMERA_DISTANCE		30.0f	// �ʱ�Ÿ�
// #define CHARACTER_CAMERA_HEIGHT			30.0f	// �ʱ����
// #define CHARACTER_CAMERA_WHEEL_DISTANCE 6.0f	// �� ����
// end 2009-02-05 by bhsohn ī�޶� ������ ���� ó��

#define CHARACTER_WALK					10.0f	// �ȱ�
#define CHARACTER_MAN_RUN				35.7f	// ���� �ٱ� 46->25->30->33		// 2006-11-02 by ispark, �ٸ������� �ٲ�
#define CHARACTER_WOMAN_RUN				33.0f	// ���� �ٱ� 46->25->30->33		// 2006-11-02 by ispark, �ٸ������� �ٲ�

#define CHARACTER_SHOP_SERCH_DISTANCE	50.0f	// ���˻�

// 2007-07-27 by bhsohn ������ī �ʸ� ���� �ν� ���� ����
#define CHARACTER_SHOP_SERCH_FREESKA_DISTANCE	100.0f	// ���˻�

#define CHARACTER_PICKING_LENGTH		10000.0f;// Picking ��� �Ÿ�
//////////////////////////////////////////////////////////////////////////

CCharacterChild::CCharacterChild()
{
	D3DXMatrixIdentity(&m_mMatrix);
	D3DXMatrixIdentity(&m_mPickMatrix);

	g_pCharacterChild = this;
	m_pCharacterInfo = nullptr;
	m_pPickingInfo = nullptr;					// 2005-07-26 by ispark Picking ����Ʈ
	m_pCharacterRender = g_pD3dApp->m_pCharacterRender;

	m_dwState = _INITIALIZING;
//	m_dwPartType = _SHUTTLE;
	m_bRButtonState = FALSE;
	m_bPickMove = FALSE;

	m_pVBShadow = nullptr;

	m_fCurrentTime = 0.0f;
	m_fCharacterSpeed = 0.0f;
	m_fCharacterAddSpeed = .0f;
	m_hyBodyCondition = 0;
	m_nUnitNum = 0;
	m_pSelectObject = nullptr;					// 2006-04-08 by ispark
	m_pShopObject = nullptr;
	m_bEnterShopState = FALSE;

	m_fOldLength = 0.0f;					// 2005-07-29 by ispark
	m_bMouseMove = FALSE;					// 2005-08-16 by ispark
	m_bMouseWheel = FALSE;					// 2005-08-17 by ispark

	m_bCharacterRender = TRUE;
	m_nStartEventType = 0;

	m_bStartPositionEvent = FALSE;
	m_pSelBazaarShop = nullptr;					// 2006-07-29 by ispark
	m_fCharacterWarpErrTime = 0.0f;				// 2006-08-07 by ispark

	m_pMoveChatShop = nullptr;

//	m_fCharacterMoveRate = 0.0f;			// 2006-11-02 by ispark

	// 2010-11-29 by jskim, �� ������ �̵��ӵ� ����
	m_bSetSpeed = FALSE;
	m_bPetWearItem = FALSE;
	// end 2010-11-29 by jskim, �� ������ �̵��ӵ� ����

	// 2012-06-25 by jhahn, �� ������ �̵��ӵ� ���� ����߰�
	PetoptionSpeed	= NULL;
	//end 2012-06-25 by jhahn, �� ������ �̵��ӵ� ���� ����߰�

	// 2013-06-10 by bhsohn ��������Ʈ ������ - ���� '�ɸ����̵��ӵ�' ����
	m_fAdminModeSpeed = 0.0f;
	// END 2013-06-10 by bhsohn ��������Ʈ ������ - ���� '�ɸ����̵��ӵ�' ����
}

CCharacterChild::~CCharacterChild()
{
	SAFE_RELEASE(m_pVBShadow);
	util::del(m_pMoveChatShop);
}

HRESULT CCharacterChild::InitDeviceObjects()
{
	SAFE_RELEASE(m_pVBShadow);
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->InitDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		util::del(m_pCharacterInfo);
	}
	if(m_pPickingInfo)
	{
		m_pPickingInfo->InvalidateDeviceObjects();
		m_pPickingInfo->DeleteDeviceObjects();
		util::del(m_pPickingInfo);
	}
	if(m_pMoveChatShop)
		m_pMoveChatShop->InitDeviceObjects();
	return S_OK;
}

HRESULT CCharacterChild::RestoreDeviceObjects()
{
	RestoreShadow();
	if(m_pMoveChatShop)
		m_pMoveChatShop->RestoreDeviceObjects();
	return S_OK;
}

HRESULT CCharacterChild::InvalidateDeviceObjects()
{
	SAFE_RELEASE(m_pVBShadow);
	if(m_pMoveChatShop)
		m_pMoveChatShop->InvalidateDeviceObjects();
	return S_OK;
}

HRESULT CCharacterChild::DeleteDeviceObjects()
{
	SAFE_RELEASE(m_pVBShadow);
//	if(m_pCharacterInfo)
//		m_pCharacterInfo->DeleteDeviceObjects();
//	if(m_pPickingInfo)
//		m_pPickingInfo->DeleteDeviceObjects();
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		util::del(m_pCharacterInfo);
	}
	if(m_pPickingInfo)
	{
		m_pPickingInfo->InvalidateDeviceObjects();
		m_pPickingInfo->DeleteDeviceObjects();
		util::del(m_pPickingInfo);
	}
	if(m_pMoveChatShop)
	{
		m_pMoveChatShop->DeleteDeviceObjects();
		util::del(m_pMoveChatShop);
	}
	return S_OK;
}
// 2010-06-08 by dgwoo, ��ý��� �߰�. 
void CCharacterChild::SetPartner(char* i_szPartner)
{
	//int nPartner = atoi(i_szPartner);
	//CreateWearItem( WEAR_ITEM_KIND_PET, nPartner, FALSE);
}
// 2010-06-08 by dgwoo, ��ý��� �߰�. 
void CCharacterChild::Render()
{
	FLOG( "CShuttleChild::Render()" );
//	if(g_pCamera->GetCamType() == CAMERA_TYPE_NORMAL ||
//		m_bIsCameraPattern == TRUE)
	// 2005-08-30 by ispark	
//	g_pShuttleChild->RenderMirror();

	if(m_bCharacterRender)
	{
		g_pD3dDev->SetRenderState( D3DRS_SPECULARENABLE, FALSE );

//		g_pD3dDev->LightEnable( 1, FALSE );
//		g_pD3dDev->LightEnable( 2, FALSE );
//		g_pD3dDev->LightEnable( 3, FALSE );
//		g_pD3dDev->SetRenderState( D3DRS_FOGENABLE,  IsFogEnableMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) );
		g_pD3dDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);
		g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  TRUE );
		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
		
//		g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
//		g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
		g_pD3dDev->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATER);
		g_pD3dDev->SetRenderState( D3DRS_ALPHAREF,  0x00000088 );
	

		
//		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		
		m_pCharacterRender->Render();	
		g_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		
	}
}

void CCharacterChild::Tick(float fElapsedTime)
{
	//////////////////////////////////////////////////////////////////////////
	// CShuttleChild->Tick()�߿��� �� �ʿ��� �Լ���... 
	// ĳ�����϶��� ���ƾ� �ϴ� �Լ���
	// 2005-08-02 by ispark

	// ��Ƽ Tick
	if(g_pShuttleChild->m_pClientParty->IsParty())
	{
		g_pShuttleChild->m_pClientParty->Tick(fElapsedTime);
	}
	
	g_pShuttleChild->m_pPkNormalTimer->Tick(fElapsedTime);

	g_pShuttleChild->UnitInfoBarSmoothRepair(fElapsedTime);
	g_pShuttleChild->UsedItemTick(fElapsedTime);
//	g_pShuttleChild->m_pSkill->Tick(fElapsedTime);			// 2005-08-08 by ispark
	// Ŭ���̾�Ʈ Ÿ�̸� ���
	g_pShuttleChild->m_timeProcess.Tick(fElapsedTime);
	g_pShuttleChild->TickBurn(fElapsedTime);				// 2005-08-04 by ispark
	g_pShuttleChild->m_pSkill->Tick(fElapsedTime);			// 2005-12-01 by ispark

	//////////////////////////////////////////////////////////////////////////
	// ���� ���� �̿����̰�? �̿����̸� Picking ����
	// 2006-06-08 by ispark, ���� üũ ����
	m_bEnterShopState = FALSE;
//	GUI_BUILDINGNPC* pTempBase = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();
//	if(pTempBase)
//	{
//		if( m_pShopObject )
//		{
//			m_bPickMove = FALSE;
//			m_bEnterShopState = TRUE;
//			m_dwState = _STAND;
//			m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
////			float fDistance = D3DXVec3Length(&(m_vPos - m_pShopObject->m_vPos));
////			if(fDistance>CHARACTER_SHOP_SERCH_DISTANCE)
////			{
////				m_bPickMove = TRUE;
////				g_pInterface->m_pCityBase->SendLeaveEnterBuilding( g_pInterface->m_pCityBase-> GetCurrentEnterBuildingIndex(), -1 );
////			}
//		}		
//	}
	// 2006-06-13 by ispark, ĳ���� ���°� _NCITYIN �ƴϾ�� �Ѵ�. ĳ���Ͱ� ó���� �ѹ��� _NCITYIN ó���ؾ� �Ѵ�.
	if(g_pD3dApp->m_dwGameState == _SHOP && m_dwState != _NCITYIN)
	{
		m_bPickMove = FALSE;
		m_bEnterShopState = TRUE;
		m_dwState = _STAND;
		m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
		 // 2010-06-08 by dgwoo, ��ý��� �߰�. 
		//ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
		 // 2010-06-08 by dgwoo, ��ý��� �߰�. 
// 		g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
// 		g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
// 		g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
  		// 2010-06-15 by shcho&hslee ��ý��� - ���� �̵� ó��
		g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORY_UNLIMITED, BODYCON_CHARACTER_MODE_STOP);
		g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT, BODYCON_CHARACTER_MODE_STOP);
		g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);

	}
	// 2007-06-12 by bhsohn �̼� �������̽� ������
	//g_pGameMain->m_pMissionInfo->SetEnterShop(m_bEnterShopState);
	g_pGameMain->SetMissionEnterShop(m_bEnterShopState);

	//////////////////////////////////////////////////////////////////////////
	// ĳ���� �̺�Ʈ ������Ʈ
	CheckEventTypeObject(fElapsedTime);

	//////////////////////////////////////////////////////////////////////////
	// ĳ���� �̵�
	CheckState();
	Move_Character(fElapsedTime);

	CheckShadowPos();
	CheckAniTime(fElapsedTime);
	g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_0);

	// ��ũ������ ��ǥ�� ���Ѵ�.
	// 2005-07-29 by ispark
	// ĳ���� Ű ���̷� ����
	D3DXVECTOR3 vPos = m_vPos;
	vPos.y += GetCharacterHeight(m_nUnitNum) + 1.0f;

	//////////////////////////////////////////////////////////////////////////
	// ĳ���� ī�޶� ����
	if(g_pShuttleChild->m_bIsCameraPattern != TRUE)
		CameraMoveTick();

//	DBGOUT("m_vPos %f,  %f,  %f\n",m_vPos.x,m_vPos.y,m_vPos.z);
	g_pD3dApp->CalcObjectSourceScreenCoords(vPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
		g_pShuttleChild->m_nObjScreenX, g_pShuttleChild->m_nObjScreenY, g_pShuttleChild->m_nObjScreenW);
	g_pShuttleChild->m_pMoveChat->Tick();

	// 2006-09-04 by ispark, ���λ��� ���� ��ǳ��
	if(m_pMoveChatShop)
	{
		m_pMoveChatShop->Tick();
	}
}

void CCharacterChild::CheckAniTime(float fElapsedTime)
{
	FLOG( "CShuttleChild::CheckAniTime()" );

	m_pCharacterInfo->Tick(fElapsedTime);
	m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
	m_fCurrentTime = m_pCharacterInfo->GetCurrentCharacterAnimationTime();

	m_pPickingInfo->Tick(fElapsedTime);
//	sprintf( g_pD3dApp->m_strDebug, "0x%016I64x %.3f",GetCurrentBodyCondition(), m_fCurrentTime );
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CShuttleChild::Move_Character(float fElapsedTime)
/// \brief		ĳ���� �̵�
/// \author		ispark
/// \date		2005-07-13 ~ 2005-07-13
/// \warning	
///
/// \param		
/// \return		void
///////////////////////////////////////////////////////////////////////////////
void CCharacterChild::Move_Character(float fElapsedTime)
{
	D3DXVec3Normalize(&m_vVel,&m_vVel);
	D3DXVec3Normalize(&m_vUp,&m_vUp);
	D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);

	//////////////////////////////////////////////////////////////////////////
	// ĳ���� �̵�
	// Picking�� ���� ó��
	D3DXVECTOR3 vPos = m_vPos;
	vPos.y += CHARACTER_HEIGHT;			// �浹�˻�� ���� ���� ���� �ø���.

	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	
	// 2010-11-29 by jskim, �� ������ �̵��ӵ� ����
	//if( COMPARE_RACE( myShuttleInfo.Race,RACE_OPERATION|RACE_GAMEMASTER) )
	// end 2010-11-29 by jskim, �� ������ �̵��ӵ� ����
	//{	// ������ ����� �̵� �ӵ� ���� ó��.
		float fTempSpeed = m_fCharacterAddSpeed;
		
		const float a_fMinAddSpeed = .0f;
		const float a_fMaxAddSpeed = 150.0f;
		
	// 2010-11-29 by jskim, �� ������ �̵��ӵ� ����
	//if ( GetAsyncKeyState ( VK_ADD ) )				// Ű�е� '+'�� �ӵ� ����.
	//	m_fCharacterAddSpeed += 1.0f;
	//else if ( GetAsyncKeyState ( VK_SUBTRACT ) )	// Ű�е� '-'�� �ӵ� ����.
	//	m_fCharacterAddSpeed -= 1.0f;
	//else if ( GetAsyncKeyState ( VK_DECIMAL ) )
	//	m_fCharacterAddSpeed += 50.0f;
	//else if ( GetAsyncKeyState ( VK_DIVIDE ) )		// Ű��� '/'�� �ʱ�ȭ.
	//	m_fCharacterAddSpeed = .0f;
		if (COMPARE_RACE(myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
	{
		// 2013-06-10 by bhsohn ��������Ʈ ������ - ���� '�ɸ����̵��ӵ�' ����
		if ( GetAsyncKeyState ( VK_ADD ) )				// Ű�е� '+'�� �ӵ� ����.
			m_fAdminModeSpeed += 1.0f;
		else if ( GetAsyncKeyState ( VK_SUBTRACT ) )	// Ű�е� '-'�� �ӵ� ����.
			m_fAdminModeSpeed -= 1.0f;
		else if ( GetAsyncKeyState ( VK_DECIMAL ) )
			m_fAdminModeSpeed += 50.0f;
		else if ( GetAsyncKeyState ( VK_DIVIDE ) )		// Ű��� '/'�� �ʱ�ȭ.
			m_fAdminModeSpeed = .0f;				
		// END 2013-06-10 by bhsohn ��������Ʈ ������ - ���� '�ɸ����̵��ӵ�' ����
	}
	// end end 2010-11-29 by jskim, �� ������ �̵��ӵ� ����

	// 2013-06-10 by bhsohn ��������Ʈ ������ - ���� '�ɸ����̵��ӵ�' ����
	{
		float fcharSpeed = GetParamFactor_DesParam( g_pShuttleChild->m_paramFactor, DES_RARE_ITEM_PARTNER_SPEED );
		if(0 != fcharSpeed)
		{
			m_fCharacterAddSpeed = GetCharacterSteps( g_pShuttleChild->m_myShuttleInfo.PilotFace ) * fcharSpeed;			
		}
		else
		{
			m_fCharacterAddSpeed = 0.0f;			
		}
		// todo : here is walking speed
#ifdef ARESBETA
		m_fCharacterAddSpeed = GetCharacterSteps(g_pShuttleChild->m_myShuttleInfo.PilotFace) * 1.5f;
#endif

		if(m_fAdminModeSpeed > 0.0f)
		{
			m_fCharacterAddSpeed += m_fAdminModeSpeed;
		}
	}
	// END 2013-06-10 by bhsohn ��������Ʈ ������ - ���� '�ɸ����̵��ӵ�' ����
		
	m_fCharacterAddSpeed = max ( m_fCharacterAddSpeed , a_fMinAddSpeed );
	m_fCharacterAddSpeed = min ( m_fCharacterAddSpeed , a_fMaxAddSpeed );
				
	// 2013-06-10 by bhsohn ��������Ʈ ������ - ���� '�ɸ����̵��ӵ�' ����
	// 2010-11-29 by jskim, �� ������ �̵��ӵ� ����
// 	if( m_bPetWearItem )
// 	{
// 		if( m_bSetSpeed == FALSE )
// 		{
// 		// 2010-12-21 by jskim, ���� �̵� �ӵ� ���� ������ ����
// 			//m_fCharacterAddSpeed += GetCharacterSteps( g_pShuttleChild->m_myShuttleInfo.PilotFace ) * 0.1f;
// 			float fcharSpeed = GetParamFactor_DesParam( g_pShuttleChild->m_paramFactor, DES_RARE_ITEM_PARTNER_SPEED );
// 			m_fCharacterAddSpeed += GetCharacterSteps( g_pShuttleChild->m_myShuttleInfo.PilotFace ) * fcharSpeed;			
// 
// 		// end 2010-12-21 by jskim, ���� �̵� �ӵ� ���� ������ ����
// 			m_bSetSpeed = TRUE;
// 		}
// 	}
// 	else
// 	{
// 		m_fCharacterAddSpeed = 0.0f;
// 		m_bPetWearItem = FALSE;
// 	}
	// end 2010-11-29 by jskim, �� ������ �̵��ӵ� ����	
	// END 2013-06-10 by bhsohn ��������Ʈ ������ - ���� '�ɸ����̵��ӵ�' ����

	// 2013-06-10 by bhsohn ��������Ʈ ������ - ���� '�ɸ����̵��ӵ�' ����
// 2012-06-25 by jhahn, �� ������ �̵��ӵ� ���� ����߰�
#ifdef S_OPTION_PARTNER__JHAHN		
// 	tPET_CURRENTINFO *pPetCurrentInfo = NULL;
// 	tPET_LEVEL_DATA *psPetLevelDataPrev = NULL;
// 	INVEN_DISPLAY_INFO *pInvenDisplayInfo = g_pGameMain->m_pInven->rtnPtr_AttachmentItemInfo ( POS_PET );
// 	BOOL runSpeed = FALSE;
// 	
// 			
// 		if ( pInvenDisplayInfo )
// 		{
// 			pPetCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData ( pInvenDisplayInfo->pItem->UniqueNumber );
// 			psPetLevelDataPrev = g_pDatabase->GetPtr_PetLevelData ( pPetCurrentInfo->PetIndex , pPetCurrentInfo->PetLevel );			
// 			CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_PET ); 	
// 
// 			for(int i =0 ; i<6 ;i++)
// 			{
// 				CItemInfo* SoketItem = g_pStoreData->FindItemInInventoryByUniqueNumber(pPetCurrentInfo->PetSocketItemUID[i] ); 
// 				
//  				if(SoketItem == NULL)
// 					continue;
// 				
// 				else if( SoketItem->GetItemInfo() && SoketItem->GetItemInfo()->ArrDestParameter[0] == DES_PET_SOCKET_ITEM_SPEED )
//  				{
//  					runSpeed = TRUE;
// 					break;
//  				}	
// 				else
// 					runSpeed = FALSE;
// 			}
// 			if (runSpeed == FALSE)
// 			{
// 				PetoptionSpeed = NULL;
// 			}
// 			
// 			if ((pPetCurrentInfo->PetLevel != PetoptionSpeed) && runSpeed)
// 			{
// 				int TempLevel;
// 				if (pPetCurrentInfo->PetLevel / 5 <= 0)
// 				{
// 					TempLevel = 1 ;						
// 				}
// 				else 
// 				{
// 					TempLevel = pPetCurrentInfo->PetLevel;
// 				}
// 				m_fCharacterAddSpeed = (((GetCharacterSteps( g_pShuttleChild->m_myShuttleInfo.PilotFace ) * 0.1f ) + (pPetCurrentInfo->PetLevel/5 ) * 2.1f) );		
// 				PetoptionSpeed = pPetCurrentInfo->PetLevel;
// 			}
// 		
// 		}
// 		else
// 		{
// 
// 			PetoptionSpeed = NULL;
// 			runSpeed = FALSE;
// 		}
		// END 2013-06-10 by bhsohn ��������Ʈ ������ - ���� '�ɸ����̵��ӵ�' ����
#endif
//end 2012-06-25 by jhahn, �� ������ �̵��ӵ� ���� ����߰�
	
	
	g_cCustomOptimizer.FastMove( m_fCharacterAddSpeed );

	if ( fTempSpeed != m_fCharacterAddSpeed )
	{
		// 2010-11-29 by jskim, �� ������ �̵��ӵ� ����
// 		if ( g_pD3dApp->m_pChat )
// 		{
// 			char szTemp[256] = {0, };
// 			sprintf ( szTemp , "Change Character Add MoveSpeed : (%.2f)" , m_fCharacterAddSpeed );
// 			g_pD3dApp->m_pChat->CreateChatChild ( szTemp , COLOR_SYSTEM );
// 		}
		if (COMPARE_RACE(myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
		{
			if ( g_pD3dApp->m_pChat )
			{
				char szTemp[256] = {0, };
				sprintf ( szTemp , "Change Character Add MoveSpeed : (%.2f)" , m_fCharacterAddSpeed );
				
				g_pD3dApp->m_pChat->CreateChatChild ( szTemp , COLOR_SYSTEM );
			}
		}
		// end 2010-11-29 by jskim, �� ������ �̵��ӵ� ����
			CBodyConditionInfo *pcGetBodyCondition = m_pCharacterInfo->FindBodyConditionInfo ( BODYCON_CHARACTER_MODE_RUN );

			if ( pcGetBodyCondition )
			{

				float fSetFrame = 1.0f + ( 2.0f * ( m_fCharacterAddSpeed / a_fMaxAddSpeed ) );


				pcGetBodyCondition->SetFrameSpeed ( fSetFrame );
			}
	}

	if(m_dwState == _STAND)
	{
		m_fCharacterSpeed = 0.0f;
	}
	if(m_bPickMove)
	{
		D3DXMATRIX  matTemp;
		double dRadian = 0.0f;
		float dot = 0.0f;
		D3DXVECTOR3 vCrossVec;
		D3DXVECTOR3 vNewDir = m_vPickDir;

		D3DXVec3Cross(&vCrossVec, &m_vVel, &vNewDir);
		D3DXVec3Normalize(&vNewDir, &vNewDir);
		dot = D3DXVec3Dot(&m_vVel, &vNewDir);

		if(m_dwState != _RUN)
		{
			if(1.0f > dot)
			{
				if(0.0f > vCrossVec.y)
				{
					//������ ȸ��  
					dRadian -= ACOS(dot);
				}
				else	
				{
					// ���� ȸ��
					dRadian += ACOS(dot);
				}

				D3DXMatrixRotationY(&matTemp, (float)dRadian);
				D3DXVec3TransformCoord(&m_vVel, &m_vVel, &matTemp);
			}
			// �ϴ� �׽�Ʈ�� �ٴ� �ൿ�� �ش�.
			// 2005-07-29 by ispark
			// Picking ��� �Ÿ��� ���� ����
			m_fOldLength = CHARACTER_PICKING_LENGTH;
			m_dwState = _RUN;
			// 2005-07-29 by ispark
			// �ٱ� �ٵ� ������� WndProc���� ����� �Ű��.
			m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_RUN);
			// 2010-06-08 by dgwoo, ��ý��� �߰�. �Ʒ��� �Լ��� ����. 
			ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_RUN);
// 			g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_RUN);
// 			g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_RUN);
// 			g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_RUN);
			// 2010-06-08 by dgwoo, ��ý��� �߰�.
		}
		// �� ���� - ĳ���� �̵� (���콺 �� Ŭ��)�� Tick() �����Ǵ� ���� ����. 2010. 10. 08. by jskim.
		if(m_dwState == _RUN)
		{
			D3DXVECTOR3 vMovingVec;
			vMovingVec = m_vPicking - m_vPos;
			FLOAT lenght = D3DXVec3Length(&vMovingVec);

			// 2005-07-29 by ispark
			// ���� �Ÿ� ���� 5.0f -> 10.0f
			if(10.0f >= lenght)
			{
				// �ѹ��� ȸ�� �˻��� �Ÿ� 0.5���� ���� ��Ų��.
				if(1.0f > dot && 0.5f < lenght)
				{
					if(0.0f > vCrossVec.y)
					{
						//������ ȸ��  
						dRadian -= ACOS(dot);
					}
					else	
					{
						// ���� ȸ��
						dRadian += ACOS(dot);
					}

					D3DXMatrixRotationY(&matTemp, (float)dRadian);
					D3DXVec3TransformCoord(&m_vVel, &m_vVel, &matTemp);
				}
			}
			
			if((0.5f > lenght) || (m_fOldLength < lenght))
				{
//					g_pShuttleChild->SendFieldSocketChangeBodyCondition(g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_CHRACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, ��ý��� �߰�. 
					m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-15 by shcho&hslee ��ý��� - ���� �̵� ó��
					//ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORY_UNLIMITED, BODYCON_CHARACTER_MODE_STOP);
					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT, BODYCON_CHARACTER_MODE_STOP);
					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, ��ý��� �߰�. 
					m_dwState = _STAND;
					m_bPickMove = FALSE;
//					m_vPos = m_vPicking;
				}
			else
			{
				// ĳ���ʹ�  Picking���� �����δ�.
				CheckMoveRate(fElapsedTime);
				m_vPos += (m_vNextPos - m_vPos) * fElapsedTime;
				m_fOldLength = lenght;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ������Ʈ �浹
	// ���⼭ ������Ʈ�� ���� �����̴�.
	// ĳ���ʹ� ������Ʈ ���� �����Ѵٴ� ���Ƿ� �����Ѵ�. ���� ���� ����
	// 2006-07-14 by ispark, ĳ���� ���� �߿��� ������Ʈ �浹 �˻縦 ���Ѵ�.
	if(m_dwState == _WARP)
	{
		// ���� �߿� �̵�
		m_vPos += m_vMoveVel * fElapsedTime;
	}
	else if(!m_bStartPositionEvent)						// 2006-07-20 by ispark, ���� ��ġ�� �̺�Ʈ ��ġ�� �ƴ϶��
	{
		float fTempHeight = 0.0f;
		float fObjHeight = -DEFAULT_COLLISION_DISTANCE;
		
		COLLISION_RESULT GroundCollResult;
		D3DXMATRIX matTemp;
		D3DXVECTOR3 vSide(0, 0, 1), vUp(0, 1, 0);
		D3DXMatrixLookAtLH(&matTemp,&vPos,&(vPos - vUp),&vSide);
	//	fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,vPos).fDist - CHARACTER_HEIGHT;
		GroundCollResult = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,vPos);

		fTempHeight = GroundCollResult.fDist - CHARACTER_HEIGHT;

		if(GroundCollResult.vNormalVector.y > 0.7f)
		{
			if(fTempHeight<m_vPos.y)
				m_vPos.y += -fTempHeight;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// �̵� ���� ����
	// ���� ����� ������Ʈ���� �Ÿ��� ��� ������ ����Ѵ�.
	// _NCITYIN ó�� ������ ���ö��� �̵� ���� ���� �˻縦 ���ϴ�.
	// �Ұ�� ĳ���͸� ������Ʈ ���� ���µ� ������ ����
	// ������ ���� Y���� ������ ���� �ް� Ŭ���̾�Ʈ���� Y���� ���
	// �׷��� ���� ���̰� ���� �� �ִ� ����� �� ���� ������ ����
	// ĳ���Ͱ� ��Ÿ���� ��ġ���� �������� ������Ʈ�� ���ٴ� �����Ͽ� ����
	// 2006-07-14 by ispark, ĳ���� ���� �߿��� ������Ʈ �浹 �˻縦 ���Ѵ�.
	if(m_dwState != _NCITYIN && m_dwState != _WARP)
	{
		float fCollResult;
		COLLISION_RESULT CollResult;
		D3DXMATRIX matArray[2];
		D3DXVECTOR3 vCollSide,vVel,vFrontPos, vDownPos;
		
		vFrontPos = vPos;			// ��� Ű���̸� ��������
		vDownPos = vPos;			// ������ ����ϱ� ���ؼ� �� ��ġ ���� �� ���� ��ǥ �Է�
		vDownPos += m_vVel * 5;

		D3DXVec3Cross(&vCollSide,&m_vUp,&m_vVel);
		
		D3DXMatrixLookAtLH(&matArray[0],&vFrontPos,&(vFrontPos + m_vVel),&m_vUp);		// ��
		D3DXMatrixLookAtLH(&matArray[1],&vDownPos,&(vDownPos - m_vUp),&vCollSide);		// �Ʒ�
		
		float fFrontMove = m_fCharacterSpeed*fElapsedTime;
		float size[2] = {7 + fFrontMove, CHARACTER_HEIGHT + 4.0f};	// ���� ũ��	��, �Ʒ�
		
		// ������Ʈ�� �浹 ó�� 2004.06.18 jschoi
		
		if(	/*m_bCollObjectCheck && */
			g_pScene &&
			g_pScene->m_byMapType != MAP_TYPE_TUTORIAL &&
			g_pScene->m_pObjectRender)
		{
			// ����
			CollResult = g_pScene->m_pObjectRender->CheckCollMesh(matArray[0], vPos);	
			// 2005-07-29 by ispark
			// ������Ʈ�ȿ��� ������ ���ϴ� ���� ���� ó��(������ ��)
//			DBGOUT("���� %f %f %f\n", m_vVel.x, m_vVel.y, m_vVel.z);
//			DBGOUT("�븻 %.2f %.2f %.2f\n", CollResult.vNormalVector.x, CollResult.vNormalVector.y, CollResult.vNormalVector.z);
//			DBGOUT("�Ÿ� %f\n", CollResult.fDist);
			float dot = D3DXVec3Dot(&m_vVel, &CollResult.vNormalVector);
			if(0.0f >= dot)
			{
//				DBGOUT("���� %f %f %f\n", m_vVel.x, m_vVel.y, m_vVel.z);
//				DBGOUT("�븻 %f %f %f\n", CollResult.vNormalVector.x, CollResult.vNormalVector.y, CollResult.vNormalVector.z);
//				if(CollResult.fDist < size[0] && m_dwState != _STAND)
				// 2005-08-04 by ispark
				// ��� ������ ���
				if((CollResult.fDist < size[0] && CollResult.vNormalVector.y <= 0.7f) && m_dwState != _STAND)
				{
					// size[0] �ȿ� ������ �浹
					m_vPos = vPos;
					m_vPos.y -= CHARACTER_HEIGHT;

					m_dwState = _STAND;
					m_bPickMove = FALSE;
//					g_pShuttleChild->SendFieldSocketChangeBodyCondition(g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_CHRACTER_MODE_STOP);
					m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, ��ý��� �߰�. �Ʒ��� �Լ��� ����. 
					ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
					
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, ��ý��� �߰�. 
				}
			}

			// ����
			fCollResult = g_pScene->m_pObjectRender->CheckCollMesh(matArray[1], m_vPos).fDist - CHARACTER_HEIGHT;
			if(fCollResult > size[1] && m_dwState != _STAND)
			{
				// size[1] ��� ��ġ���� ũ�� ����
				m_vPos = vPos;
				m_vPos.y -= CHARACTER_HEIGHT;

				m_dwState = _STAND;
				m_bPickMove = FALSE;
//				g_pShuttleChild->SendFieldSocketChangeBodyCondition(g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_CHRACTER_MODE_STOP);
				m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
				// 2010-06-08 by dgwoo, ��ý��� �߰�. �Ʒ��� �Լ��� ����. 
				ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
// 				g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
// 				g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
// 				g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
				// 2010-06-08 by dgwoo, ��ý��� �߰�.
			}
			else if(m_vPos.y < vPos.y - CHARACTER_HEIGHT - 5.0f)		// ���� �������� ��ġ�ΰ�
			{															// 5.0f�� ������ �ƴѰ��� Ȯ���ϱ� ���ؼ�
				// ��� ��ġ�̸� �ε巴�� ��������.
				// 2005-08-04 by ispark
				// ���� ������ ���� �����δ�.
				D3DXVECTOR3 vVel = m_vNextPos - vPos;
				D3DXVec3Normalize(&vVel, &vVel);
				m_vPos = vPos + (vVel) * fElapsedTime;
				m_vPos.y = vPos.y - CHARACTER_HEIGHT - (CHARACTER_HEIGHT * fElapsedTime * 6.0f);
			}
		}
	}
	else if(m_dwState == _NCITYIN)
	{
		// Move_Character()���� ó������ �����ؾ� �ϴ� �κ�
		// _NCITYIN �̶�� _STAND�� �ٲ۴�. 
		// _NCITYIN �ѹ��� ���
		m_dwState = _STAND;
		m_bPickMove = FALSE;
		m_bStartPositionEvent = FALSE;
		// ī�޶� �ʱ� ����
		// InitCharacterData()���� ���� �ʴ� ������ ������ ī�޶� ��ġ�� ������ ����
		g_pCamera->SetCamDistance(CHARACTER_CAMERA_DISTANCE);		// �Ÿ�
		g_pD3dApp->SetCamPosInit();
	}

	//////////////////////////////////////////////////////////////////////////
	// �̺�Ʈ ���� üũ - Shop
	int nCheckX = ((int)m_vPos.x)/TILE_SIZE;
	int nCheckZ = ((int)m_vPos.z)/TILE_SIZE;
	if((g_pGround->m_pTileInfo[nCheckX*g_pGround->m_projectInfo.sYSize + nCheckZ].bEventType != EVENT_TYPE_ENTER_BUILDING &&
		g_pGround->m_pTileInfo[nCheckX*g_pGround->m_projectInfo.sYSize + nCheckZ].bEventType != EVENT_TYPE_ENTER_BUILDING_BAZAAR) &&
		m_dwState != _STAND)
	{
		m_vPos = vPos;
		m_vPos.y -= CHARACTER_HEIGHT;
		
		m_dwState = _STAND;
		m_bPickMove = FALSE;
//		g_pShuttleChild->SendFieldSocketChangeBodyCondition(g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_CHRACTER_MODE_STOP);
		m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
		// 2010-06-08 by dgwoo, ��ý��� �߰�. �Ʒ��� �Լ��� ����. 
		ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
		//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
		//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
		//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
		// 2010-06-08 by dgwoo, ��ý��� �߰�.

	}

	m_bBazaarEventPos = FALSE;
	if(g_pGround->m_pTileInfo[nCheckX*g_pGround->m_projectInfo.sYSize + nCheckZ].bEventType == EVENT_TYPE_ENTER_BUILDING_BAZAAR)
	{
		m_bBazaarEventPos = TRUE;
	}
	//////////////////////////////////////////////////////////////////////////
	SetMatrix_Move_Character(fElapsedTime);
	//////////////////////////////////////////////////////////////////////////
	// ShuttleChild�� ���� �Է��ؾ� �Ѵ�.
	g_pShuttleChild->m_vPos = m_vNextPos = m_vPos;
	g_pShuttleChild->m_vVel = m_vVel;
	g_pShuttleChild->m_vUp = m_vUp;
	g_pShuttleChild->m_vSideVel;

	// 2005-07-28 by ispark
	// Move ��Ŷ�� ������ Up ���͸� Metrix���� �����´�. 
	g_pShuttleChild->SetMatrix_Move_Ground(fElapsedTime);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CShuttleChild::SetMatrix_Move_Character(float fElapsedTime)
/// \brief		ĳ���� ��Ʈ���� ����
/// \author		ispark
/// \date		2005-07-13 ~ 2005-07-13
/// \warning	
///
/// \param		
/// \return		void
///////////////////////////////////////////////////////////////////////////////
void CCharacterChild::SetMatrix_Move_Character(float fElapsedTime)
{
	D3DXVec3Normalize(&m_vUp,&m_vUp);
	D3DXVec3Normalize(&m_vVel,&m_vVel);
	D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);
	D3DXVec3Cross(&m_vVel,&m_vSideVel,&m_vUp);

	D3DXMatrixLookAtRH(&m_mMatrix, &m_vPos, &(m_vPos + m_vVel), &m_vUp);
	D3DXMatrixInverse(&m_mMatrix, nullptr, &m_mMatrix );

	D3DXMATRIX matTemp;
	D3DXMatrixScaling(&matTemp, CHARACTER_SCALE, CHARACTER_SCALE, CHARACTER_SCALE);
	m_mMatrix = matTemp * m_mMatrix;
}

void CCharacterChild::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CCharacterChild::WndProc()" );
	// 2006-07-14 by ispark, ������ ���ν��� ����
	if(m_dwState == _WARP)
		return;
	
	float fElapsedTime = g_pD3dApp->GetElapsedTime();

	if(!g_pD3dApp->m_bChatMode)// && m_bMoveInputType == 0)
	{
		switch(uMsg)
		{
		case WM_KEYDOWN:
			{
				switch(lParam)
				{
				case WM_KEYDOWN_LPARAM_Z:
					{
//						if( g_pShuttleChild->m_pClientParty->GetNtoNCallState() ||
//							g_pGameMain->m_pCommunity->GetGuild()->GetNtoNCallState())
//						{
//							// 2008-10-14 by bhsohn VOIP����							
////							COPYDATASTRUCT	copyData;
////							memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
////							AV_MSG_WM_COPYDATA voIP(_NtoNCall, AV_MT_TALK, 0);
////							copyData.dwData = (DWORD)voIP;
////							copyData.lpData = NULL;
////							copyData.cbData = 0;
////							::SendMessage(g_pD3dApp->m_VOIPState.m_hWndAtumVoIPClient, WM_COPYDATA, (WPARAM)g_pD3dApp->GetHwnd(), (LPARAM)&copyData);
//							//g_pGameMain->ChangeVOIPMode();
//						}
					}
					break;
				}

//				switch(wParam)
//				{
//				case VK_ADD:
//					{
//						m_fCharacterMoveRate += 0.1f;
//					}
//					break;
//				case VK_SUBTRACT:
//					{
//						m_fCharacterMoveRate -= 0.1f;
//					}
//					break;
//				}
			}
		case WM_KEYUP:
			{
				switch(lParam)
				{
				case WM_KEYUP_LPARAM_Z:
					{
						// 2008-10-14 by bhsohn VOIP����							
//						if( g_pShuttleChild->m_pClientParty->GetNtoNCallState() == TRUE ||
//							g_pGameMain->m_pCommunity->GetGuild()->GetNtoNCallState() == TRUE)
//						{
//							COPYDATASTRUCT	copyData;
//							memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
//							AV_MSG_WM_COPYDATA voIP(_NtoNCall, AV_MT_TALK_CANCEL, 0);
//							copyData.dwData = (DWORD)voIP;
//							copyData.lpData = NULL;
//							copyData.cbData = 0;
//							::SendMessage(g_pD3dApp->m_VOIPState.m_hWndAtumVoIPClient, WM_COPYDATA, (WPARAM)g_pD3dApp->GetHwnd(), (LPARAM)&copyData);
//						}
						// end 2008-10-14 by bhsohn VOIP����							
					}
				}
				break;
			}
		case WM_MBUTTONDOWN:
			{				
			}
			break;
		case WM_MBUTTONUP:
			{
			}
			break;
		case WM_MOUSEWHEEL:
			{
				// 2005-08-18 by ispark
				// ĳ���� ī�޶� ��
				m_bMouseWheel = TRUE;
				float nDist = g_pCamera->GetCamDistance();
				float fCollDist = g_pCamera->GetCollDistance();			// 2005-08-19 by ispark

				if((int)wParam > 0)
				{
					// 2005-08-19 by ispark
					// �浹 �Ÿ� ���� �� ī�޶� �ڷ� ���� ��� ����
					if(fCollDist != 0 && fCollDist < nDist)
					{
						break;
					}

					float fDistMax = CHARACTER_CAMERA_DISTANCE_MAX;					
#ifdef _DEBUG
					fDistMax = 600.0f;
#endif
					if(nDist + CHARACTER_CAMERA_WHEEL_DISTANCE > fDistMax)
					{
						nDist = fDistMax;
						g_pCamera->SetCamDistance(nDist);
					}
					else
					{
						g_pCamera->SetCamDistance(nDist + CHARACTER_CAMERA_WHEEL_DISTANCE);
					}
				}
				else
				{
					// 2005-08-19 by ispark
					// ������ �� ���� �浹 �Ÿ� ����(���� �Ÿ� ����)
					if(fCollDist != 0 && fCollDist < nDist)
					{
						if(fCollDist < CHARACTER_CAMERA_DISTANCE_MIN)
						{
							g_pCamera->SetCamDistance(fCollDist);
							break;
						}

						nDist = fCollDist;
					}

					if(nDist - CHARACTER_CAMERA_WHEEL_DISTANCE < CHARACTER_CAMERA_DISTANCE_MIN)
					{
						nDist = CHARACTER_CAMERA_DISTANCE_MIN;
						g_pCamera->SetCamDistance(nDist);
					}
					else
					{
						g_pCamera->SetCamDistance(nDist - CHARACTER_CAMERA_WHEEL_DISTANCE);
					}
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				POINT pt;
				GetCursorPos(&pt);
				ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
				CheckMouseReverse(&pt);
	
				// Pick
				// 2005-08-25 by ispark, �˾� �޴� �����¶��
				// 2006-06-20 by ispark, ������ ���� ������ ��ٸ��� ���̶�� ĳ���Ͱ� �������� ���ϰ� �Ѵ�.
				// (�������̽� â�� ������ Picking�� �Ǳ� �����̴�.) 
				// 2007-12-17 by bhsohn ������ ������ �� �ȿ��� ���� �����·�
				//if(g_pGameMain->m_bMenuLock || g_pD3dApp->m_bRequestEnable == FALSE)
				if(g_pGameMain->m_bMenuLock || g_pD3dApp->IsLockMode() == FALSE)
					break;

				// 2005-09-14 by ispark
				// �⺻�������̽� ���� Ŭ���� ���´�.
				if(g_pInterface->m_bShowInterface && 
					(pt.x >= 0 && pt.x <= g_pD3dApp->GetBackBufferDesc().Width) &&
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
					(pt.y >= g_pD3dApp->GetBackBufferDesc().Height - 70) || // #define CHATBOX_FIELD_SHOWCHATBOX_INIT_HEIGHT			70 -> INFGameMainChat�� ����
#else				 
					(pt.y >= g_pD3dApp->GetBackBufferDesc().Height - 39) ||
#endif
					((pt.y >= g_pD3dApp->GetBackBufferDesc().Height - 51) && 
					(pt.x >= g_pD3dApp->GetBackBufferDesc().Width / 2 - 117 &&
					pt.x <= g_pD3dApp->GetBackBufferDesc().Width / 2 + 117)))
				{
					break;
				}
					
				m_dwState = _STAND;
				
				COLLISION_RESULT Coll_Result_Pick;
				Coll_Result_Pick = g_pScene->m_pObjectRender->CheckPickMesh(pt.x, pt.y);

				// 2007-07-27 by bhsohn ������ī �ʸ� ���� �ν� ���� ����
				float fSearchLength = CHARACTER_SHOP_SERCH_DISTANCE;
				// 2009. 10. 14 by jskim ������ī ���� 
// 				if(IS_NORMAL_CITY_MAP_INDEX(g_pShuttleChild->GetMyShuttleInfo().MapChannelIndex.MapIndex))
// 				{
// 					fSearchLength = CHARACTER_SHOP_SERCH_FREESKA_DISTANCE;
// 				}
				//end 2009. 10. 14 by jskim ������ī ���� 
				//if(!CheckEnterShopNPC(&pt, CHARACTER_SHOP_SERCH_DISTANCE, TRUE))
				if(!CheckEnterShopNPC(&pt, fSearchLength, TRUE))
				{
					// Picking�� �ٴ����� ����
	//				DBGOUT("���� %.2f %.2f %.2f\n", Coll_Result_Pick.vPicking.x, Coll_Result_Pick.vPicking.y, Coll_Result_Pick.vPicking.z);
	//				DBGOUT("�븻 %.2f %.2f %.2f\n", Coll_Result_Pick.vNormalVector.x, Coll_Result_Pick.vNormalVector.y, Coll_Result_Pick.vNormalVector.z);
					if(Coll_Result_Pick.vNormalVector.y > 0.7f)
					{
						m_bPickMove = TRUE;
						m_vPicking = Coll_Result_Pick.vPicking;
						D3DXVECTOR3 vPosTemp = m_vPos;
						D3DXVECTOR3 vPickTemp = m_vPicking;
						// Picking ��ġ ����
						SetMatrix_Picking(m_vPicking, Coll_Result_Pick.vNormalVector);
						m_pPickingInfo->SetSingleBodyConditionMatrix(BODYCON_LANDED_MASK, m_mPickMatrix);
						
						vPosTemp.y = vPickTemp.y = 0.0f;
						m_vPickDir = vPickTemp - vPosTemp;
					}
				}
			}
			break;
		case WM_LBUTTONUP:
			{
			}
			break;
		case WM_RBUTTONDOWN:
			{
				if(g_pCamera->GetCamType() == CAMERA_TYPE_NORMAL)
				{
					g_pCamera->m_bIsCamControl = TRUE;
					POINT pt;
					pt.x = LOWORD(lParam);
					pt.y = HIWORD(lParam);
					CheckMouseReverse(&pt);

					m_pOldMousePoint = pt;
				}
				
				m_bRButtonState = TRUE;
			}
			break;
		case WM_RBUTTONUP:
			{
				// ī�޶� ��ȯ ��� ����
				if(g_pD3dApp->m_pCamera->m_bIsCamControl)
				{
					g_pD3dApp->m_pCamera->m_bIsCamControl = FALSE;
				}
				m_bRButtonState = FALSE;
			}
			break;
		case WM_MOUSEMOVE:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				// 2007-07-27 by bhsohn ������ī �ʸ� ���� �ν� ���� ����
				//CheckEnterShopNPC(&pt, CHARACTER_SHOP_SERCH_DISTANCE);
				float fSearchLength = CHARACTER_SHOP_SERCH_DISTANCE;
				// 2009. 10. 14 by jskim ������ī ���� 
				//if(IS_NORMAL_CITY_MAP_INDEX(g_pShuttleChild->GetMyShuttleInfo().MapChannelIndex.MapIndex))
				//{
				//	fSearchLength = CHARACTER_SHOP_SERCH_FREESKA_DISTANCE;
				//}				
				//end 2009. 10. 14 by jskim ������ī ����  
				CheckEnterShopNPC(&pt, fSearchLength);
				
				// end 2007-07-27 by bhsohn ������ī �ʸ� ���� �ν� ���� ����

				

				m_bMouseMove = TRUE;

//				if(g_pCamera->GetCamType() == CAMERA_TYPE_NORMAL)
//				{
//					if( g_pCamera->GetCamType() == CAMERA_TYPE_NORMAL &&
//						g_pD3dApp->m_pCamera->m_bIsCamControl && 
//						m_bRButtonState)
//					{
//						float fDirY = 0.0f, fDirX = 0.0f;
//						fDirY = m_pOldMousePoint.y - pt.y;
//						fDirX = m_pOldMousePoint.x - pt.x;
//						if(fDirY != 0)
//						{
//							// ĳ���� ī�޶� ȸ���� ������Ʈ�� ����� �ϴµ� �������� ���콺 �̵��� Ŀ�� �׷���.
//							// �׷��� ������ �ξ���.
//							if(20 < fDirY)
//								fDirY = 20;
//							if(-20 > fDirY)
//								fDirY = -20;
//						}
//						g_pCamera->SetAngleY(fDirY);
//						
//						if(fDirX != 0)
//						{
//							if(20 < fDirX)
//								fDirX = 20;
//							if(-20 > fDirX)
//								fDirX = -20;					
//						}
//						g_pCamera->SetAngleX(fDirX);
//					}
//				}
//				m_pOldMousePoint = pt;
			}
			break;
		}

	}
}

void CCharacterChild::CheckShadowPos()
{
	// ���⼭ ��ġ�� �׸��ڸ� �Ѹ� ����� ���Ѵ�. �������� �밢�� �������� �Ÿ��̴�.
	m_vShadowPos[0] = m_vPos + CHARACTER_SHADOW_SCALE * (m_vVel - m_vSideVel);
	m_vShadowPos[1] = m_vPos + CHARACTER_SHADOW_SCALE * (m_vVel + m_vSideVel);
	m_vShadowPos[2] = m_vPos - CHARACTER_SHADOW_SCALE * (m_vVel - m_vSideVel);
	m_vShadowPos[3] = m_vPos - CHARACTER_SHADOW_SCALE * (m_vVel + m_vSideVel);

	for(int i = 0 ; i < 4 ; i++)
	{
		m_vShadowPos[i].y += CHARACTER_HEIGHT;
		float fTempHeight = 0.0f;
		float fObjHeight = -DEFAULT_COLLISION_DISTANCE;

		D3DXMATRIX matTemp;
		D3DXVECTOR3 vSide(0, 0, 1), vUp(0, 1, 0);
		D3DXMatrixLookAtLH(&matTemp,&m_vShadowPos[i],&(m_vShadowPos[i] - vUp),&vSide);
		fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,m_vShadowPos[i]).fDist - CHARACTER_HEIGHT;

		if(fTempHeight <= 1.0f && fTempHeight >= -1.0f)
		{
			m_vShadowPos[i].y += -fTempHeight;
		}
		else
		{
			if(fTempHeight > 0) 
			{
				m_vShadowPos[i].y += -fTempHeight;
			}
			else
			{
				float fRealObjectHeight = m_vShadowPos[i].y - fTempHeight;
				if( fRealObjectHeight > m_vShadowPos[i].y )
				{
					m_vShadowPos[i].y = fRealObjectHeight;
				}
			}
		}

		// �׸��ڴ� Z���۸� ����ϹǷ� �׸��� ���̸� 0.1f��ŭ �ø���.
		if(g_pD3dApp->m_nDephbias == -1)
		{
			m_vShadowPos[i].y -= (CHARACTER_HEIGHT - 0.1f);	
		}
		else
		{
			m_vShadowPos[i].y -= CHARACTER_HEIGHT;
		}
	}

	SPRITEVERTEX* pV;
	m_pVBShadow->Lock( 0, 0, (void**)&pV,	0 );
	pV[0].p = m_vShadowPos[3]; 
	pV[1].p = m_vShadowPos[0]; 
	pV[2].p = m_vShadowPos[2]; 
	pV[3].p = m_vShadowPos[1];
	m_pVBShadow->Unlock();
}

void CCharacterChild::CheckMoveRate(float fElapsedTime)
{
	if(m_dwState == _WALK)
	{
		m_fCharacterSpeed = CHARACTER_WALK;
	}
	else if(m_dwState == _RUN)
	{
//		// 2006-02-09 by ispark, ����, ���� ������ �ӵ� �ٸ�
//		if(g_pShuttleChild->m_myShuttleInfo.PilotFace < 100)
//		{
//			m_fCharacterSpeed = CHARACTER_WOMAN_RUN;		
//		}
//		else
//		{
//			m_fCharacterSpeed = CHARACTER_MAN_RUN;		
//		}
		// 2006-11-02 by ispark, ĳ���� ���� ���������� ����
		m_fCharacterSpeed = GetCharacterSteps(g_pShuttleChild->m_myShuttleInfo.PilotFace);

		m_fCharacterSpeed += m_fCharacterAddSpeed;

// 		m_fCharacterSpeed = GetCharacterSteps(g_pShuttleChild->m_myShuttleInfo.PilotFace) + m_fCharacterMoveRate;
	}

	m_vNextPos += m_vVel * m_fCharacterSpeed;
}

void CCharacterChild::InitEffect()
{
	FLOG( "CCharacterChild::InitEffect()" );
	char strFileName[16];
//	CItemInfo* pItem = m_pStoreData->FindItemInInventoryByWindowPos( POS_CENTER );
//	// 2005-07-20 by ispark
//	// ĳ���� ������ ��ȣ�� �Է��Ѵ�. ���⿡ ����Ʈ ����
////	ChangeUnitCharacterInfo((pItem?pItem->ItemNum:0), g_pShuttleChild->m_myShuttleInfo.PilotFace, GetCurrentBodyCondition(), FALSE);
	int nTemp = ::GetUnitNum(0, 0, g_pShuttleChild->m_myShuttleInfo.PilotFace, TRUE);

//	nTemp = 20000100;
	if(nTemp != m_nUnitNum)
	{
		m_nUnitNum = nTemp;
		wsprintf( strFileName, "%08d", m_nUnitNum );
		LoadCharacterEffect(&m_pCharacterInfo, strFileName);
		if(m_pCharacterInfo)
		{
			m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
			m_pCharacterInfo->SetCharacterAnimationBodyConditionMask(BODYCON_HUMAN_ANIMATION_TIME);
			m_pCharacterInfo->SetBodyCondition(m_hyBodyCondition);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Picking ����Ʈ6
	wsprintf(strFileName, "%08d", PICKING_OBJECT_NUMBER);
	LoadCharacterEffect(&m_pPickingInfo, strFileName);
	if(m_pPickingInfo)
	{
		m_pPickingInfo->SetAllBodyConditionMatrix(m_mPickMatrix);
		m_pPickingInfo->SetBodyCondition(BODYCON_LANDED_MASK);
	}
}

void CCharacterChild::InitCharacterData()
{
	D3DXVECTOR3 vTempPos;
	m_vUp = D3DXVECTOR3(0, 1, 0);
	vTempPos.x = g_pGround->m_projectInfo.sXSize*TILE_SIZE/2.0f+RAND100();
	vTempPos.y = m_vPos.y;
	vTempPos.z = g_pGround->m_projectInfo.sYSize*TILE_SIZE/2.0f+RAND100();
	D3DXVec3Normalize(&m_vVel,&(vTempPos-m_vPos));
	// 2007-07-04 by dgwoo ������ī �ʿ����� �ٶ󺸴� ������ �ݴ� �������� ����.
	// 2009. 10. 14 by jskim ������ī ���� 
// 	if(IS_NORMAL_CITY_MAP_INDEX(g_pShuttleChild->GetMyShuttleInfo().MapChannelIndex.MapIndex))
// 	{
// 		m_vVel = -m_vVel;
// 	}
	//end 2009. 10. 14 by jskim ������ī ���� 
	DBGOUT("CCharacterChild,m_vPos(%.2f, %.2f, %.2f)\n", m_vPos.x, m_vPos.y, m_vPos.z);
	DBGOUT("CCharacterChild,m_vVel(%.2f, %.2f, %.2f)\n", m_vVel.x, m_vVel.y, m_vVel.z);
	D3DXVec3Normalize(&m_vVel,&m_vVel);
    D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);        

	m_dwState = _NCITYIN;
	g_pShuttleChild->ChangeUnitState( _LANDED );
	m_bPickMove = FALSE;										// ó������ Picking ���� �ƴ�
	m_bCharacterRender = TRUE;
	m_bBazaarEventPos = FALSE;
		
	InitEffect();
	g_pShuttleChild->InitItemPoint();									// 2006-07-21 by ispark
	g_pD3dApp->m_pSound->StopD3DSound( SOUND_GROUND_MOVING_A_GEAR );	// 2005-08-19 by ispark
	//////////////////////////////////////////////////////////////////////////
	// ĳ���� ���� ����
//	D3DXVECTOR3 vPos = m_vPos;
//	vPos.y += CHARACTER_HEIGHT;

	// �ʱ⿡�� �� �ִ� �ִϸ��̼�
//	g_pShuttleChild->SendFieldSocketChangeBodyCondition(g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_CHARACTER_MODE_STOP);
	m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
	// 2010-06-08 by dgwoo, ��ý��� �߰�. �Ʒ��� �Լ��� ����. 
	ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
	//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
	//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
	//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
	// 2010-06-08 by dgwoo, ��ý��� �߰�.

	// �ʱ�ȭ
//	float fTempHeight = 0.0f;
//	float fObjHeight = -DEFAULT_COLLISION_DISTANCE;
//	
//	D3DXMATRIX matTemp;
//	D3DXVECTOR3 vSide(0, 0, 1), vUp(0, 1, 0);
//	D3DXMatrixLookAtLH(&matTemp,&vPos,&(vPos - vUp),&vSide);
//	fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,vPos).fDist - CHARACTER_HEIGHT;
//	
//	if(fTempHeight<m_vPos.y)
//	m_vPos.y += -fTempHeight;

	m_vNextPos = m_vPos;
	//////////////////////////////////////////////////////////////////////////
	// CShuttleChild �ʱ�ȭ
	// ������ ������ 1��Ī�̿����� 3��Ī���� ��ȯ 
	if(g_pD3dApp->m_pCamera->GetCamType() == CAMERA_TYPE_FPS)
	{
		g_pD3dApp->m_pCamera->SetCamType(CAMERA_TYPE_NORMAL);
		g_pShuttleChild->m_bLandingOldCamtypeFPS = TRUE;
	}
	
	g_pShuttleChild->m_vPos = m_vPos;
	g_pShuttleChild->m_vVel = m_vVel;

	g_pShuttleChild->InitCharacterToShuttleData();				// 2005-11-10 by ispark

	//////////////////////////////////////////////////////////////////////////
	// ��Ÿ
	g_pInterface->m_pGameMain->InitShowWindow();				// 2005-08-05 by ispark

	// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	{
		//g_pInterface->m_bShowInterface = TRUE;						// 2005-08-30 by ispark	
		//BOOL bShowInter = !g_pD3dApp->IsOptionEtc(OPTION_RADIO_INTERFACE_HIDE);
		//g_pInterface->SetShowInterface(bShowInter);		
	}

	// �� �˻縦 ���� ��ǥ �ʱ�ȭ
	g_pD3dApp->m_vShuttleOldPos = m_vPos;


//	// 2006-02-09 by ispark, ����, ���� ������ �ӵ� �ٸ�
//	if(g_pShuttleChild->m_myShuttleInfo.PilotFace < 100)
//	{
//		m_fCharacterSpeed = CHARACTER_WOMAN_RUN;		
//	}
//	else
//	{
//		m_fCharacterSpeed = CHARACTER_MAN_RUN;		
//	}
	// 2006-11-02 by ispark, ĳ���� ���� ���������� ����
	m_fCharacterSpeed = GetCharacterSteps(g_pShuttleChild->m_myShuttleInfo.PilotFace);

	m_nStartEventType = EVENT_TYPE_NOEVENT;
	m_nSelBazaarType = 0;
	m_ShopEnemyClientIndex = 0;
	m_fCharacterWarpErrTime	= 0.0f;
}

void CCharacterChild::LoadCharacterEffect(CCharacterInfo** pCharacterInfo, char *strFileName)
{
	if((*pCharacterInfo))
	{
		(*pCharacterInfo)->InvalidateDeviceObjects();
		(*pCharacterInfo)->DeleteDeviceObjects();
		util::del(*pCharacterInfo);
	}
	(*pCharacterInfo) = new CCharacterInfo();
	if((*pCharacterInfo)->Load(strFileName))
	{
		(*pCharacterInfo)->InitDeviceObjects();
		(*pCharacterInfo)->RestoreDeviceObjects();
	}
	else
	{
		util::del(*pCharacterInfo);
		DBGOUT("WARNING!! [EFFECT ERROR] Can't Find Effect File : %s \n",strFileName);
	}
}

HRESULT CCharacterChild::RestoreShadow()
{
	DWORD	dwColor = 0xFFFFFFFF;
	if( FAILED( g_pD3dDev->CreateVertexBuffer( 4*sizeof( SPRITEVERTEX ),
		0, D3DFVF_SPRITEVERTEX, D3DPOOL_MANAGED, &m_pVBShadow,NULL ) ) )
	{
		g_pD3dApp->CheckError(CLIENT_ERR_SHUTTLE_EFFECTRESTORE);
		return E_FAIL;
	}
	SPRITEVERTEX* v;
	m_pVBShadow->Lock( 0, 0, (void**)&v, 0 );
	v[0].p = D3DXVECTOR3(0,0,0);  v[0].color=dwColor;	v[0].tu=0; v[0].tv=1;	
	v[1].p = D3DXVECTOR3(0,0,0);  v[1].color=dwColor;	v[1].tu=0; v[1].tv=0;	
	v[2].p = D3DXVECTOR3(0,0,0);  v[2].color=dwColor;	v[2].tu=1; v[2].tv=1;	
	v[3].p = D3DXVECTOR3(0,0,0);  v[3].color=dwColor;	v[3].tu=1; v[3].tv=0;	
	m_pVBShadow->Unlock();

	return S_OK;
}

void CCharacterChild::FineObjectTakeOff()
{
	if(m_bEnterShopState == TRUE)
		return;
	CObjectChild * pCurrentObjEvent = g_pScene->FindEventObjectByTypeAndPosition(EVENT_TYPE_TAKEOFF_PATTERN_START, m_vPos, 10240.0f);
	
	if(pCurrentObjEvent)
	{
		CObjectChild* pObj = g_pScene->FindObjectByIndex(pCurrentObjEvent->m_sEventIndexFrom);
		
		if(pObj)
		{
		    m_vPos = pObj->m_vPos;
			// ��� ��� ��ȯ
			g_pD3dApp->m_bCharacter = FALSE;
			g_pInterface->m_pGameMain->InitWarp();
			g_pShuttleChild->m_vPos = m_vPos;
			g_pShuttleChild->InitShuttleData(TRUE);
			g_pShuttleChild->m_vPos.y = m_vPos.y;
			g_pShuttleChild->m_bWarpLink = TRUE;
		}
	}
	
}

void CCharacterChild::CheckState()
{
	if(g_pShuttleChild->m_dwState == _WARP)
	{
		EVENT_POINT UnitPoint, CameraPoint;
		BOOL bResult = FALSE;
//		if(g_pShuttleChild->m_pCinemaUnitPattern != NULL)
//		{
//			bResult = g_pShuttleChild->m_pCinemaUnitPattern->SkillTick();
//			UnitPoint = g_pShuttleChild->m_pCinemaUnitPattern->GetCurrentCinemaPoint();
//			m_vPos = UnitPoint.vPosition;				// ��ġ
//			m_vVel = UnitPoint.vDirection;				// ����
//			m_vUp = UnitPoint.vUpVector;
//			m_vTargetPosition = UnitPoint.vTarget;			
//		}

		if(g_pShuttleChild->m_bIsCameraPattern == TRUE)
		{
			bResult = g_pShuttleChild->m_pCinemaCamera->SkillTick();
			D3DXVECTOR3 vCameraPos, vCameraVel, vCameraUp;
			CameraPoint = g_pShuttleChild->m_pCinemaCamera->GetCurrentCinemaPoint();
			vCameraPos = CameraPoint.vPosition;
			vCameraVel = m_vPos - CameraPoint.vPosition;
			vCameraUp = CameraPoint.vUpVector;
			g_pCamera->Tick(vCameraPos,vCameraVel,vCameraUp);
			
		}
//		m_fEventCheckTime = 2.0f;
		if(bResult)	// ī�޶� ������ ������ �ƴϸ� ���� ������ ������ �����Ѵ�.
		{
			switch(g_pShuttleChild->m_nEventType)
			{
			case EVENT_GAME_END:
				{
					g_pShuttleChild->SetSelectCharacterItem();
					g_pD3dApp->SendFieldSocketCharacterGameEnd(FALSE);	// 2015-09-26 Future, standardized Game closing
				}
				break;
			}
//			m_bEventReady = FALSE; WARP_OK���� �Ѵ�.
			
		}
	}
}

void CCharacterChild::SetMatrix_Picking(D3DXVECTOR3 vPos, D3DXVECTOR3 vNorPos)
{
	// ������Ʈ ��ǥ�� Z�������� 90�� ȸ�� �Ǿ� �ִ� �����̴�.
	// �̺κ��� ������ ������ �ִ�.
	// ���⼭ vNorPos�� 90�� ȸ�� ��Ų ���� vDir(����) ���� �ȴ�.
//	D3DXMATRIX matTemp;
//	D3DXVECTOR3 vDir;
//	D3DXMatrixRotationZ(&matTemp, D3DXToRadian(90));
//	D3DXVec3TransformCoord(&vDir, &vNorPos, &matTemp);

	// LookAtRH���� Up���� x�� �������� �־���.
	vPos.y += 0.3f;
	D3DXMatrixLookAtRH(&m_mPickMatrix, &vPos, &(vPos + vNorPos), &D3DXVECTOR3(1, 0, 0));
	D3DXMatrixInverse(&m_mPickMatrix, nullptr, &m_mPickMatrix );
}

BOOL CCharacterChild::CheckEnterShopNPC(POINT *pt, float fLength, BOOL nFlag)
{
	int nShopIndex = 0;
	BOOL bCheckShopObj = FALSE;

	// 2005-12-26 by ispark
	// �ŷ����̸� ������ ���� ���ϰ� �Ѵ�.
	if(g_pGameMain->m_nLeftWindowInfo == LEFT_WINDOW_TRANS)
		return FALSE;
	
	// 2005-11-11 by ispark
	// ������ TRUE�̸� ���� ���
	if(g_pD3dApp->m_dwGameState == _SHOP)
		return TRUE;

	D3DXVECTOR2 vPos1 = D3DXVECTOR2(pt->x,pt->y);
	COLLISION_OBJ_RESULT stObject;
	CObjectChild * NPCtempObj = nullptr;
	m_pSelectObject = nullptr;

	stObject = g_pScene->m_pObjectRender->GetPickObject(pt->x, pt->y);
	
	if(stObject.pObject)
	{
		float fDistanceScene = D3DXVec3Length(&(m_vPos - stObject.stCollision.vPicking));
//		DBGOUT("ObjNum %d, Pick EventType %d(dis = %f)\n", stObject.pObject->m_nCode, stObject.pObject->m_bEventType, fDistanceScene);
		if(stObject.pObject->m_bEventType == EVENT_TYPE_OBJ_BUILDING_NPC && fDistanceScene<100.0f)
		{
//			m_nSelectObjectIndex = tempObj->m_pObjectInfo->RenderIndex; // ����	
			m_pSelectObject = stObject.pObject; // ����	
			
//			float fDistance = D3DXVec3Length(&(m_vPos - tempObj->m_vPos)) - tempObj->m_pObjMesh->m_fRadius;
			float fDistance = D3DXVec3Length(&(m_vPos - stObject.stCollision.vPicking));
			if( fDistance<fLength && nFlag)
			{
				nShopIndex = stObject.pObject->m_sEventIndexTo;
#ifdef CUSTOM_OPTIMIZER_HSSON
				g_cCustomOptimizer.m_nMyInfiShopIndex = stObject.pObject->m_sEventIndexTo;
#endif
			}

			bCheckShopObj = TRUE;
		}
	}

	// 2006-09-14 by ispark, �ι�° �˻�
	NPCtempObj = g_pScene->m_pObjectRender->GetPickObjectShopNPC(pt->x, pt->y);
	if(NPCtempObj && bCheckShopObj == FALSE)
	{
		
		float fDistanceScene = D3DXVec3Length(&(m_vPos - NPCtempObj->m_vPos));
		if(NPCtempObj->m_bEventType == EVENT_TYPE_OBJ_BUILDING_NPC && fDistanceScene<10.0f)	// 2012-10-10 by mspark, �ٸ� ���� �ǹ� Ŭ���Ǵ� ���� ���� - ���� 100.0f���� 10.0f�� ����
		{
			m_pSelectObject = NPCtempObj; // ����	
			
			float fDistance = D3DXVec3Length(&(m_vPos - stObject.stCollision.vPicking));
			if( fDistance<fLength && nFlag)
			{
				nShopIndex = NPCtempObj->m_sEventIndexTo;
			}
		}
	}

	// 2008-03-27 by bhsohn �̼Ǽ���â�� ���ִ� ���¿����� ������ �ȿ����� ����
	if(nShopIndex && !g_pD3dApp->IsPossibleShopOpen())
	{
		return TRUE;
	}
	// end 2008-03-27 by bhsohn �̼Ǽ���â�� ���ִ� ���¿����� ������ �ȿ����� ����


	// 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)
	if( nShopIndex )
	{
		BUILDINGNPC* pBuilding = g_pDatabase->GetServerBuildingNPCInfo( nShopIndex );
		if( pBuilding )
		{
			// ���Ǵ�Ƽ �˾�â�� ������ ��� ���Ǵ�Ƽ ����Ŭ���� �ߺ����� �ʵ��� �Ѵ�
			if( pBuilding->BuildingKind == BUILDINGKIND_INFINITY )
			{
				if( g_pD3dApp->m_pInterface->m_pInfinityPopup 
					&& g_pD3dApp->m_pInterface->m_pInfinityPopup->IsShowWnd() )
				{
					return TRUE;
				}

				if( g_pD3dApp->m_pInterface->IsArenaGameState( ARENA_GAME_STATE_TEAMINFO ) )
				{
					return TRUE;
				}
			}

			if( pBuilding->BuildingKind == BUILDINGKIND_ARENA )
			{
				if( g_pD3dApp->m_pInterface->m_pInfinityPopup 
					&& g_pD3dApp->m_pInterface->m_pInfinityPopup->IsShowWnd() )
				{
					return TRUE;
				}
			}
		}
	}
	// end 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)

	// 2013-03-21 by bhsohn â�� ������ ���λ��� ĳ���� ���� �� ���λ����� ������� �ʰ� â�� ���µǴ� ����.
#ifdef FREEWAR_
	if (!IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_NORMAL, 0, g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
#else
	if(!IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType,INFLUENCE_TYPE_NORMAL,0))
#endif
	{			
		//���� �ڽ��� ������ �Ϲݱ��̸� ���λ����� ��Ȱ��ȭ �Ѵ�.
		// ĳ���� üũ
		CEnemyData* pEnemy = g_pScene->GetEnemyCharaterID(vPos1);
		m_pSelBazaarShop = nullptr;
		if(pEnemy)
		{
			float fDistanceScene = D3DXVec3Length(&(m_vPos - pEnemy->m_vPos));
			
			if(fDistanceScene <= REQUERED_BAZAAR_DISTANCE)
			{
				return g_pInterface->SendBazaarRequestItemList(pEnemy, nFlag);
			}
		}
	}
	// END 2013-03-21 by bhsohn â�� ������ ���λ��� ĳ���� ���� �� ���λ����� ������� �ʰ� â�� ���µǴ� ����.


	// 2006-09-14 by ispark, ���� �޼���
//*--------------------------------------------------------------------------*//
	if(nShopIndex)
	{
		// 2011-07-22 by hsson �?�ߺ� �������� ��æƮ �Ǵ� ���� ����
		// �������� ����ؼ� ������ �������� �����ð� ������ �� ���� �Ѵ�.
		if( g_pD3dApp->m_pInterface->m_pGameMain->m_pInven->m_bShopConcurrent + CINFInvenExtend::SHOP_CONCURRENT_DELAY < timeGetTime() )
		{
			g_pD3dApp->m_pInterface->m_pGameMain->m_pInven->m_bShopConcurrent = timeGetTime();

			m_pShopObject = stObject.pObject;
			
			MSG_FC_EVENT_CHARACTERMODE_ENTER_BUILDING sMsg;
			sMsg.nBuildingIndex0 = nShopIndex;
			//		sMsg.SendShopItemList = FALSE;
			g_pFieldWinSocket->SendMsg( T_FC_EVENT_CHARACTERMODE_ENTER_BUILDING, (char*)&sMsg, sizeof(sMsg) );				
			g_pD3dApp->m_bRequestEnable = FALSE;			// 20 06-06-21 by ispark, �޼��� ������ ��ٸ���.
			return TRUE;
		}
		// end 2011-07-22 by hsson �?�ߺ� �������� ��æƮ �Ǵ� ���� ����
	}
	
//*--------------------------------------------------------------------------*//

	// 2013-03-21 by bhsohn â�� ������ ���λ��� ĳ���� ���� �� ���λ����� ������� �ʰ� â�� ���µǴ� ����.
	// 2006-08-07 by dgwoo ���� �ڽ��� ������ �Ϲݱ��̸� ���λ����� ��Ȱ��ȭ �Ѵ�.
// 	if(IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType,INFLUENCE_TYPE_NORMAL,0))
// 		return FALSE;
// 
// 	// 2006-07-29 by ispark, ĳ���� üũ
// 	CEnemyData* pEnemy = g_pScene->GetEnemyCharaterID(vPos1);
// 	m_pSelBazaarShop = NULL;
// 	if(pEnemy)
// 	{
// 		float fDistanceScene = D3DXVec3Length(&(m_vPos - pEnemy->m_vPos));
// 
// 		if(fDistanceScene <= REQUERED_BAZAAR_DISTANCE)
// 		{
// 			return g_pInterface->SendBazaarRequestItemList(pEnemy, nFlag);
// 		}
// 	}
	// END 2013-03-21 by bhsohn â�� ������ ���λ��� ĳ���� ���� �� ���λ����� ������� �ʰ� â�� ���µǴ� ����.

	return FALSE;
}

CObjectChild *CCharacterChild::GetEventObject(float fDistance)
{
	CObjectChild * pObjectRes = nullptr;
	vectorCObjectChildPtr::iterator itObj = g_pScene->m_vectorRangeObjectPtrList.begin();
	while(itObj != g_pScene->m_vectorRangeObjectPtrList.end())
	{
		CObjectChild * pObject = *itObj;
		if(pObject->m_bEventType == EVENT_TYPE_OBJ_BUILDING_NPC)
		{
			float fLengthTemp = D3DXVec3Length(&(m_vPos - pObject->m_vPos));
			if(fLengthTemp > fDistance)
			{
				itObj++;
				continue;
			}
			
			if(pObjectRes == nullptr)
			{
				pObjectRes = pObject;
			}

			float fLengthTemp1 = D3DXVec3Length(&(m_vPos - pObject->m_vPos));
			float fLengthTemp2 = D3DXVec3Length(&(m_vPos - pObjectRes->m_vPos));
			
			if(fLengthTemp2>fLengthTemp1)
			{
				pObjectRes = pObject;
			}
			
		}
		itObj++;
	}

	return pObjectRes;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CCharacterChild::CameraMoveTick()
/// \brief		ĳ���� ���콺 ������ Tick()
/// \author		ispark
/// \date		2005-08-16 ~ 2005-08-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCharacterChild::CameraMoveTick()
{
	float fDirY = 0.0f, fDirX = 0.0f;
	POINT pt;

	m_bCharacterRender = TRUE;

	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);

	if(m_bRButtonState && m_bMouseMove)
	{
//		CheckMouseReverse(&pt);
		// �Ϲ�, �ܰ����� ���콺 �������� ��
		if(pt.y <= 0)
			fDirY = 10.0f;
		else if(pt.y >= g_pD3dApp->GetBackBufferDesc().Height-1)
			fDirY = -10.0f;
		else
			fDirY = m_pOldMousePoint.y - pt.y;
		
		if(pt.x <= 0)
			fDirX = -10.0f;
		else if(pt.x >= g_pD3dApp->GetBackBufferDesc().Width-1)
			fDirX = 10.0f;
		else
			fDirX = pt.x - m_pOldMousePoint.x;
		
		m_pOldMousePoint = pt;
	}
	else if(m_bRButtonState)
	{
		// �ܰ�ó�� ���� ������ ��ư������ ���� ó��, ���콺 �������� ����
		if(pt.y <= 0)
			fDirY = 10.0f;
		else if(pt.y >= g_pD3dApp->GetBackBufferDesc().Height-1)
			fDirY = -10.0f;

		if(pt.x <= 0)
			fDirX = -10.0f;
		else if(pt.x >= g_pD3dApp->GetBackBufferDesc().Width-1)
			fDirX = 10.0f;
	}
	
	g_pCamera->SetAngleX(fDirX);
	g_pCamera->SetAngleY(fDirY);

	// �̺�Ʈ ������Ʈ�� ���� ī�޶� ���������� ȸ���� �ϸ� �ȵŴ� ��Ȳ
	BOOL bWarp = (m_nStartEventType != EVENT_TYPE_NOEVENT) ? TRUE : FALSE;

	if(m_bPickMove == TRUE || m_bMouseWheel == TRUE || bWarp)
		g_pCamera->SetCamMove(m_bRButtonState, m_bMouseWheel, bWarp);

	// ī�޶� �Ÿ��� �ּ����ġ���� ������ ĳ���� ������ ����
	float fCollDist = g_pCamera->GetCollDistance();
	if(fCollDist != 0 && 
		fCollDist < CHARACTER_CAMERA_DISTANCE_MIN)
		m_bCharacterRender = FALSE;

	m_bMouseMove = FALSE;
	m_bMouseWheel = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCharacterChild::CheckEventTypeObject(float fElapsedTime)
/// \brief		ĳ���� ���� �̺�Ʈ ������Ʈ üũ
/// \author		ispark
/// \date		2006-07-14 ~ 2006-07-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCharacterChild::CheckEventTypeObject(float fElapsedTime)
{
	if(g_pScene->m_pObjectRender)
	{
		CObjectChild* pCurrentObjEvent = (CObjectChild *)g_pScene->m_pObjectRender->CheckCollMeshEvent(40.0f);
		
		if(!pCurrentObjEvent)
		{
			return;
		}

		// �����̶��
		if(m_dwState == _NCITYIN)
		{
			m_bStartPositionEvent = TRUE;
			return;
		}

		// Ÿ�� üũ
		// ���� Ÿ��(10��)�� �ʰ� �ߴ���
		m_fCharacterWarpErrTime -= fElapsedTime;
		if(m_fCharacterWarpErrTime > 0.0f)
		{
			return;
		}
		else
		{
			m_fCharacterWarpErrTime = 0.0f;
		}

		float fEventLength = D3DXVec3Length(&D3DXVECTOR3(pCurrentObjEvent->m_vPos - m_vPos));
		
		switch(pCurrentObjEvent->m_bEventType)
		{
		case EVENT_TYPE_CHARACTERMODE_WARP:
			{
				if(m_nStartEventType == EVENT_TYPE_CHARACTERMODE_WARP_TARGET)
				{
					// ���� ����
					// �̺�Ʈ�� Ư�� �Ÿ� �ȿ� �����
					if(fEventLength > 15.0f)
					{
						break;
					}

//					m_dwState = _STAND;
					m_bPickMove = FALSE;

					m_vMoveVel = D3DXVECTOR3(0, 0, 0);
					m_fCharacterWarpErrTime = 10.0f;
					SendRequestEventObjectWarpIn(pCurrentObjEvent);
					m_nStartEventType = EVENT_TYPE_NOEVENT;
				}
				else if(m_nStartEventType == EVENT_TYPE_NOEVENT)
				{
					// Ÿ�� �̺�Ʈ ������Ʈ ������ ã�´�.
					SetMoveVelByEventObject(EVENT_TYPE_CHARACTERMODE_WARP_TARGET, 500.0f);
					m_nStartEventType = EVENT_TYPE_CHARACTERMODE_WARP;
					m_dwState = _WARP;

					m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, ��ý��� �߰�. �Ʒ��� �Լ��� ����. 
					ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, ��ý��� �߰�.
				}
			}
			break;
		case EVENT_TYPE_CHARACTERMODE_WARP_TARGET:
			{
				if(m_nStartEventType == EVENT_TYPE_CHARACTERMODE_WARP)
				{
					// ���� ��
					// �̺�Ʈ ������Ʈ ���̺��� ���ƾ� �Ѵ�.
					if(pCurrentObjEvent->m_vPos.y > m_vPos.y)
					{
						break;
					}

					m_dwState = _STAND;
					m_nStartEventType = -1;								// �̺�Ʈ Ÿ�� ����
				}
				else if(m_nStartEventType == EVENT_TYPE_NOEVENT)
				{
					// �̺�Ʈ�� Ư�� �Ÿ� �ȿ� �����
					if(fEventLength > 10.0f)
					{
						break;
					}

					// ���� �̺�Ʈ ������Ʈ ������ ã�´�.
					SetMoveVelByEventObject(EVENT_TYPE_CHARACTERMODE_WARP, 500.0f);
					m_nStartEventType = EVENT_TYPE_CHARACTERMODE_WARP_TARGET;
					m_dwState = _WARP;
					m_bPickMove = FALSE;

					m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, ��ý��� �߰�. �Ʒ��� �Լ��� ����. 
					ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
					//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
					//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
					//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, ��ý��� �߰�.
				}
				else if(m_nStartEventType == -1)
				{
					// ������ �� �� ���� ������ �Ѿ�� �������� �̺�Ʈ ������Ʈ�� �����Ѵ�.
					float fEventLength = D3DXVec3Length(&D3DXVECTOR3(pCurrentObjEvent->m_vPos - m_vPos));
					if(fEventLength > 30.0f)
					{
						m_nStartEventType = EVENT_TYPE_NOEVENT;
					}
				}
			}
			break;
			// 2007-12-14 by dgwoo ĳ���� ��忡�� �ٷ� ���� Ż���ֵ��� �̺�Ʈ �߰�.
		case EVENT_TYPE_CHARACTERMODE_DIRECTLY_WARP:
			{
				m_bPickMove = FALSE;
				m_vMoveVel = D3DXVECTOR3(0, 0, 0);
				m_fCharacterWarpErrTime = 10.0f;
				SendRequestEventObjectWarpIn(pCurrentObjEvent);
				m_nStartEventType = EVENT_TYPE_NOEVENT;
			}
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCharacterChild::SendRequestEventObjectWarpIn(CObjectChild *pObj)
/// \brief		ĳ���� ���� ��
/// \author		ispark
/// \date		2006-07-19 ~ 2006-07-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCharacterChild::SendRequestEventObjectWarpIn(CObjectChild *pObj)
{
	MSG_FC_EVENT_REQUEST_OBJECT_EVENT sMsg;
	sMsg.ObjectType = pObj->m_nCode;
	sMsg.ObjectPosition = pObj->m_vPos;
	g_pFieldWinSocket->SendMsg( T_FC_EVENT_REQUEST_OBJECT_EVENT, (char*)&sMsg, sizeof(sMsg) );
	DBGOUT("Character Request Warp : MSG_FC_EVENT_REQUEST_OBJECT_EVENT(%d)\n",pObj->m_nCode);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			D3DXVECTOR3 CCharacterChild::SetMoveVelByEventObject(BYTE bObjectType, float fDist)
/// \brief		Ư�� �̺�Ʈ ������Ʈ ������ ���Ѵ�.
/// \author		ispark
/// \date		2006-07-20 ~ 2006-07-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3 CCharacterChild::SetMoveVelByEventObject(BYTE bObjectType, float fDist)
{
	// �̺�Ʈ ������Ʈ�� ã�´�.
	CObjectChild * pNextTargetObjEvent = g_pScene->FindEventObjectByTypeAndPosition(bObjectType, m_vPos, fDist);
	// �̵� ������ �����Ѵ�.
	m_vMoveVel = pNextTargetObjEvent->m_vPos - m_vPos;
	m_vMoveVel.x = m_vMoveVel.z = 0.0f;
	D3DXVec3Normalize(&m_vMoveVel, &m_vMoveVel);

	m_vMoveVel.y *= CHARACTER_MAN_RUN;
	return m_vMoveVel;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCharacterChild::DeleteChatMoveShop()
/// \brief		
/// \author		ispark
/// \date		2006-09-04 ~ 2006-09-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCharacterChild::DeleteChatMoveShop()
{
	if(m_pMoveChatShop)
	{
		m_pMoveChatShop->InvalidateDeviceObjects();
		m_pMoveChatShop->DeleteDeviceObjects();
		util::del(m_pMoveChatShop);
	}
}
// 2010-06-08 by dgwoo, ��ý��� �߰�. �Ʒ��� �Լ��� ����. 
void CCharacterChild::ChangeWearItemBodyConditionAllProcess(BodyCond_t i_nBodyCondition)
{
	g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORY_UNLIMITED, i_nBodyCondition);
	g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT, i_nBodyCondition);
	g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, i_nBodyCondition);
	g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_PET, i_nBodyCondition);
}
// 2010-06-08 by dgwoo, ��ý��� �߰�. �Ʒ��� �Լ��� ����. 