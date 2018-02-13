// MonsterData.cpp: implementation of the CMonsterData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MonsterData.h"
#include "ChatMoveData.h"
//#include "MonsterRender.h"
#include "AtumApplication.h"
#include "SceneData.h"
#include "AtumDatabase.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "Chat.h"
#include "Background.h"
#include "dxutil.h"
#include "Camera.h"
#include "Weapon.h"
#include "Cinema.h"
// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
#include "IMSocketManager.h"
//#include "ImSocket.h"
#include "INFGameMain.h" // 2011-05-17 by jhahn	����3�� ���� ��ü

#include "FieldWinSocket.h"
#include "Frustum.h"
#include "MonRender.h"
#include "SkillEffect.h"
#include "ObjectChild.h"
#include "StoreData.h"

//#define COLOR_MONSTER_8		D3DCOLOR_ARGB(0, 255, 66, 41)
//#define COLOR_MONSTER_7		D3DCOLOR_ARGB(0, 255, 174, 163)
//#define COLOR_MONSTER_6		D3DCOLOR_ARGB(0, 0, 208, 0)
//#define COLOR_MONSTER_5		D3DCOLOR_ARGB(0, 108, 255, 105)
//#define COLOR_MONSTER_4		D3DCOLOR_ARGB(0, 255, 255, 255)
//#define COLOR_MONSTER_3		D3DCOLOR_ARGB(0, 144, 255, 255)
//#define COLOR_MONSTER_2		D3DCOLOR_ARGB(0, 0, 216, 255)
//#define COLOR_MONSTER_1		D3DCOLOR_ARGB(0, 15, 117, 255)
//#define COLOR_MONSTER_0		D3DCOLOR_ARGB(0, 0, 0, 255)
#define COLOR_MONSTER_8		D3DCOLOR_ARGB(0, 200, 0, 0)
#define COLOR_MONSTER_7		D3DCOLOR_ARGB(0, 255, 64, 64)
#define COLOR_MONSTER_6		D3DCOLOR_ARGB(0, 255, 131, 131)
#define COLOR_MONSTER_5		D3DCOLOR_ARGB(0, 255, 200, 200)
#define COLOR_MONSTER_4		D3DCOLOR_ARGB(0, 255, 255, 255)
#define COLOR_MONSTER_3		D3DCOLOR_ARGB(0, 200, 200, 255)
#define COLOR_MONSTER_2		D3DCOLOR_ARGB(0, 131, 131, 255)
#define COLOR_MONSTER_1		D3DCOLOR_ARGB(0, 64, 64, 255)
#define COLOR_MONSTER_0		D3DCOLOR_ARGB(0, 0, 0, 200)

#define MIN_GAP_TO_GROUND	30.0f
#define MONSTER_LIFE_TIME	5.0f
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonsterData::CMonsterData(MSG_FC_CHARACTER_GET_MONSTER_INFO_OK * pMsg)
{
	FLOG( "CMonsterData(MSG_FC_CHARACTER_GET_MONSTER_INFO_OK * pMsg)" );
	m_pCharacterInfo = NULL;
	memcpy(&m_info,pMsg,sizeof(MSG_FC_CHARACTER_GET_MONSTER_INFO_OK));
//	memset(&CharacterName,0x00,sizeof(CharacterName));
	m_pRenderer = g_pScene->m_pMonsterRender;
	m_pTarget = NULL;
	m_fTargetCheckTime = 1.0f;

	m_fCurrentTime = 0.0f;
	//m_dwState = _NORMAL;
	m_dwPartType = _MONSTER;
	
	// 2004-11-24 by ydkim ���� ������ ����
	//m_vPos = D3DXVECTOR3(0,30,0);
	//m_vOldPos = D3DXVECTOR3(0,30,0);
	//m_vVel = D3DXVECTOR3(1,0,0);
	m_vPos		= A2DX(m_info.PositionVector);
	m_vOldPos	= m_vPos;
	D3DXVec3Normalize(&m_vVel, &A2DX(m_info.TargetVector));
	m_vUp = D3DXVECTOR3(0,1,0);

	// 2004.06.26 jschoi
	m_vOldSkillEndPos = D3DXVECTOR3(0,30,0);
	m_vOldSkillEndVel = D3DXVECTOR3(1,0,0);
	m_vOldSkillEndUp = D3DXVECTOR3(0,1,0);

	m_vAttackVel = D3DXVECTOR3(0,0,0);

//	m_vMovepos = D3DXVECTOR3(0,0,0);
//	m_vMoveVel = D3DXVECTOR3(1,0,0);
//	m_vMoveup = D3DXVECTOR3(0,1,0);
//	m_vNextPos = D3DXVECTOR3(0,30,0);
//	m_vNextVel = D3DXVECTOR3(1,0,0);
	m_vMovepos = D3DXVECTOR3(0,0,0);
	m_vNextPos = m_vPos;
	m_vMoveVel = D3DXVECTOR3(0,0,0);
	m_vNextVel = m_vVel;
	m_vMoveup = D3DXVECTOR3(0,0,0);
//	m_nMonsterTypePrimary = MONSTERDATA_MONSTERLIST;
//	m_nMonsterTypeSecondary = MONSTERDATA_MONSTERLIST;
	m_bRenderState = TRUE;
	m_pMoveChat = NULL;
	m_pIDChat = NULL;
	m_fLifeTime = MONSTER_LIFE_TIME;
	m_fLeanRate = 0.0f;
	m_bIsUnderWater = FALSE;
	m_bCheckBlend = FALSE;
	m_bCheckColl = FALSE;
//	m_bIsRender = FALSE;
	m_fAniCollTime = 1.0f;
	m_fMaxHP = 0.0f;
	m_fFallingTime = -1.0f;
	m_bRenderHP = FALSE;
	m_fObjectSize = 0.0f;

	m_bAlphaBlending = FALSE;
	m_fMaterialCheckTime = 2.0f;
	D3DUtil_InitMaterial( m_material, 1.0f, 1.0f, 1.0f );// �ӽ� �ڵ�
	m_pMonsterInfo = NULL;
	m_ptCurRegion.x = (int)(m_vPos.x/(TILE_SIZE*3));
	m_ptCurRegion.y = (int)(m_vPos.z/(TILE_SIZE*3));
	m_ptOldRegion = m_ptCurRegion;
	m_pMonMesh = NULL;
	Init();
	m_pEffectTarget = NULL;
	m_nUpDir = 1;
	m_nSideDir = 1;

	// ���� ��ų ���� 2004.06.25 jschoi
	m_nMonsterSkillState = MONSTER_SKILL_OFF;
	m_bSkillEnd = FALSE;
	m_pMonsterMovePattern = NULL;
//	RestoreShadow();
	SetPkState(PK_NORMAL,TRUE);
	// 2004-11-30 by jschoi
	DisableObjectMonsterObject();	// �����Ǵ� ���Ͱ� ������Ʈ ���Ͷ�� ��ü�Ǵ� �ʿ�����Ʈ�� ��Ȱ��ȭ ��Ų��.
	m_nTargetIndex = 0;

	// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
	m_nMultiIndex = 0;
	m_nSiegeIndex = 0;
	m_bSiegeMode = FALSE;
	// end 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
}

CMonsterData::~CMonsterData()
{
	FLOG( "~CMonsterData()" );
	
	// 2004-11-30 by jschoi
	EnableObjectMonsterObject();	// �Ҹ�Ǵ� ���Ͱ� ������Ʈ ���Ͷ�� ��ü�Ǵ� �ʿ�����Ʈ�� Ȱ��ȭ ��Ų��.

	m_pMonsterInfo = NULL;
	if(m_pMoveChat)
	{
		m_pMoveChat->InvalidateDeviceObjects();
		m_pMoveChat->DeleteDeviceObjects();
	}
	util::del(m_pMoveChat);
	if(m_pIDChat)
	{
		m_pIDChat->InvalidateDeviceObjects();
		m_pIDChat->DeleteDeviceObjects();
	}
	util::del(m_pIDChat);
	if(m_pCharacterInfo)//�߰�
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		util::del(m_pCharacterInfo);
	}
	if(m_pEffectTarget)
	{
		m_pEffectTarget->m_bUsing = FALSE;
		m_pEffectTarget = NULL;
	}

	util::del(m_pMonsterMovePattern);
//	InvalidateShadow();
}

HRESULT CMonsterData::InitDeviceObjects()
{
	return S_OK;
}

HRESULT CMonsterData::RestoreDeviceObjects()
{
	RestoreChat();
	return CUnitData::RestoreDeviceObjects();
}

HRESULT CMonsterData::InvalidateDeviceObjects()
{
	InvalidateChat();
	return CUnitData::InvalidateDeviceObjects();
}

HRESULT CMonsterData::DeleteDeviceObjects()
{
	return S_OK;
}


void CMonsterData::RestoreChat()
{
	FLOG( "CMonsterData::RestoreChat()" );
	if(m_pIDChat)
		m_pIDChat->RestoreDeviceObjects();
	if(m_pMoveChat)
		m_pMoveChat->RestoreDeviceObjects();
}

void CMonsterData::InvalidateChat()
{
	FLOG( "CMonsterData::InvalidateChat()" );
	if(m_pIDChat)
		m_pIDChat->InvalidateDeviceObjects();
	if(m_pMoveChat)
		m_pMoveChat->InvalidateDeviceObjects();
}

void CMonsterData::Init()
{
	FLOG( "CMonsterData::Init()" );
	ASSERT_ASSERT(g_pDatabase);
	m_pMonsterInfo = g_pDatabase->CheckMonsterInfo(m_info.MonsterUnitKind);
	if(m_pMonsterInfo)
		D3DXMatrixScaling(&m_mScale,m_pMonsterInfo->ScaleValue,m_pMonsterInfo->ScaleValue,m_pMonsterInfo->ScaleValue);
	else
		D3DXMatrixScaling(&m_mScale,1.0f,1.0f,1.0f);
	// �ʱ�ȭ�ÿ� ���� ��Ʈ������ ���Ѵ�.
	D3DXMatrixLookAtRH( &m_mMatrix, &m_vPos, &(m_vPos+10.0f*m_vVel), &m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	m_mMatrix = m_mScale*m_mMatrix;

	// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
	m_vecMonMultiInfo = g_pDatabase->GetvecMultiTargetInfo(m_pMonsterInfo->MonsterUnitKind);
	m_nindexSize = m_vecMonMultiInfo.size();

	// 2011. 09. 28 by jskim �� Ÿ�� �ý��� ���� ����
	//if(m_nindexSize > NULL)
	if(m_nindexSize > 0)
	// end 2011. 09. 28 by jskim �� Ÿ�� �ý��� ���� ����
	{
		for(int i=0; i < m_nindexSize; i++)
		{
			MULTI_TARGET_DATA* pMultiTargetData = new MULTI_TARGET_DATA;
			
			MONSTER_MULTI_TARGET MonMultiData = g_pDatabase->GetMultiTargetPosion( m_vecMonMultiInfo[i].MonsterIndex, m_vecMonMultiInfo[i].PointIndex );

			D3DXVECTOR3 tempvec = MonMultiData.TargetPosition;
			D3DXMATRIX tempmat;
			D3DXMatrixIdentity(&tempmat);
			pMultiTargetData->PointIndex		= MonMultiData.PointIndex;

			tempmat._41 = tempvec.x;
			tempmat._42 = tempvec.y;
			tempmat._43 = tempvec.z;
			tempmat = tempmat * m_mMatrix;
			pMultiTargetData->TargetMatrix	= tempmat;

			m_vecvmultiData.push_back( pMultiTargetData );
		}

	}
	// end 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���


	// effect
	if(m_pMoveChat)
	{
		m_pMoveChat->InvalidateDeviceObjects();
		m_pMoveChat->DeleteDeviceObjects();
		util::del(m_pMoveChat);
	}
	if(m_pIDChat)
	{
		m_pIDChat->InvalidateDeviceObjects();
		m_pIDChat->DeleteDeviceObjects();
		util::del(m_pIDChat);
	}
	m_pMoveChat = new CAtumNode;
	m_pIDChat = new CAtumNode;
	char strFileName[20];
	if(m_pMonsterInfo)
	{
		wsprintf(strFileName,"%08d.obj",m_pMonsterInfo->SourceIndex);
		LoadCharacterEffect(strFileName);
		// 2012-04-18 by mspark, ������Ʈ ���ҽ� ����
#ifdef C_OBJECT_RESOURCE_ORGANIZE_MSPARK
		FILE* file = _tfopen( _T("(Obj)MonsterData.txt"), _T("a") );
		fprintf( file, "%08d - [MapIndex : %04d]\n", m_pMonsterInfo->SourceIndex, g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex );
		fclose(file);
#endif
		// end 2012-04-18 by mspark, ������Ʈ ���ҽ� ����
		if(m_pCharacterInfo)
		{
			m_pCharacterInfo->SetCharacterAnimationBodyConditionMask(BODYCON_MONSTER_ANIMATION_TIME);
			m_pCharacterInfo->SetBodyCondition(m_info.BodyCondition);
			m_SecondaryAttack.vSidePos = m_pCharacterInfo->GetEffectPos( BODYCON_MON_FIREATTACK1_MASK, RC_EFF_DUMMY_POSITION );
		}

		////////////////// bodycondition ����(state) /////////////////////////////////////////
		if( m_info.BodyCondition & BODYCON_FLY_MASK )
		{
			ChangeUnitState( _NORMAL );
		}
		else if( m_info.BodyCondition & BODYCON_LANDED_MASK )
		{
			ChangeUnitState( _LANDED );
		}
		else if( m_info.BodyCondition & BODYCON_DEAD_MASK )
		{
			ChangeUnitState( _EXPLODED );
		}
		///////////////////////////////////////////////////////////////////////////////////////
		int nCheckLevel;
//		if(COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race,RACE_OPERATION|RACE_GAMEMASTER))
//		{
//			char buf[SIZE_MAX_MONSTER_NAME];
//			wsprintf(buf,"%s(%d)",m_pMonsterInfo->MonsterName,m_pMonsterInfo->Level);
//			strncpy(m_pMonsterInfo->MonsterName,buf,SIZE_MAX_MONSTER_NAME);
//		}
		///////////////////////////////////////////////////////////////////////////////////////
		m_bAlphaBlending = m_pMonsterInfo->AlphaBlending;


		// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
		//m_fMaxHP = m_pMonsterInfo->HP;
		m_fMaxHP = m_info.MaxHP;
		// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
		


		nCheckLevel = m_pMonsterInfo->Level - g_pShuttleChild->m_myShuttleInfo.Level;
		// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
		//m_pMonMesh = m_pRenderer->InitData(m_pMonsterInfo->SourceIndex);
		m_pMonMesh = m_pRenderer->InitData(m_pMonsterInfo->SourceIndex, _MONSTER_TYPE);
		//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
//		m_fObjectSize = m_pMonsterInfo->Size * m_pMonsterInfo->ScaleValue;
		m_fObjectSize = m_pMonsterInfo->Size;
		
		DWORD dwChatType;
		if(nCheckLevel > 9)
			dwChatType = COLOR_MONSTER_0;
		else if(nCheckLevel > 7)
			dwChatType = COLOR_MONSTER_1;
		else if(nCheckLevel > 5)
			dwChatType = COLOR_MONSTER_2;
		else if(nCheckLevel > 3)
			dwChatType = COLOR_MONSTER_3;
		else if(nCheckLevel > -2)
			dwChatType = COLOR_MONSTER_4;
		else if(nCheckLevel > -4)
			dwChatType = COLOR_MONSTER_5;
		else if(nCheckLevel > -6)
			dwChatType = COLOR_MONSTER_6;
		else if(nCheckLevel > -8)
			dwChatType = COLOR_MONSTER_7;
		else
			dwChatType = COLOR_MONSTER_8;
//		DWORD dwChatType;
//		if(nCheckLevel > 5)
//			dwChatType = COLOR_MONSTER_ID00;
//		else if(nCheckLevel > 1)
//			dwChatType = COLOR_MONSTER_ID01;
//		else if(nCheckLevel > -2)
//			dwChatType = COLOR_MONSTER_ID02;
//		else if(nCheckLevel > -6)
//			dwChatType = COLOR_MONSTER_ID03;
//		else
//			dwChatType = COLOR_MONSTER_ID04;
		if(g_pScene->m_byMapType == MAP_TYPE_TUTORIAL)
		{
			dwChatType = COLOR_CHARACTER_ID00;
		}

		if(nCheckLevel <= 0)
		{

			// 2006-12-08 by ispark, ������������ ����� �������̴�.(�Ķ��� ����)
			if(COMPARE_MPOPTION_BIT(m_pMonsterInfo->MPOption, MPOPTION_BIT_BOSS_MONSTER))
			{
				if(nCheckLevel <= -2)
				{
					dwChatType = COLOR_MONSTER_4;
				}
			}
			// 2006-12-08 by ispark, ���ӵ忡���� 5������ ���� ���Ѵ�.
			else if(COMPARE_MPOPTION_BIT(m_pMonsterInfo->MPOption, MPOPTION_BIT_NAMED_MONSTER))
			{
				if(nCheckLevel > -5)
					dwChatType = COLOR_MONSTER_5;
				else if(nCheckLevel > -10)
					dwChatType = COLOR_MONSTER_6;
				else if(nCheckLevel > -15)
					dwChatType = COLOR_MONSTER_7;
				else
					dwChatType = COLOR_MONSTER_8;
			}
		}

		CChatMoveData * pData = new CChatMoveData(m_pIDChat,m_pMonsterInfo->MonsterName,dwChatType,9,256,32);
		pData->InitDeviceObjects();
//		pData->RestoreDeviceObjects();
		pData = (CChatMoveData *)m_pIDChat->AddChild(pData);
//		RestoreChat();
		RestoreDeviceObjects();
	}
	
	m_vPos = A2DX(m_info.PositionVector);
	m_vNextPos = A2DX(m_info.PositionVector);

}

void CMonsterData::Tick()
{
	FLOG( "CMonsterData::Tick()" );
	if(!m_bUsing)
		return;
	float fElapsedTime = g_pD3dApp->GetElapsedTime();
	
	// SKILL EFFECT
	if(m_pSkillEffect)
	{
		m_pSkillEffect->Tick(fElapsedTime);
	}

	m_ptOldRegion = m_ptCurRegion;

	D3DXMATRIX matTemp;
	m_bIsUnderWater = FALSE;

	if(m_dwState != _EXPLODING && m_dwState != _FALLING)
	{
		if(m_fLifeTime > 0.0f)
			m_fLifeTime -= fElapsedTime;
		if(m_fLifeTime <= 0.0f)
		{
//			DBGOUT("Monster(%d) LifeTime <= 0.0f\n", this->m_info.MonsterIndex);
			ChangeUnitState( _EXPLODED );
			//m_dwState = _EXPLODED;
		}
	}
	// ��Ŷ�� ���� �ð��� ���� �̵� ������ ���δ�(������ ���� ��鸲 ����)
	D3DXVECTOR3 vMovepos;
//	if(m_fLifeTime < 1.5f && m_fLifeTime >= 0.0f)
	if(m_fLifeTime < (MONSTER_LIFE_TIME-0.5f) && m_fLifeTime >= (MONSTER_LIFE_TIME-2.0f))
	{
		vMovepos = m_vMovepos * ((MONSTER_LIFE_TIME-m_fLifeTime) * 0.5f);
	}
//	else if(m_fLifeTime < 0.0f)
	else if(m_fLifeTime < MONSTER_LIFE_TIME-2.0f)
	{
		vMovepos = D3DXVECTOR3(0,0,0);
	}
	else
	{
		vMovepos = m_vMovepos;
	}
	if(MOVE_AVAILABLE_STATE(m_dwState))
	{
		m_bRenderHP = FALSE;
		char buf[8];
		memset(buf,0x00,sizeof(buf));
		if(g_pShuttleChild->m_pTarget == this)
		{
			m_bRenderHP = TRUE;
		}
//		m_nMonsterTypePrimary = MONSTERDATA_MONSTERLIST; // �ӽ� List  Ÿ���� ���־�������...

		// 2004-05-07 by dhkwon
//		ITEM* pItem = NULL;
//		if(m_PrimaryAttackData1.WeaponItemNumber!=0)
//		{
//			pItem = g_pDatabase->GetServerItemInfo(m_PrimaryAttackData1.WeaponItemNumber);
//			ASSERT_ASSERT(pItem);
//			if(pItem)
//			{
//				m_PrimaryAttack.fCheckAutoMaticTimer -= g_pD3dApp->GetElapsedTime();
//				m_PrimaryAttack.fCheckAttackTime -= g_pD3dApp->GetElapsedTime();
//			}
//		}
//		else if(m_PrimaryAttackData2.WeaponItemNumber!=0)
//		{
//			pItem = g_pDatabase->GetServerItemInfo(m_PrimaryAttackData2.WeaponItemNumber);
//		}
//		if(pItem)
//		{
//			CheckPrimaryAttack(FALSE, pItem);
//		}
		if(m_PrimaryAttack.AttackData.ItemNum!=0)
		{
			ITEM* pWeaponITEM = g_pDatabase->GetServerItemInfo(m_PrimaryAttack.AttackData.ItemNum);
			ASSERT_ASSERT(pWeaponITEM);
			if(pWeaponITEM)
			{
				m_PrimaryAttack.fCheckAutoMaticTimer -= g_pD3dApp->GetElapsedTime();
				m_PrimaryAttack.fCheckAttackTime -= g_pD3dApp->GetElapsedTime();
				CheckAttack(m_PrimaryAttack, FALSE, pWeaponITEM);
			}
		}
//		if(m_SecondaryAttack.AttackData.ItemNum!=0)
//		{
//			ITEM* pWeaponITEM = g_pDatabase->GetServerItemInfo(m_SecondaryAttack.AttackData.ItemNum);
//			ASSERT_ASSERT(pWeaponITEM);
//			if(pWeaponITEM)
//			{
//				m_SecondaryAttack.fCheckAutoMaticTimer -= g_pD3dApp->GetElapsedTime();
//				m_SecondaryAttack.fCheckAttackTime -= g_pD3dApp->GetElapsedTime();
//				CheckAttack(m_SecondaryAttack, FALSE, pWeaponITEM);
//			}
//		}
/*		if(m_nMonsterTypeSecondary == MONSTERDATA_MONSTERTARGET)
		{
			wsprintf(buf,"%s_F",g_pShuttleChild->m_strSecondaryWeaponTypeName);
			if(!m_pEffectTarget)
			{
				D3DXVECTOR3 vTemp = D3DXVECTOR3(0,0,0);
				m_pEffectTarget = new CAppEffectData(this,g_pShuttleChild->m_nSecondaryWeaponType,vTemp);
				m_pEffectTarget->m_bodyCondition = BODYCON_LANDED_MASK;
				if(m_pEffectTarget->m_pCharacterInfo)
				{
					m_pEffectTarget->m_pCharacterInfo->ChangeBodyCondition(m_pEffectTarget->m_bodyCondition);
					g_pD3dApp->ChangeEffectTexture(m_pEffectTarget,buf);
				}
				m_pEffectTarget = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(m_pEffectTarget);
				char message[256];
				sprintf(message,"2�� Ÿ�� ������");
				g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_SYSTEM);
			}
		}
		else
		{
			if(m_pEffectTarget)
			{
				m_pEffectTarget->m_bUsing = FALSE;
				m_pEffectTarget = NULL;
				char message[256];
				wsprintf(message,"2�� Ÿ�� ������");
				g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_SYSTEM);
			}
		}
*/
//		if(!m_bRenderState)
//		{
//			m_nMonsterTypePrimary = MONSTERDATA_MONSTERLIST;
//			m_nMonsterTypeSecondary = MONSTERDATA_MONSTERLIST;
//		}


		// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����

// 		if(COMPARE_BODYCON_BIT(m_info.BodyCondition,
// 			BODYCON_MON_FIREATTACK1_MASK|
// 			BODYCON_MON_FIREATTACK2_MASK|
// 			BODYCON_MON_FIREATTACK3_MASK|
// 			BODYCON_MON_FIREATTACK4_MASK|
// 			BODYCON_MON_FIREATTACK5_MASK|
// 			BODYCON_MON_FIREATTACK6_MASK))// && m_dwState != _LANDED)
// 		{
// 			D3DXVec3Normalize(&m_vVel,&m_vNextVel);
// 		}

		if( COMPARE_BODYCON_BIT( m_info.BodyCondition, BODYCON_MON_FIREATTACK_ALL_MASK ) )
		{
			D3DXVec3Normalize(&m_vVel,&m_vNextVel);
		}
		
		// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
		else
		{
			D3DXVECTOR3 vSide;
//			if(m_dwState == _LANDED)
//			{
//				D3DXVECTOR3 vNormal = GetMapNormalVector(m_vPos);
//				D3DXVec3Cross(&vSide,&vNormal,&m_vNextVel);
//				D3DXVec3Cross(&m_vNextVel,&vSide,&vNormal);
//				D3DXVec3Normalize(&m_vNextVel,&m_vNextVel);
//				m_vUp = vNormal;
//			}
//			// ����
//			else
			{
				float fAngle;
				fAngle = ACOS(D3DXVec3Dot(&m_vVel,&m_vNextVel))*fElapsedTime*2.0f;
				if(fAngle)
				{
					D3DXVec3Cross(&vSide,&m_vVel,&m_vNextVel);
					D3DXMatrixRotationAxis(&matTemp,&vSide,fAngle);
					D3DXVec3TransformCoord(&m_vVel,&m_vVel,&matTemp);
				}
			}
		}
//		fLength = D3DXVec3Length(&(m_vVel - m_vNextVel));
//		m_vVel += fLength*10.0f*m_vMoveVel*fElapsedTime;
//		D3DXVec3Normalize(&m_vVel,&m_vVel);
		// ��ǥ
		m_vPos += 1.0f*vMovepos*fElapsedTime;
		D3DXVECTOR3 vUp;
		if(m_dwState == _LANDED)
		{
			vUp = GetMapNormalVector(m_vPos);
			D3DXVec3Cross(&vUp,&m_vVel,&vUp);
			D3DXVec3Cross(&m_vUp,&vUp,&m_vVel);
		}
		else
		{
			vUp = D3DXVECTOR3(0,1,0);
			D3DXVec3Cross(&vUp,&m_vVel,&vUp);
			D3DXVec3Cross(&m_vUp,&vUp,&m_vVel);

			D3DXVECTOR3 vTemp;
			D3DXVec3Normalize(&vTemp,&(m_vPos-m_vOldPos));
			float fLength = D3DXVec3Length(&(m_vVel-vTemp));
			if(fLength > 1.0f)
			{
				vUp = D3DXVECTOR3(0,1,0);
				D3DXMatrixRotationAxis(&matTemp,&vUp,0.1f);
				D3DXVec3TransformCoord(&vUp,&m_vVel,&matTemp);
				if(D3DXVec3Length(&(m_vVel+vTemp)) > D3DXVec3Length(&(vUp+vTemp)))
				{
					m_fLeanRate += fElapsedTime;
					if(m_fLeanRate > 0.4f)
						m_fLeanRate = 0.4f;
				}
				else
				{
					m_fLeanRate -= fElapsedTime;
					if(m_fLeanRate < -0.4f)
						m_fLeanRate = -0.4f;
				}
			}
			else
			{
				if(m_fLeanRate > 0.1f)
					m_fLeanRate -= fElapsedTime;
				else if(m_fLeanRate < -0.1f)
					m_fLeanRate += fElapsedTime;
				else
				{
					if(m_fLeanRate != 0.0f)
						m_fLeanRate *= 0.9f;
					if(m_fLeanRate < 0.01f)
						m_fLeanRate = 0.0f;
				}
			}
			D3DXMatrixRotationAxis(&matTemp,&m_vVel,m_fLeanRate);
			D3DXVec3TransformCoord(&m_vUp,&m_vUp,&matTemp);
		}

		// ������Ʈ�� ���Ͱ� ���� �ʰ� �˻��Ͽ� ��ǥ �̵������ش� - �߰��ؾ���
		/////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////
		m_vOldPos = m_vPos;
	}
	else if(m_dwState==_EXPLODING)
	{
		if(m_pEffectTarget)
		{
			m_pEffectTarget->m_bUsing = FALSE;
			m_pEffectTarget = NULL;
			char message[256];
			wsprintf(message,STRMSG_C_TARGET_0001);	//"2�� Ÿ�� ������"
			g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_SYSTEM);
		}
		if(g_pShuttleChild->m_pOrderTarget == this)
			g_pShuttleChild->m_pOrderTarget = NULL;
		if(!COMPARE_BODYCON_BIT(m_info.BodyCondition,BODYCON_EXPLOSION_MASK))
		{
			m_info.BodyCondition = BODYCON_EXPLOSION_MASK;
			if(m_pCharacterInfo)
				m_pCharacterInfo->ChangeBodyCondition(m_info.BodyCondition);
		}
		if(g_pScene->m_byMapType == MAP_TYPE_TUTORIAL)
		{// �ʺ� �̼��� �� ���� �������� ����
			m_fMaterialCheckTime -= 2.0f*fElapsedTime;
			if(m_fMaterialCheckTime <= 0.0f)
			{
				m_fMaterialCheckTime = 0.0f;
				ChangeUnitState( _EXPLODED );
				//m_dwState = _EXPLODED;
			}
			if(m_fMaterialCheckTime < 1.0f)
				D3DUtil_InitMaterial( m_material, m_fMaterialCheckTime, m_fMaterialCheckTime, m_fMaterialCheckTime );// �ӽ� �ڵ�
		}
		m_fMaterialCheckTime -= 2.0f*fElapsedTime;
		if ( m_fMaterialCheckTime <= 0.0f )
		{
			m_fMaterialCheckTime = 0.0f;
			ChangeUnitState( _EXPLODED );
		}
		if ( m_fMaterialCheckTime < 1.0f )
		{
			D3DUtil_InitMaterial( m_material, m_fMaterialCheckTime, m_fMaterialCheckTime, m_fMaterialCheckTime );
		}
	}
	else if(m_dwState == _EXPLODED)
	{
		if(m_pEffectTarget)
		{
			m_pEffectTarget->m_bUsing = FALSE;
			m_pEffectTarget = NULL;
			char message[256];
			wsprintf(message,STRMSG_C_TARGET_0001);	//"2�� Ÿ�� ������"
			g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_SYSTEM);
		}
		if(g_pShuttleChild->m_pOrderTarget == this)
			g_pShuttleChild->m_pOrderTarget = NULL;
		m_bUsing = FALSE;
	}
	else if(m_dwState == _FALLING)
	{
//		if(COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_DAMAGE3_MASK))
//		{
//			ChangeBodyCondition(BODYCON_DAMAGE3_MASK | BODYCON_BOOSTER1_MASK);//�߶��ϸ鼭 ������ �ٵ������
//		}

		if(m_bRenderState && m_vPos.y < g_pGround->CheckHeightMap(m_vPos) + 
			(m_pMonMesh!=NULL?m_pMonMesh->m_fRadius : m_fObjectSize) )
		{
			ChangeBodyCondition(BODYCON_EXPLOSION_MASK);//�߶� �� ������ �ٵ������
			ChangeUnitState( _FALLEN );

			// 2004-10-19 by jschoi   ���⼭ ���� �Ҹ�
			util::del(m_pMonsterMovePattern);		

		}
		else
		{
			// ���⼭ ���� ����
			if(m_pMonsterMovePattern)
			{

				// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
				// ���� ���� ���� �߶��ϴ� ������ ��� ������ �ӵ� ����
				if( COMPARE_MPOPTION_BIT( m_pMonsterInfo->MPOption, MPOPTION_BIT_NO_ALPHABLENDING_IN_DEAD )
					&& m_pMonsterMovePattern->IsCinemaOver() )
				{
					m_pMonsterMovePattern->SetCurrentCinemaPointVelocity( 500.0f );
				}
				// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
								
				BOOL bSkillStop = m_pMonsterMovePattern->SkillTick();	
				EVENT_POINT SkillPoint;

				SkillPoint=m_pMonsterMovePattern->GetCurrentCinemaPoint();
				m_vPos = SkillPoint.vPosition;			// ��ġ
				m_vVel = SkillPoint.vDirection;			// ����
				m_vUp = SkillPoint.vUpVector;			// ������

			}
			else
			{ // ������ ���ٸ�..
			m_fFallingTime += fElapsedTime;
			if(1.0f - m_fFallingTime > 0.0f)
				m_vPos -= 40.0f*(1.0f - m_fFallingTime)*m_vVel*fElapsedTime;
			m_vPos.y -= 40.0f*fElapsedTime*m_fFallingTime;
		}
	}
	}
	else if(m_dwState == _FALLEN)
	{
		if(m_pEffectTarget)
		{
			m_pEffectTarget->m_bUsing = FALSE;
			m_pEffectTarget = NULL;
			char message[256];
			wsprintf(message,STRMSG_C_TARGET_0001);	//"2�� Ÿ�� ������"
			g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_SYSTEM);
		}
		if(g_pShuttleChild->m_pOrderTarget == this)
			g_pShuttleChild->m_pOrderTarget = NULL;

		m_fMaterialCheckTime -= 0.5f*fElapsedTime;
		if(m_fMaterialCheckTime <= 0.0f)
		{
			m_bUsing = FALSE;
		}
		if(m_fMaterialCheckTime < 1.0f)
		{
			D3DUtil_InitMaterial( m_material, m_fMaterialCheckTime, m_fMaterialCheckTime, m_fMaterialCheckTime );
		}
	}
	else if(m_dwState == _AUTODESTROYED)
	{
		// 2005-10-28 by ispark
		// ���� �Ҹ�
		if(m_pEffectTarget)
		{
			m_pEffectTarget->m_bUsing = FALSE;
			m_pEffectTarget = NULL;
			char message[256];
			wsprintf(message,STRMSG_C_TARGET_0001);	//"2�� Ÿ�� ������"
			g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_SYSTEM);
		}
		if(g_pShuttleChild->m_pOrderTarget == this)
			g_pShuttleChild->m_pOrderTarget = NULL;
		if(!COMPARE_BODYCON_BIT(m_info.BodyCondition,BODYCON_MON_AUTODESTROYED_MASK))
		{
			m_info.BodyCondition = BODYCON_MON_AUTODESTROYED_MASK;
			if(m_pCharacterInfo)
				m_pCharacterInfo->ChangeBodyCondition(m_info.BodyCondition);
		}	
		
		if(m_pCharacterInfo->IsUsing() == FALSE)
		{
			m_bUsing = FALSE;
		}
	}
	// �������� üũ
	if(g_pShuttleChild->CheckIsWater(m_vPos) && m_vPos.y < g_pGround->m_projectInfo.fWaterHeight)
		m_bIsUnderWater = TRUE;
	if(m_bAniColl && MOVE_AVAILABLE_STATE(m_dwState))
	{
		if(m_fAniCollTime >= 0.0f)
			m_fAniCollTime -= 3.0f*fElapsedTime;
		if(m_fAniCollTime < 0.0f)
		{
			m_fAniCollTime = 1.0f;
			m_bAniColl = FALSE;
		}
	}

	// ��ų ����� - 2004.06.25 jschoi
	if(m_nMonsterSkillState != MONSTER_SKILL_OFF)
	{
		UseSkillMove();
	}

	// 2004-11-26 by jschoi - TargetVector�� �������θ� �̵��Ǵ� ���� Ÿ�� �߰�
	if(m_info.MonsterForm == FORM_OBJECT_PLANE_ROTATE || m_info.MonsterForm == FORM_OBJECT_STOP)
	{
		m_vVel.y = 0;
		m_vUp = D3DXVECTOR3(0,1,0);
		// 2008-11-06 by bhsohn ���� ���� �������� ���� ����
		D3DXVec3Normalize(&m_vVel,&m_vVel);

		D3DXMatrixLookAtRH( &m_mMatrix, &m_vPos, &(m_vPos+m_vVel), &m_vUp);
	}
	else
	{
		D3DXMatrixLookAtRH( &m_mMatrix, &m_vPos, &(m_vPos+m_vVel), &m_vUp);
	}
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	m_mMatrix = m_mScale*m_mMatrix;

	m_vSideVel.x = m_mMatrix._11;
	m_vSideVel.y = m_mMatrix._12;
	m_vSideVel.z = m_mMatrix._13;
	D3DXVec3TransformCoord(&m_vLWPos, &m_PrimaryAttack.vSidePos, &m_mMatrix);
	m_vLWPos += 5.0f*m_vVel;
	D3DXVECTOR3 vPrimaryRightPos = D3DXVECTOR3(-m_PrimaryAttack.vSidePos.x, m_PrimaryAttack.vSidePos.y, m_PrimaryAttack.vSidePos.z);
	D3DXVec3TransformCoord(&m_vRWPos, &vPrimaryRightPos, &m_mMatrix);
	m_vRWPos += 5.0f*m_vVel;
	D3DXVec3TransformCoord(&m_vLWSecondaryPos, &m_SecondaryAttack.vSidePos, &m_mMatrix);
	D3DXVECTOR3 vSecondaryRightPos = D3DXVECTOR3(-m_SecondaryAttack.vSidePos.x, m_SecondaryAttack.vSidePos.y, m_SecondaryAttack.vSidePos.z);
	D3DXVec3TransformCoord(&m_vRWSecondaryPos, &vSecondaryRightPos, &m_mMatrix);

	g_pD3dApp->CalcObjectSourceScreenCoords(m_vPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
		m_nObjScreenX, m_nObjScreenY, m_nObjScreenW );

	// 2005-01-05 by jschoi - ���� ���� �޽� ��� ����.
//	m_bCheckBlend = FALSE;

//	m_bIsRender = FALSE;
	// 50.0f ��ġ�� ���Ŀ� ��Ʋ�� ũ��� ������ ũ���� ���� ������ ũ��� ���õǾ���Ѵ�.
	if(m_bRenderState && D3DXVec3Length(&(m_vPos - g_pShuttleChild->m_vPos)) < 
		20.0f+(m_pMonMesh!=NULL?m_pMonMesh->m_fRadius : m_fObjectSize) )
	{
		m_bCheckColl = TRUE;
		g_pShuttleChild->m_bCollMonsterCheck++;		// ��Ʋ���� �浹 üũ��ƾ�� ����� ����
	}
	else
		m_bCheckColl = FALSE;
	
	/// Monster �� ����ü �ø��� �����Ͽ� m_vecMonsterRenderList �� �����. - 2004-08-03 jschoi	
	if(m_bUsing)
	{
		BOOL bResult = g_pFrustum->CheckSphere( m_vPos.x, m_vPos.y, m_vPos.z, 
			(m_pMonMesh!=NULL?m_pMonMesh->m_fRadius : m_fObjectSize) );
		if(bResult)
		{
			g_pScene->m_vecMonsterRenderList.push_back(this);
			g_pScene->m_vecUnitRenderList.push_back((CUnitData*)this);

			m_fDistanceCamera = D3DXVec3Length(&(m_vPos - g_pD3dApp->m_pCamera->GetEyePt()));

			// 2005-01-05 by jschoi - ���� ���� �޽� ������.
//			D3DXVECTOR3 v1,v2;
//			m_bCheckBlend = FALSE;
//			D3DXVec3Normalize(&v1,&(g_pShuttleChild->m_vPos - g_pD3dApp->m_pCamera->GetEyePt()));
//			D3DXVec3Normalize(&v2,&(m_vPos - g_pD3dApp->m_pCamera->GetEyePt()));
//			if(g_pShuttleChild->m_fDistanceCamera + (m_pMonMesh!=NULL?m_pMonMesh->m_fRadius : m_fObjectSize) > 
//				m_fDistanceCamera)
//			{
//				if(sin(ACOS(D3DXVec3Dot(&v1,&v2)))*m_fDistanceCamera <= 
//					(m_pMonMesh!=NULL?m_pMonMesh->m_fRadius : m_fObjectSize) )
//				{
//					m_bCheckBlend = TRUE;
//				}
//			}
			
			if(m_fDistanceCamera < 150.0f)
				m_bProgressiveRate = 0;
			else if(m_fDistanceCamera < 200.0f)
				m_bProgressiveRate = 1;
			else if(m_fDistanceCamera < 290.0f)
				m_bProgressiveRate = 2;
			else if(m_fDistanceCamera < 310.0f)
				m_bProgressiveRate = 3;
			else if(m_fDistanceCamera < 330.0f)
				m_bProgressiveRate = 4;
			else
				m_bProgressiveRate = 5;
				
		}	
	}

	// �׸��� �׸� ���� ���� 2004-07-29 jschoi
	if(m_bRenderState && m_bUsing)
	{
	D3DXVECTOR3 vShadowPos;
	vShadowPos = m_vPos;
	BOOL bIsWater = g_pShuttleChild->CheckIsWater(m_vPos);
	if(bIsWater)
	{
		vShadowPos.y = g_pGround->m_projectInfo.fWaterHeight;
	}
	else
	{
		vShadowPos.y = g_pCamera->CheckMap(m_vPos);
	}
	m_bShadowIsRender = FALSE;
	if(g_pSOption->sShadowState > 0)
	{
		BOOL bResult = g_pFrustum->CheckSphere( vShadowPos.x, vShadowPos.y, vShadowPos.z, m_fObjectSize );
		float fDistanceToCam = D3DXVec3Length(&(vShadowPos - g_pD3dApp->m_pCamera->GetEyePt()));
	//	if( g_pSOption->sShadowState && 
	//		g_pFrustum->CheckSphere( vShadowPos.x, vShadowPos.y, vShadowPos.z, m_fObjectSize ) &&
	//		D3DXVec3Length(&(vShadowPos - g_pD3dApp->m_pCamera->GetEyePt())) < SHADOW_MAX_DISTANCE)
//		if( bResult && 
//			fDistanceToCam < g_pScene->m_fFogEndValue*g_pSOption->sShadowState/MAX_OPTION_VALUE)
		if( bResult && 
			fDistanceToCam < RANGE_OF_VISION*g_pSOption->sShadowState/MAX_OPTION_VALUE)
		{
			m_bShadowIsRender = TRUE;
			g_pScene->m_vecMonsterShadowRenderList.push_back(this);
			CheckShadowPos();
			}		
		}
	}

	if(g_pScene->m_byMapType != MAP_TYPE_TUTORIAL)
	{
		float fHeight;
		if(m_pMonsterInfo)
			fHeight = g_pGround->CheckHeightMap(m_vPos) - m_pMonsterInfo->Size;
		else
			fHeight = g_pGround->CheckHeightMap(m_vPos) - 20.0f;
		if(m_vPos.y < fHeight)
		{
			if(g_pShuttleChild->m_pOrderTarget == this)
			{
				g_pShuttleChild->m_pOrderTarget = NULL;
//				m_nMonsterTypePrimary = MONSTERDATA_MONSTERLIST;
//				m_nMonsterTypeSecondary = MONSTERDATA_MONSTERLIST;
				m_bShadowIsRender = FALSE;
			}
		}
	}

	// �ʺ� �� �϶�(����Ʈ��)
	if(g_pScene->m_byMapType == MAP_TYPE_TUTORIAL)
	{
		if(g_pShuttleChild->m_pTarget == this)
		{
			if(!COMPARE_BODYCON_BIT(m_info.BodyCondition,BODYCON_SHAKEING_MASK))
			{
				m_info.BodyCondition = BODYCON_SHAKEING_MASK;
				if(m_pCharacterInfo)
					m_pCharacterInfo->ChangeBodyCondition(m_info.BodyCondition);
			}
		}
		else
		{
			if(COMPARE_BODYCON_BIT(m_info.BodyCondition,BODYCON_SHAKEING_MASK))
			{
				m_info.BodyCondition = BODYCON_FREEZING_MASK;
				if(m_pCharacterInfo)
					m_pCharacterInfo->ChangeBodyCondition(m_info.BodyCondition);
			}
		}
	}

	m_pMoveChat->Tick();
	m_pIDChat->Tick();
	// effect matrix & ticking
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->SetAllBodyConditionMatrix( m_mMatrix );
		m_pCharacterInfo->Tick(fElapsedTime);
	}
	CheckAniTime(fElapsedTime);

	m_ptCurRegion.x = (int)(m_vPos.x/(TILE_SIZE*3));
	m_ptCurRegion.y = (int)(m_vPos.z/(TILE_SIZE*3));

	if( m_ptCurRegion.x < 0 )
	{
		m_ptCurRegion.x = 0;
		//DBGOUT("MONSTER m_ptCurRegion.x < 0\n");
	}
	if( m_ptCurRegion.y < 0 )
	{
		m_ptCurRegion.y = 0;
		//DBGOUT("MONSTER m_ptCurRegion.y < 0\n");
	}
	if( m_ptCurRegion.x > g_pGround->m_projectInfo.sXSize/3 )
	{
		m_ptCurRegion.x = g_pGround->m_projectInfo.sXSize/3;
		//DBGOUT("MONSTER m_ptCurRegion.x > g_pGround->m_projectInfo.sXSize/3\n");
	}
	if( m_ptCurRegion.y > g_pGround->m_projectInfo.sYSize/3 )
	{
		m_ptCurRegion.y = g_pGround->m_projectInfo.sYSize/3;
		//DBGOUT("MONSTER m_ptCurRegion.y > g_pGround->m_projectInfo.sYSize/3\n");
	}

	if(m_ptCurRegion.x != m_ptOldRegion.x || m_ptCurRegion.y != m_ptOldRegion.y)
	{
		g_pScene->ChangeToBlockData(this);
	}

	// 2007-05-17 by bhsohn ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��
	TickObjectCheck();

	// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���

	// 2011. 09. 28 by jskim �� Ÿ�� �ý��� ���� ����
	//if(m_nindexSize > NULL)
	if(m_nindexSize > 0)
	// end 2011. 09. 28 by jskim �� Ÿ�� �ý��� ���� ����
	{
		for( int i=0; i < m_nindexSize; i ++)
		{	
			D3DXVECTOR3 tempvec = m_vecMonMultiInfo[i].TargetPosition;
			D3DXMATRIX tempmat;
			D3DXMatrixIdentity(&tempmat);
			tempmat._41 = tempvec.x;
			tempmat._42 = tempvec.y;
			tempmat._43 = tempvec.z;
			tempmat = tempmat * m_mMatrix;
			
			m_vecvmultiData[i]->TargetMatrix = tempmat;	
		}		
	}
	// end 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
}

void CMonsterData::CheckAniTime(float fElapsedTime)
{
	FLOG( "CMonsterData::CheckAniTime(float fElapsedTime)" );
	if(!m_pCharacterInfo)
	{
		m_fCurrentTime += 20.0f*fElapsedTime;
		if(m_fCurrentTime >= 70.0f)
			m_fCurrentTime = 0.0f;
		return;
	}
	m_fCurrentTime = GetCurrentAnimationTime();
	BodyCond_t hyBody = GetCurrentBodyCondition() & BODYCON_MON_ATTACKALL_MASK;
	if(hyBody)
	{
		if( m_fCurrentTime == GetCurrentBodyConditionEndAnimationTime())
		{
			ChangeBodyCondition( GetCurrentBodyCondition() & ~hyBody );
		}
	}

}


void CMonsterData::Render()
{
	FLOG( "CMonsterData::Render()" );
/*	if(m_pRenderer && m_bRenderState && m_pMonsterInfo
		&& m_nObjScreenX > -m_fObjectSize && m_nObjScreenX < g_pD3dApp->GetBackBufferDesc().Width+m_fObjectSize 
		&& m_nObjScreenY > -m_fObjectSize && m_nObjScreenY < g_pD3dApp->GetBackBufferDesc().Height+m_fObjectSize 
		&& m_nObjScreenW > -m_fObjectSize && m_pMonsterInfo->MonsterUnitKind > 0)
*/
	if(m_pRenderer)
	{
		if(m_pMonsterInfo)
		{
			/// ���� ����
			// 2006-08-14 by ispark, ���� ���� �缳��
			// ���ָʿ����� �� ������ ������ �޴´�.
			// ���̴ٰ� ������ ��Ӱ� �Ѵ�.
			if( m_bCharacter == FALSE && 
				g_pStoreData->FindItemInInventoryByWindowPos(POS_PROW) != NULL &&
				(g_pScene->m_bNight == TRUE ||
				IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE))
			{
//				g_pD3dDev->SetLight( 0, &light ); /// ����̽��� 0�� ���� ��ġ 
				g_pD3dDev->SetLight( 0, &g_pScene->m_light0 ); /// ����̽��� 0�� ���� ��ġ 
				g_pD3dDev->SetLight( 1, &g_pScene->m_light1 );
				g_pD3dDev->LightEnable( 0, TRUE ); /// 0�� ������ �Ҵ� 
				g_pD3dDev->LightEnable( 1, TRUE ); /// 1�� ������ �Ҵ� 
				g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE ); /// ���������� �Ҵ� 			
//				g_pD3dDev->SetRenderState( D3DRS_AMBIENT, 0x00505050 ); /// ȯ�汤��(ambient light)�� �� ����
			}
			else
			{
				g_pD3dDev->SetLight( 0, &g_pScene->m_light0 ); /// ����̽��� 0�� ���� ��ġ 
				g_pD3dDev->LightEnable( 0, TRUE ); /// 0�� ������ �Ҵ� 
				g_pD3dDev->LightEnable( 1, FALSE ); /// 1�� ������ �Ҵ� 
				g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE ); /// ���������� �Ҵ�
			}			
			m_pRenderer->Render(this);
			
			g_pD3dDev->SetLight( 0, &g_pScene->m_light0 ); /// ����̽��� 0�� ���� ��ġ 
			g_pD3dDev->LightEnable( 0, TRUE ); /// 0�� ������ �Ҵ� 
			g_pD3dDev->LightEnable( 1, FALSE ); /// 1�� ������ �Ҵ� 
			g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE ); /// ���������� �Ҵ� 	
		}
		else
		{
			Init();// ���� ������ �������� ���� �ʾ� �ٽ� ��û�Ѵ�.
			DBGOUT("Request MonsterInfo(%d) to Server\n", m_info.MonsterUnitKind);
		}
	}
}

void CMonsterData::RenderShadow()
{
	if(m_pRenderer)
	{
		if(m_pMonsterInfo)
		{
			m_pRenderer->RenderShadow(this);
		}
		else
		{
			Init();// ���� ������ �������� ���� �ʾ� �ٽ� ��û�Ѵ�.
			DBGOUT("Request MonsterInfo(%d) to Server\n", m_info.MonsterUnitKind);
		}
	}
}

void CMonsterData::SetPosition(MSG_FC_MOVE_OK* pMsg)
{
	FLOG( "CMonsterData::SetPosition(MSG_FC_MOVE_OK* pMsg)" );
	if(MOVE_AVAILABLE_STATE(m_dwState))
	{
		m_vNextPos = A2DX(pMsg->PositionVector);
		D3DXVec3Normalize(&m_vNextVel,&A2DX(pMsg->TargetVector));

		m_vUp = D3DXVECTOR3(0,1,0);

		float fLength = D3DXVec3Length(&(m_vNextPos - m_vPos));
		// �Ÿ����� �߻� ������ ���������� ����Ѵ�.-������ �ӵ��� ����Ͽ� ���
		m_vExpectPos = m_vNextPos + (m_vNextPos - m_vPos)*0.5f;//*fLength;
		m_vMovepos = m_vExpectPos - m_vPos;
		fLength = D3DXVec3Length(&m_vMovepos);
		// ���� �ӵ��� ��Ʋ�� �ִ� �ӵ����� ũ�� �ִ� �ӵ���ŭ�� ũ��� �ٿ�����Ѵ�.
		if(!COMPARE_BODYCON_BIT(m_info.BodyCondition,BODYCON_CREATION_MASK) && 
			(fLength > 250.0f || m_fLifeTime <= 0.0f))
		{
			D3DXVec3Normalize(&m_vMovepos,&m_vMovepos);
			m_vPos = m_vNextPos;
		}
		// ����� �����ʹ� ��ǥó�� ũ�� ������ ���� �����Ƿ� Expect�� ������ �ʰڴ�.
		m_vMoveVel = m_vNextVel - m_vVel;
	}
//	DBGOUT("Monster(%d) LifeTime = %.2f\n", this->m_info.MonsterIndex, m_fLifeTime);
	m_fLifeTime = MONSTER_LIFE_TIME;
}

void CMonsterData::SetMonPosition(MSG_FC_MONSTER_MOVE_OK* pMsg)
{
//	DBGOUT("Monster(%d) LifeTime = %.2f\n", this->m_info.MonsterIndex, m_fLifeTime);
	m_fLifeTime = MONSTER_LIFE_TIME;
	if(m_nMonsterSkillState != MONSTER_SKILL_OFF)
		return;
	FLOG( "CMonsterData::SetMonPosition(MSG_FC_MONSTER_MOVE_OK* pMsg)" );

	// ������ Ÿ�� �ε���
	m_nTargetIndex = pMsg->TargetIndex;
	
	if(m_info.MonsterForm == FORM_OBJECT_STOP)	// ������ ������ ���
	{
		m_vNextPos = A2DX(pMsg->PositionVector);
		D3DXVec3Normalize(&m_vNextVel,&A2DX(pMsg->TargetVector));
		m_vUp = D3DXVECTOR3(0,1,0);
		m_vExpectPos = m_vNextPos;
		m_vMovepos = D3DXVECTOR3(0,0,0);
		m_vVel = m_vNextVel;
		m_vPos = m_vNextPos;
	}
	else										// ������ ���Ͱ� �ƴѰ��
	{
		if(MOVE_AVAILABLE_STATE(m_dwState))
		{
	//		if(!COMPARE_BODYCON_BIT(m_info.BodyCondition,BODYCON_CREATION_MASK) && m_fLifeTime <= 1.0f)
	//		{// 1.5�ʵ��� �������� ���� �ʴٰ� ������ ���� �����Ƿ� ����ǥ�� �̵���Ų��.
	//		}
			m_vNextPos = A2DX(pMsg->PositionVector);
			D3DXVec3Normalize(&m_vNextVel,&A2DX(pMsg->TargetVector));

			m_vUp = D3DXVECTOR3(0,1,0);

			float fLength = D3DXVec3Length(&(m_vNextPos - m_vPos));
			// �Ÿ����� �߻� ������ ���������� ����Ѵ�.-������ �ӵ��� ����Ͽ� ���
			m_vExpectPos = m_vNextPos + (m_vNextPos - m_vPos)*0.5f;//*fLength;
			m_vMovepos = m_vExpectPos - m_vPos;
			fLength = D3DXVec3Length(&m_vMovepos);
			// ���� �ӵ��� ��Ʋ�� �ִ� �ӵ����� ũ�� �ִ� �ӵ���ŭ�� ũ��� �ٿ�����Ѵ�.
			if(!COMPARE_BODYCON_BIT(m_info.BodyCondition,BODYCON_CREATION_MASK) && 
				(fLength > 500.0f || m_fLifeTime <= 0.0f))
			{
				D3DXVec3Normalize(&m_vMovepos,&m_vMovepos);
				m_vPos = m_vNextPos;
	//			m_vPos = A2DX(pMsg->PositionVector);
				D3DXVec3Normalize(&m_vVel,&A2DX(pMsg->TargetVector));
//				DBGOUT("Monster : Different Position to Set \n");//��ǥ ���̰� ���� ���� �ѹ��� �̵��մϴ�.
			}
			// ����� �����ʹ� ��ǥó�� ũ�� ������ ���� �����Ƿ� Expect�� ������ �ʰڴ�.
			if(m_dwState == _LANDED)
			{
				D3DXVECTOR3 vNormal = GetMapNormalVector(m_vPos);
				D3DXVECTOR3 vSide;
				D3DXVec3Cross(&vSide,&vNormal,&m_vNextVel);
				D3DXVec3Cross(&m_vNextVel,&vSide,&vNormal);
				D3DXVec3Normalize(&m_vNextVel,&m_vNextVel);
				m_vUp = vNormal;
			}
			m_vMoveVel = m_vNextVel - m_vVel;
		}

	}
}

void CMonsterData::CheckShadowPos()
{
	FLOG( "CMonsterData::CheckShadowPos()" );
	if( IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE )
	{
		m_bShadowIsRender = FALSE;
		return;
	}
	D3DXVec3Normalize( &m_vSideVel, &m_vSideVel );
	m_vShadowPos1 = m_vPos+m_fObjectSize*(m_vVel+m_vSideVel);
	m_vShadowPos2 = m_vPos+m_fObjectSize*(m_vVel-m_vSideVel);
	m_vShadowPos3 = m_vPos-m_fObjectSize*(m_vVel+m_vSideVel);
	m_vShadowPos4 = m_vPos-m_fObjectSize*(m_vVel-m_vSideVel);

//	FLOAT fBary1, fBary2;
	FLOAT fDist;
	int i,j;
	i = (int)(m_vShadowPos1.x/TILE_SIZE);
	j = (int)(m_vShadowPos1.z/TILE_SIZE);
	D3DXVECTOR3 v,orig,dir;//,v2;
	orig = m_vShadowPos1;
	dir = D3DXVECTOR3(0,-1,0);
	if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
	{
		if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
			g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			m_vShadowPos1 = orig + dir*fDist;
			if(g_pShuttleChild->CheckIsWater(orig))
			{
				if(m_vShadowPos1.y < g_pGround->m_projectInfo.fWaterHeight)
					m_vShadowPos1.y = g_pGround->m_projectInfo.fWaterHeight;
			}
		}
		else if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			m_vShadowPos1 = orig + dir*fDist;
			if(g_pShuttleChild->CheckIsWater(orig))
			{
				if(m_vShadowPos1.y < g_pGround->m_projectInfo.fWaterHeight)
					m_vShadowPos1.y = g_pGround->m_projectInfo.fWaterHeight;
			}
		}
	}
	i = (int)(m_vShadowPos2.x/TILE_SIZE);
	j = (int)(m_vShadowPos2.z/TILE_SIZE);
	orig = m_vShadowPos2;
	dir = D3DXVECTOR3(0,-1,0);
	if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
	{
		if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
			g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			m_vShadowPos2 = orig + dir*fDist;
			if(g_pShuttleChild->CheckIsWater(orig))
			{
				if(m_vShadowPos2.y < g_pGround->m_projectInfo.fWaterHeight)
					m_vShadowPos2.y = g_pGround->m_projectInfo.fWaterHeight;
			}
		}
		else if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			m_vShadowPos2 = orig + dir*fDist;
			if(g_pShuttleChild->CheckIsWater(orig))
			{
				if(m_vShadowPos2.y < g_pGround->m_projectInfo.fWaterHeight)
					m_vShadowPos2.y = g_pGround->m_projectInfo.fWaterHeight;
			}
		}
	}
	i = (int)(m_vShadowPos3.x/TILE_SIZE);
	j = (int)(m_vShadowPos3.z/TILE_SIZE);
	orig = m_vShadowPos3;
	dir = D3DXVECTOR3(0,-1,0);
	if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
	{
		if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
			g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			m_vShadowPos3 = orig + dir*fDist;
			if(g_pShuttleChild->CheckIsWater(orig))
			{
				if(m_vShadowPos3.y < g_pGround->m_projectInfo.fWaterHeight)
					m_vShadowPos3.y = g_pGround->m_projectInfo.fWaterHeight;
			}
		}
		else if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			m_vShadowPos3 = orig + dir*fDist;
			if(g_pShuttleChild->CheckIsWater(orig))
			{
				if(m_vShadowPos3.y < g_pGround->m_projectInfo.fWaterHeight)
					m_vShadowPos3.y = g_pGround->m_projectInfo.fWaterHeight;
			}
		}
	}
	i = (int)(m_vShadowPos4.x/TILE_SIZE);
	j = (int)(m_vShadowPos4.z/TILE_SIZE);
	orig = m_vShadowPos4;
	dir = D3DXVECTOR3(0,-1,0);
	if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
	{
		if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
			g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			m_vShadowPos4 = orig + dir*fDist;
			if(g_pShuttleChild->CheckIsWater(orig))
			{
				if(m_vShadowPos4.y < g_pGround->m_projectInfo.fWaterHeight)
					m_vShadowPos4.y = g_pGround->m_projectInfo.fWaterHeight;
			}
		}
		else if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			m_vShadowPos4 = orig + dir*fDist;
			if(g_pShuttleChild->CheckIsWater(orig))
			{
				if(m_vShadowPos4.y < g_pGround->m_projectInfo.fWaterHeight)
					m_vShadowPos4.y = g_pGround->m_projectInfo.fWaterHeight;
			}
		}
	}
	SPRITEVERTEX* pV;
	m_pVBShadow->Lock( 0, 0, (void**)&pV, 0 );
	pV[0].p = m_vShadowPos4; 
	pV[1].p = m_vShadowPos1; 
	pV[2].p = m_vShadowPos3; 
	pV[3].p = m_vShadowPos2;
	m_pVBShadow->Unlock();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CMonsterData::ChangeBodyConditionFromServer(BodyCond_t hyBodyCondition)
/// \brief		�������� ���� �ٵ������ ü����
/// \author		dhkwon
/// \date		2004-05-17 ~ 2004-05-17
/// \warning	LANDED : LANDING -> LANDED, DEAD : 2���� Ÿ��(EXPLOSION, FALL)
///				
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CMonsterData::ChangeBodyConditionFromServer(BodyCond_t hyBodyCondition)
{
	if((!COMPARE_BODYCON_BIT(m_info.BodyCondition,BODYCON_DEAD_MASK) && 
		COMPARE_BODYCON_BIT(hyBodyCondition,BODYCON_DEAD_MASK)) ||
		(!COMPARE_BODYCON_BIT(m_info.BodyCondition,BODYCON_MON_AUTODESTROYED_MASK) && 
		COMPARE_BODYCON_BIT(hyBodyCondition,BODYCON_MON_AUTODESTROYED_MASK)))
	{
		// target����, weapon����
		if( g_pShuttleChild->m_pOrderTarget && 
			g_pShuttleChild->m_pOrderTarget == this)
		{
			g_pShuttleChild->m_pOrderTarget = NULL;
		}
		CWeapon * pNode = (CWeapon *)g_pScene->m_pWeaponData->m_pChild;
		while(pNode)
		{
			if(pNode->m_pTarget == this)
			{
				pNode->m_pTarget = NULL;
				
				// 2004-12-02 by ydkim ���ΰ��ݽ� ������������ Ÿ���� ����������
				if(pNode->m_dwPartType == _MINE
					&& pNode->m_dwWeaponState == _EXPLODING)
				{
					pNode->m_dwWeaponState = _NORMAL;
				}
			}
			pNode = (CWeapon *)pNode->m_pNext;
		}
	}
	if(m_pCharacterInfo)
	{
		BodyCond_t hyAttackBodyCondition = hyBodyCondition & BODYCON_MON_ATTACKALL_MASK;
		switch(hyAttackBodyCondition)
		{
		case BODYCON_MON_FIREATTACK1_MASK:
			{
//				m_PrimaryAttack.vSidePos = m_pCharacterInfo->GetEffectPos( BODYCON_MON_FIREATTACK1_MASK, RC_EFF_DUMMY_POSITION );
				m_SecondaryAttack.vSidePos = m_pCharacterInfo->GetEffectPos( BODYCON_MON_FIREATTACK1_MASK, RC_EFF_DUMMY_POSITION );
			}
			break;
		case BODYCON_MON_FIREATTACK2_MASK:
			{
//				m_PrimaryAttack.vSidePos = m_pCharacterInfo->GetEffectPos( BODYCON_MON_FIREATTACK2_MASK, RC_EFF_DUMMY_POSITION );
				m_SecondaryAttack.vSidePos = m_pCharacterInfo->GetEffectPos( BODYCON_MON_FIREATTACK2_MASK, RC_EFF_DUMMY_POSITION );
			}
			break;
		case BODYCON_MON_FIREATTACK3_MASK:
			{
//				m_PrimaryAttack.vSidePos = m_pCharacterInfo->GetEffectPos( BODYCON_MON_FIREATTACK3_MASK, RC_EFF_DUMMY_POSITION );
				m_SecondaryAttack.vSidePos = m_pCharacterInfo->GetEffectPos( BODYCON_MON_FIREATTACK3_MASK, RC_EFF_DUMMY_POSITION );
			}
			break;
		case BODYCON_MON_FIREATTACK4_MASK:
			{
//				m_PrimaryAttack.vSidePos = m_pCharacterInfo->GetEffectPos( BODYCON_MON_FIREATTACK4_MASK, RC_EFF_DUMMY_POSITION );
				m_SecondaryAttack.vSidePos = m_pCharacterInfo->GetEffectPos( BODYCON_MON_FIREATTACK4_MASK, RC_EFF_DUMMY_POSITION );
			}
			break;
		case BODYCON_MON_FIREATTACK5_MASK:
			{
//				m_PrimaryAttack.vSidePos = m_pCharacterInfo->GetEffectPos( BODYCON_MON_FIREATTACK5_MASK, RC_EFF_DUMMY_POSITION );
				m_SecondaryAttack.vSidePos = m_pCharacterInfo->GetEffectPos( BODYCON_MON_FIREATTACK5_MASK, RC_EFF_DUMMY_POSITION );
			}
			break;
		case BODYCON_MON_FIREATTACK6_MASK:
			{
//				m_PrimaryAttack.vSidePos = m_pCharacterInfo->GetEffectPos( BODYCON_MON_FIREATTACK6_MASK, RC_EFF_DUMMY_POSITION );
				m_SecondaryAttack.vSidePos = m_pCharacterInfo->GetEffectPos( BODYCON_MON_FIREATTACK6_MASK, RC_EFF_DUMMY_POSITION );
			}
			break;

		// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
		case BODYCON_MON_FIREATTACK7_MASK:
		case BODYCON_MON_FIREATTACK8_MASK:
		case BODYCON_MON_FIREATTACK9_MASK:
		case BODYCON_MON_FIREATTACK10_MASK:
		case BODYCON_MON_FIREATTACK11_MASK:
		case BODYCON_MON_FIREATTACK12_MASK:
		case BODYCON_MON_FIREATTACK13_MASK:
		case BODYCON_MON_FIREATTACK14_MASK:
		case BODYCON_MON_FIREATTACK15_MASK:
			{
				m_SecondaryAttack.vSidePos = m_pCharacterInfo->GetEffectPos( hyAttackBodyCondition, RC_EFF_DUMMY_POSITION );
			}
			break;
		// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����

		}
		// 2006-08-23 by ispark, �ٵ� ������� �޾Ƽ� ��ǥ�� ���� ������ tick()�� ���� ���� ���⸦ �����Ѵ�.
		// �׷��� ���� ���� �� ���� ��ǥ���� ���Ⱑ �߻簡 �ȴ�.
		// �׷��� �ؿ��� ������ �Ѵ�.
		D3DXVec3TransformCoord(&m_vLWSecondaryPos, &m_SecondaryAttack.vSidePos, &m_mMatrix);
		D3DXVECTOR3 vSecondaryRightPos = D3DXVECTOR3(-m_SecondaryAttack.vSidePos.x, m_SecondaryAttack.vSidePos.y, m_SecondaryAttack.vSidePos.z);
		D3DXVec3TransformCoord(&m_vRWSecondaryPos, &vSecondaryRightPos, &m_mMatrix);
	}
	ChangeBodyCondition(hyBodyCondition);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CMonsterData::ChangeBodyCondition(BodyCond_t hyBodyCondition)
/// \brief		������ �ٵ�������� �ٲ۴�.
/// \author		dhkwon
/// \date		2004-04-09 ~ 2004-04-09
/// \warning	CUnitData�� ChangeBodyCondition�� UNIT���� ����Ǳ� ������ ���ʹ� ���⼭ �������ؼ� ����.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CMonsterData::ChangeBodyCondition(BodyCond_t hyBodyCondition)
{
	FLOG( "CMonsterData::ChangeBodyCondition()" );
	m_info.BodyCondition = hyBodyCondition;
	if(m_pCharacterInfo == NULL)
	{
		return;
	}
	if(hyBodyCondition & BODYCON_FLY_MASK)//FLY����
	{
		ChangeUnitState( _NORMAL );
		hyBodyCondition = hyBodyCondition & ~BODYCON_FLY_MASK;
		if( COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_BOOSTER3_MASK) && 
			!COMPARE_BODYCON_BIT(hyBodyCondition,BODYCON_BOOSTER3_MASK))
		{
//			m_pCharacterInfo->ChangeBodyCondition((hyBodyCondition & ~BODYCON_MONSTER_SINGLE) | BODYCON_BOOSTER4_MASK);//�ν��� ����
//			return;
			hyBodyCondition = (hyBodyCondition & ~BODYCON_MONSTER_SINGLE) | BODYCON_BOOSTER4_MASK;
		}
	}
	else if(hyBodyCondition & BODYCON_LANDED_MASK)//LANDED����
	{
		if( COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_LANDED_MASK) && 
			!COMPARE_BODYCON_BIT(hyBodyCondition,BODYCON_LANDED_MASK))
		{
			m_pCharacterInfo->ChangeBodyCondition( (hyBodyCondition & ~BODYCON_MONSTER_SINGLE) | BODYCON_LANDING_MASK);//����
			ChangeUnitState(_LANDING);
			return;
		}
		else
		{
			ChangeUnitState(_LANDED);
		}
	}
	else if(hyBodyCondition & BODYCON_DEAD_MASK)//DEAD����
	{
		if(!COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_DEAD_MASK) &&
			COMPARE_BODYCON_BIT(hyBodyCondition,BODYCON_DEAD_MASK))
		{
			DelSecondWeapon();

			// 2010. 02. 04 by ckPark ���� �غ� ���߿� ���� ���� �ִϸ��̼� ���� ����
			m_pCharacterInfo->ForceTurnOffBodyCondition( BODYCON_MON_ATTACKALL_MASK );
			// end 2010. 02. 04 by ckPark ���� �غ� ���߿� ���� ���� �ִϸ��̼� ���� ����

			if(COMPARE_BODYCON_BIT(hyBodyCondition,BODYCON_EXPLOSION_MASK))
			{
				m_pCharacterInfo->ChangeBodyCondition( (hyBodyCondition & ~BODYCON_MONSTER_SINGLE) | BODYCON_EXPLOSION_MASK );
				ChangeUnitState( _EXPLODING );
//				DBGOUT("����(%x) EXPLODING... \n",this);
				return;
			}
			else // BODYCON_FALL_MASK, �߶� ����
			{
				m_pCharacterInfo->ChangeBodyCondition( (hyBodyCondition & ~BODYCON_MONSTER_SINGLE) | (BODYCON_EXPLOSION_MASK|BODYCON_DAMAGE3_MASK) );
				ChangeUnitState( _FALLING );
				
				// 2004-10-19 by jschoi  ���⼭ ���� ����
				m_mMatrix._32 = 0;
				m_mMatrix._21 = 0;
				m_mMatrix._22 = 1;
				m_mMatrix._23 = 0;
				InitMonsterMovePattern(PATTERN_UNIT_FALLING);

//				DBGOUT("����(%x) FALLING... \n",this);
				return;
			}
		}
	}
	else if(hyBodyCondition & BODYCON_MON_AUTODESTROYED_MASK)// ���� �Ҹ�
	{
		if(!COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_MON_AUTODESTROYED_MASK) &&
			COMPARE_BODYCON_BIT(hyBodyCondition,BODYCON_MON_AUTODESTROYED_MASK))
		{
			if(COMPARE_BODYCON_BIT(hyBodyCondition,BODYCON_MON_AUTODESTROYED_MASK))
			{
				// 2005-10-28 by ispark
				m_pCharacterInfo->ChangeBodyCondition( (hyBodyCondition & ~BODYCON_MONSTER_SINGLE) | BODYCON_MON_AUTODESTROYED_MASK );
				ChangeUnitState( _AUTODESTROYED );
				return;
			}
		}
	}
	// object ������ ���� ChangeBodyCondition�� ���� �Ϲ� ������ ���� Reset�� ����.
	if(m_info.MonsterForm >= FORM_OBJECT_STOP && m_info.MonsterForm <= FORM_OBJECT_CANNON)
	{
		m_pCharacterInfo->ChangeBodyCondition( hyBodyCondition );
	}
	else
	{
		m_pCharacterInfo->ResetBodyCondition( hyBodyCondition );
	}
//	m_pCharacterInfo->ResetBodyCondition( hyBodyCondition ); //(hyBodyCondition & ~BODYCON_MONSTER_SINGLE) | hyBodyCondition );
//	m_pCharacterInfo->ChangeBodyCondition( hyBodyCondition ); //(hyBodyCondition & ~BODYCON_MONSTER_SINGLE) | hyBodyCondition );
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CMonsterData::MonsterChangeHPFromServer( INT CurrentHP )
/// \brief		�������� �� ���� HP ����
/// \author		dhkwon
/// \date		2004-05-17 ~ 2004-05-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//void CMonsterData::MonsterChangeHPFromServer( short CurrentHP )
void CMonsterData::MonsterChangeHPFromServer( INT CurrentHP )
{
	m_info.CurrentHP = CurrentHP;
	if(m_info.CurrentHP < 0.0f)
	{
		m_info.CurrentHP = 0.0f;
	}
	if(m_pMonsterInfo)
	{

		// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
		//float fRate = (float)CurrentHP/(float)m_pMonsterInfo->HP;
		float fRate = (float)CurrentHP/(float)m_info.MaxHP;
		// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )


		// damage bodycondition ����
		if(fRate < CHARACTER_DAMAGE_STATE_05)
		{
			// ������ 1,2,3�� ��� Ȱ��ȭ
			ChangeKeepingBodyCondition( BODYCON_DAMAGE1_MASK | BODYCON_DAMAGE2_MASK | BODYCON_DAMAGE3_MASK );
		}
		else if(fRate < CHARACTER_DAMAGE_STATE_10)
		{
			// ������ 1,2�� Ȱ��ȭ
			ChangeKeepingBodyCondition( BODYCON_DAMAGE1_MASK | BODYCON_DAMAGE2_MASK );
		}
		else if(fRate < CHARACTER_DAMAGE_STATE_15)
		{
			// ������ 1�� Ȱ��ȭ
			ChangeKeepingBodyCondition( BODYCON_DAMAGE1_MASK );
		}
		else
		{
			// ������ ��Ȱ��ȭ
			ChangeKeepingBodyCondition( GetCurrentBodyCondition() & ~(BODYCON_DAMAGE1_MASK | BODYCON_DAMAGE2_MASK | BODYCON_DAMAGE3_MASK ) );
		}
	}
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			D3DXVECTOR3 CMonsterData::GetMapNormalVector(D3DXVECTOR3 vPos)
/// \brief		���� ������ Ÿ�ٰ� ����Ÿ�� Ŭ���̾�Ʈ���� ����Ѵ�.
/// \author		dhkwon
/// \date		2004-05-29 ~ 2004-05-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////

D3DXVECTOR3 CMonsterData::GetMapNormalVector(D3DXVECTOR3 vPos)
{
	FLOG( "CShuttleChild::CheckMap(D3DXVECTOR3 vPos)" );
	int i,j;
	i = (int)(vPos.x/TILE_SIZE);
	j = (int)(vPos.z/TILE_SIZE);
	D3DXVECTOR3 vNormal;
	int nMapSizeX = g_pGround->m_projectInfo.sXSize;
	int nMapSizeY = g_pGround->m_projectInfo.sYSize;
	if( i >= 0 && 
		i < nMapSizeX && 
		j >= 0 && 
		j < nMapSizeY)
	{
		int k = (i*nMapSizeY+j);

		// 2005-04-06 by jschoi
//		if(g_pGround->m_pTileInfo[k].useWater)// �� ��
		if(g_pGround->m_pTileInfo[k].dwWaterType == 1)// �� ��
		{
			return D3DXVECTOR3(0,1,0);
		}
		float fDist;
		D3DXVECTOR3 dir = D3DXVECTOR3(0,-1,0);
		if(g_pGround->IntersectTriangle(vPos,dir,g_pGround->m_pTileVertexArray[i*(nMapSizeY+1)+j].pos,
			g_pGround->m_pTileVertexArray[i*(nMapSizeY+1)+(j+1)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(nMapSizeY+1)+(j)].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			// ������ ��ֺ��͸� ���Ѵ�.
			D3DXVec3Cross(&vNormal,&(g_pGround->m_pTileVertexArray[(i)*(nMapSizeY+1)+(j+1)].pos
				-g_pGround->m_pTileVertexArray[(i)*(nMapSizeY+1)+j].pos),
				&(g_pGround->m_pTileVertexArray[(i+1)*(nMapSizeY+1)+(j)].pos
				-g_pGround->m_pTileVertexArray[(i)*(nMapSizeY+1)+(j+1)].pos));
			D3DXVec3Normalize(&vNormal,&vNormal);
		}
		else if(g_pGround->IntersectTriangle(vPos,dir,g_pGround->m_pTileVertexArray[i*(nMapSizeY+1)+j+1].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(nMapSizeY+1)+(j)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(nMapSizeY+1)+j+1].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			// ������ ��ֺ��͸� ���Ѵ�.
			D3DXVec3Cross(&vNormal,&(g_pGround->m_pTileVertexArray[(i+1)*(nMapSizeY+1)+(j+1)].pos
				-g_pGround->m_pTileVertexArray[(i)*(nMapSizeY+1)+j+1].pos),
				&(g_pGround->m_pTileVertexArray[(i+1)*(nMapSizeY+1)+(j)].pos
				-g_pGround->m_pTileVertexArray[(i+1)*(nMapSizeY+1)+(j+1)].pos));
			D3DXVec3Normalize(&vNormal,&vNormal);
		}
	}
//	DBGOUT("Monster Normal Vector : %.2f, %.2f, %.2f\n", vNormal.x, vNormal.y, vNormal.z);
	return vNormal;
}


// ���� ��ų 2004.06.26 jschoi
void CMonsterData::UseSkillMove()
{
	if(m_nMonsterSkillState == MONSTER_WAIT_END_SKILL)
	{
		// ���� ��ǥ �ε�
		m_vPos = m_vOldSkillEndPos;
		m_vVel = m_vOldSkillEndVel;
		m_vUp = m_vOldSkillEndUp;
	}
	else if(m_pMonsterMovePattern)
	{
		BOOL bSkillStop = m_pMonsterMovePattern->SkillTick();	
		EVENT_POINT SkillPoint;

		SkillPoint=m_pMonsterMovePattern->GetCurrentCinemaPoint();
		m_vPos = SkillPoint.vPosition;			// ��ġ
		m_vVel = SkillPoint.vDirection;			// ����
		m_vUp = SkillPoint.vUpVector;			// ������

		// �浹 ó�� 2004.06.26 jschoi
		float fCheckHeight;
		BOOL bIsWater = g_pShuttleChild->CheckIsWater(m_vPos);
		if(bIsWater)
		{
			fCheckHeight = g_pGround->m_projectInfo.fWaterHeight + MIN_GAP_TO_GROUND;
		}
		else
		{
			fCheckHeight = g_pShuttleChild->CheckMap(m_vPos) + MIN_GAP_TO_GROUND;
		}
		if(m_vPos.y < fCheckHeight)
		{
			m_vPos.y = fCheckHeight;	
		}

		if(bSkillStop)
		{
			m_fLeanRate= 0;						// ���� �����͸� ����Ͽ� �ε巴�� ����δ�(�¿� �̵� ����Ӱ� ȸ�� ����� ����)
			m_nMonsterSkillState = MONSTER_WAIT_END_SKILL;

			// ��ų�� �������� ������ �˸���.
			MSG_FC_MONSTER_SKILL_END_SKILL sMsg;
			sMsg.MonsterIndex	= m_info.MonsterIndex;
			sMsg.PositionVector	= m_vPos;
			sMsg.TargetVector	= m_vVel*1000.0f;			// TargetVector * 1000.0f		

			// ���� ��ǥ ����
			m_vOldSkillEndPos = m_vPos;
			m_vOldSkillEndVel = m_vVel;
			m_vOldSkillEndUp = m_vUp;

			g_pFieldWinSocket->SendMsg(T_FC_MONSTER_SKILL_END_SKILL, (char*)&sMsg, sizeof(sMsg));

			// 2004-10-19 by jschoi
			util::del(m_pMonsterMovePattern);
		}
	}
}

void CMonsterData::InitMonsterMovePattern(int nMonsterMoveType)
{
//  �ó׸� ����Ʈ ����/////////////////////////////////////////////////////////////////
	char str[32];
	sprintf(str,"%08d", nMonsterMoveType);
	if(m_pMonsterMovePattern != NULL)
	{
		util::del( m_pMonsterMovePattern );
	}
	m_pMonsterMovePattern = g_pScene->LoadCinemaData(str);
	if(m_pMonsterMovePattern == NULL)
	{
		#ifdef _DEBUG
			DBGOUT( "Can't Find Pattern(Cinema) File.(%s)\n",str);
		#endif //_DEBUG_endif
		m_pMonsterMovePattern = g_pScene->LoadCinemaData(PATTERN_DEFAULT);
	}
	EVENT_POINT ep;
	ep.vPosition = D3DXVECTOR3(m_mMatrix._41,m_mMatrix._42,m_mMatrix._43);
	ep.vDirection = -D3DXVECTOR3(m_mMatrix._31,m_mMatrix._32,m_mMatrix._33);
	ep.vUpVector = D3DXVECTOR3(m_mMatrix._21,m_mMatrix._22,m_mMatrix._23);
	D3DXVec3Normalize(&ep.vDirection,&ep.vDirection);
	ep.fVelocity = 100;
	ep.fCurvature = DEFAULT_CURVATURE;	// �ʱⰪ ����
	m_pMonsterMovePattern->InitCinemaData(ep);
//	DBGOUT("��ų ��� MonsterData�� InitSkillPattern()��..\n");
//  �ʱ�ȭ ���� ///////////////////////////////////////////////////////////////////////
}

void CMonsterData::EnableObjectMonsterObject()
{
	if(g_pGround && g_pGround->m_pObjectMonster)
	{
		CObjectChild* pObj = (CObjectChild*)g_pGround->m_pObjectMonster->m_pChild;
		while(pObj)
		{
			if(	pObj->m_pObjectInfo &&
				m_pMonsterInfo &&
				pObj->m_nCode == (m_pMonsterInfo->MonsterUnitKind + DEFAULT_OBJECT_MONSTER_OBJECT) &&
				D3DXVec3Length(&(pObj->m_vPos - m_vPos)) < 10.0f )
			{
				pObj->m_bEnableObjectMonsterObject = TRUE;
				g_pScene->m_vectorRangeObjectPtrList.push_back(pObj);
				g_pScene->m_vectorCollisionObjectPtrList.push_back(pObj);
				g_pScene->m_vectorCulledObjectPtrList.push_back(pObj);
				break;
			}
			pObj = (CObjectChild*)pObj->m_pNext;
		}
	}
}

void CMonsterData::DisableObjectMonsterObject()
{
	if(g_pGround && g_pGround->m_pObjectMonster)
	{
		CObjectChild* pObj = (CObjectChild*)g_pGround->m_pObjectMonster->m_pChild;
		while(pObj)
		{
			if(	pObj->m_pObjectInfo &&
				m_pMonsterInfo &&
				pObj->m_nCode == (m_pMonsterInfo->MonsterUnitKind + DEFAULT_OBJECT_MONSTER_OBJECT) &&
				D3DXVec3Length(&(pObj->m_vPos - m_vPos)) < 10.0f )
			{
				pObj->m_bEnableObjectMonsterObject = FALSE;
				// 2006-07-05 by ispark, �״� �κ� ����
				vectorCObjectChildPtr::iterator itObj = find(g_pScene->m_vectorRangeObjectPtrList.begin(), g_pScene->m_vectorRangeObjectPtrList.end(), pObj);
				if(itObj != g_pScene->m_vectorRangeObjectPtrList.end())
				{
					g_pScene->m_vectorRangeObjectPtrList.erase(itObj);
					break;
				}
			}
			pObj = (CObjectChild*)pObj->m_pNext;
		}
	}
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CEnemyData::UpdateCheckObjectTOEnemy()
/// \brief		������Ʈ �浹 ���� ���� �Ǵ� 
/// \author		// 2007-05-17 by bhsohn ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��	
/// \date		2007-05-17 ~ 2007-05-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CMonsterData::UpdateCheckObjectTOEnemy()
{	
	// ������Ʈ �浹 ���� üũ
	D3DXVECTOR3	vMyShuttlePos	= g_pShuttleChild->m_vPos;
	D3DXVECTOR3	vEmenyPos		= m_vPos;

 	m_bCheckObject = FALSE;
	m_bCheckObjectEnemyShow = TRUE;		
	
	// �ѹ��� ���� �������� ���.
	if(g_pScene->IsObjectCheckPosTOPos(vMyShuttlePos, vEmenyPos, m_vUp))
	{
		// ������Ʈ�� �ִ�.
		m_bCheckObject = TRUE;
		m_bCheckObjectEnemyShow = FALSE;
	}
	// �������� üũ�� �Ѵ�. 
	else if(g_pScene->IsTileCheckPosTOPos(vMyShuttlePos, vEmenyPos))
	{
		// ������ �ִ�.
		m_bCheckObject = TRUE;
		m_bCheckObjectEnemyShow = FALSE;
	}
	return m_bCheckObject;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CMonsterData::IsCheckObjectColl()
/// \brief		Ÿ���� ���� ��, ������Ʈ �浹 ���� ���� �Ǵ� 
/// \author		// 2007-05-17 by bhsohn ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��	
/// \date		2007-05-17 ~ 2007-05-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CMonsterData::IsCheckObjectColl()
{
	return UpdateCheckObjectTOEnemy();		
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CMonsterData::IsCheckObjectShow()
/// \brief		������Ʈ�� �׸��� ������ �����Ѵ�.
/// \author		// 2007-05-17 by bhsohn ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��	
/// \date		2007-05-17 ~ 2007-05-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CMonsterData::IsCheckObjectShow()
{		
	return m_bCheckObjectEnemyShow;
}	

void CMonsterData::TickObjectCheck()
{
	if((g_pD3dApp->IsMyShuttleCharacter() == TRUE)		// �����̸� ������ ���δ�.
		|| (m_info.MonsterIndex == g_pShuttleChild->GetTargetClientIndext())	// ���� Ÿ�� ���������
		|| (m_nTargetIndex == g_pShuttleChild->GetMyShuttleInfo().ClientIndex)// ���� Ÿ���� ��� ������ ������ ���δ�.
		|| ( m_nTargetIndex != 0))  // 2007-06-28 by bhsohn ���� Ÿ�� ��� ���� ���� ���Ϳ� ���� Ÿ�� ó��
		
	{
		m_bCheckObjectEnemyShow = TRUE;
	}
	else if( m_pMonsterInfo && (IS_SAME_CHARACTER_MONSTER_INFLUENCE(g_pShuttleChild->GetMyShuttleInfo().InfluenceType,
													m_pMonsterInfo->Belligerence)))
	{
		// ������ ���� �����̴�. 
		m_bCheckObjectEnemyShow = TRUE;
	}
	// 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)
	else if( m_pMonsterInfo && m_pMonsterInfo->Belligerence == BELL_INFINITY_DEFENSE_MONSTER )
	{
		m_bCheckObjectEnemyShow = TRUE;
	}
	// end 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)
	else
	{
		m_bCheckObjectEnemyShow = FALSE;
	}
}

// 2010. 03. 18 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ��ų �߰�)
void	CMonsterData::InsertMonsterBuffSkill( INT nSkillNum, ITEM* pSkillItem )
{
	if( m_mapMonsterBuffSkill.find( nSkillNum ) == m_mapMonsterBuffSkill.end() )
		m_mapMonsterBuffSkill.insert( std::pair<INT, ITEM*>( nSkillNum, pSkillItem ) );
}

void	CMonsterData::RemoveMonsterBuffSkill( INT nSkillNum )
{
	if( m_mapMonsterBuffSkill.find( nSkillNum ) != m_mapMonsterBuffSkill.end() )
		m_mapMonsterBuffSkill.erase( nSkillNum );
}

ITEM*	CMonsterData::GetSkillItemWithDesParam( BYTE nDesParam )
{
	std::map<INT, ITEM*>::iterator it = m_mapMonsterBuffSkill.begin();
	while( it != m_mapMonsterBuffSkill.end() )
	{
		if( ((*it).second)->IsExistDesParam( nDesParam ) )
			return (*it).second;

		++it;
	}

	return NULL;
}
// end 2010. 03. 18 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ��ų �߰�)

// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
D3DXVECTOR3 CMonsterData::GetMultiPos(int nMultiIndex)
{
	D3DXVECTOR3 v3 = D3DXVECTOR3(0,0,0);
	if(nMultiIndex > m_vecvmultiData.size())
	{
		nMultiIndex= 0;
	}
	if(	!m_vecvmultiData.empty() && nMultiIndex >= 0)
	{

		v3.x = 	m_vecvmultiData[nMultiIndex]->TargetMatrix._41;
		v3.y = 	m_vecvmultiData[nMultiIndex]->TargetMatrix._42;
		v3.z =  m_vecvmultiData[nMultiIndex]->TargetMatrix._43;
	}
	return v3;
}

void CMonsterData::GetScreenPos(D3DXVECTOR3 vMultiPos, int &x, int &y, int& w)
{
	g_pD3dApp->CalcObjectSourceScreenCoords(vMultiPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, x, y, w );
}
// end 11. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���


// 2011-05-17 by jhahn	����3�� ���� ��ü
void CMonsterData::ChangeMonster(int ChangeMonsterUnitKind)
{
	
	if(m_pMonsterInfo)
	{
 		MEX_MONSTER_INFO * pMonsterInfo = g_pGameMain->CheckMonsterInfo( ChangeMonsterUnitKind );
		if( pMonsterInfo )
		{
			m_pMonsterInfo = pMonsterInfo;
		}
		ChangeMonsterInfo(m_pMonsterInfo->SourceIndex, m_info.BodyCondition);
	}
}

void CMonsterData::ChangeMonsterInfo(int nSourceIndex, BodyCond_t nBodyCon)
{
	m_pMonMesh = m_pRenderer->InitData( nSourceIndex, nBodyCon);
}
//end 2011-05-17 by jhahn	����3�� ���� ��ü

//2011-06-05 by jhahn	����3�� ���� ����
void CMonsterData::DeleteMonster()
{	
	m_info.BodyCondition = BODYCON_MON_AUTODESTROYED_MASK;
	if(m_pCharacterInfo)
		m_pCharacterInfo->ChangeBodyCondition(m_info.BodyCondition);
	
	m_pMonMesh = m_pRenderer->InitData( FALSE, m_info.BodyCondition);
	
}
//end 2011-06-05 by jhahn	����3�� ���� ����