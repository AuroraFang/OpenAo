// UnitData.cpp: implementation of the CUnitData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UnitData.h"
#include "StoreData.h"
#include "EnemyData.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "SceneData.h"
#include "WeaponFireData.h"
#include "WeaponFastData.h"
#include "UnitRender.h"
#include "CharacterRender.h"					// 2005-07-21 by ispark
#include "AtumSound.h"
#include "AtumDatabase.h"
#include "WeaponRocketData.h"
#include "WeaponMissileData.h"
#include "ItemInfo.h"
#include "WeaponAllAttackData.h"
#include "dxutil.h"
#include "Skill.h"
#include "SkillEffect.h"
#include "WeaponItemInfo.h"
#include "Camera.h"
#include "MonsterData.h"
#include "ItemData.h"
#include "PetManager.h"	// 2010-06-15 by shcho&hslee ��ý��� - �� �����͸� �������� �Լ�
	

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define CREATE_PRIMARY_SINGLE_WEAPON(p1, p2) \
	(p1 ? new CWeaponFastData(this,pTarget,p1) :	\
		  new CWeaponFastData(this,pTarget,p2) )
#define CREATE_PRIMARY_DOUBLE_WEAPON(p1, p2, i) \
	(p1 ? new CWeaponFastData(this,pTarget,i,p1) :	\
		  new CWeaponFastData(this,pTarget,i,p2) )
#define CREATE_PRIMARY_FIRE_WEAPON_BY_ITEM_INDEX(p1, p2) \
	(p1 ? new CWeaponFireData(this,p1->WeaponItemNumber) :	\
		  new CWeaponFireData(this,p2->WeaponItemNumber) )
#define CREATE_SECONDARY_WEAPON_ROCKET(p1, p2) \
	(p1 ? new CWeaponRocketData(this,pTarget,p1) :	\
		  new CWeaponRocketData(this,pTarget,p2) )
#define CREATE_SECONDARY_WEAPON_MISSILE(p1, p2) \
	(p1 ? new CWeaponMissileData(this,pTarget,p1) :	\
		  new CWeaponMissileData(this,pTarget,p2) )
#define MSG_PRIMARY_ATTACKINDEX(p1,p2)		((p1) ? p1->AttackIndex : p2->AttackIndex)
#define MSG_PRIMARY_WEAPONITEMNUMBER(p1,p2)		((p1) ? p1->WeaponItemNumber : p2->WeaponItemNumber)
#define MSG_SECONDARY_ATTACKINDEX(p1,p2)		((p1) ? p1->AttackIndex : p2->AttackIndex)
#define MSG_SECONDARY_WEAPONITEMNUMBER(p1,p2)		((p1) ? p1->WeaponItemNumber : p2->WeaponItemNumber)



// �ν��� �ٵ� ����� ���� ���
// BODYCON_BOOSTER1_MASK : Ű�� �ȴ����� ����(��ռӵ� ����, �ӵ�����Ű �ȴ�����)
// BODYCON_BOOSTER2_MASK : ����Ű ������ ����(��ռӵ� �̻�, �ӵ�����Ű ������)
// BODYCON_BOOSTER3_MASK : ��� �ν���
// BODYCON_BOOSTER4_MASK : ����� ���� 
// BODYCON_BOOSTER5_MASK : ������
// BODYCON_BOOSTER_OFF_MASK : �ν��� ��

#define BODYCON_BOOSTER_OFF				(BodyCond_t)0x0000000000000004// ȭ�鿡 ��Ÿ���� ����Ʈ�� �Ҹ�

// ���������� �ν��� bodycon�� �ߵ����϶� BODYCON_FLY_MASK�� ���� �ߵ��Ѵ�.
// ������, Ŭ���̾�Ʈ�� BODYCON_FLY_MASK bodycon�� ����.
// �������� BODYCON_FLY_MASK bodycon�� ���� �ν��� bodycon�� Ȯ���Ѵ�.

CUnitData::CUnitData()
{
	FLOG("CUnitData::CUnitData()");
	ChangeUnitState( _NORMAL );
//	m_bShootingPrimaryWeapon = FALSE;
//	m_fAutoMaticTimer = 0.0f;
//	m_bZigZagWeapon = FALSE;
//	m_vPrimarySidePos = D3DXVECTOR3(0,0,0);
//	m_vPrimarySideBackPos = D3DXVECTOR3(0,0,0);
//	m_vSecondarySidePos = D3DXVECTOR3(0,0,0);
	m_pVBShadow = NULL;
	m_nUnitNum = 0;
	m_nPilotNum = 0;						// 2005-07-13 by ispark	�ʱ�ȭ
	m_bCharacter = FALSE;					// 2005-07-13 by ispark

	m_dwPkState = PK_NONE;
	m_bPkAttack = FALSE;
	CUnitData::InitDeviceObjects();

	m_nAlphaValue = SKILL_OBJECT_ALPHA_NONE;// 2006-11-16 by ispark
	m_bySkillStateFlag = CL_SKILL_NONE;		// 2006-11-21 by ispark

	// 2007-04-02 by bhsohn Move��Ŷ���� ���� ���� üũ	
	m_byUnitState = 0x00;
	for(int nCnt = 0;nCnt < UNIT_STATE_MAX; nCnt++)
	{		
		m_fUnitStateChgCap[nCnt] = 0;
	}	

	// 2007-05-17 by bhsohn ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��
	m_bCheckObjectEnemyShow = TRUE;		// ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��
	m_bCheckObject = FALSE;
	n_fCheckObjectTime = 0;
	// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
	m_LoadingPriority = _NOTHING_PRIORITY;
	//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
	
	// 2010. 03. 18 by jskim ���ͺ��� ī��
	m_MonsterTransPrimarySidePos = D3DXVECTOR3(0,0,0);
	m_MonsterTransSecondarySidePos = D3DXVECTOR3(0,0,0);
	m_MonsterTransScale  = 0.0f;
	m_MonsterTransformer = 0;
	//end 2010. 03. 18 by jskim ���ͺ��� ī��
}

CUnitData::~CUnitData()
{
	FLOG("~CUnitData::~CUnitData()");
	CUnitData::DeleteDeviceObjects();
	SAFE_RELEASE(m_pVBShadow);
	m_pChaffData.clear();
}

void CUnitData::ChangeUnitState( DWORD dwState )
{
	m_dwState = dwState;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BodyCond_t CUnitData::GetCurrentBodyCondition(void)
/// \brief		������ ���� �ٵ�������� ��ȯ�Ѵ�. ������ ���۽ÿ� ����Ѵ�.
/// \author		dhkwon
/// \date		2004-03-27 ~ 2004-03-27
/// \warning	BOOSTER�ٵ�������� �������϶�, BODYCON_FLY_MASK�� �ִ´�.
///				m_pCharacterInfo == NULL�� ���(���������� ���� ���) �߻�
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BodyCond_t CUnitData::GetCurrentBodyCondition(void)
{
	FLOG("CUnitData::GetCurrentBodyCondition(void)");
	if(!m_pCharacterInfo)
		return 0;
	BodyCond_t hyBody = 0;
	hyBody = m_pCharacterInfo->m_nCurrentBodyCondition;
	if(hyBody & BODYCON_BOOSTER_EX_STATE_CLEAR_MASK)
	{
		hyBody |= BODYCON_FLY_MASK;
	}
	return hyBody;
}

void CUnitData::TurnSingleBodyCondition(BodyCond_t hySingleBodyCondition, BOOL bSet)
{
	FLOG("CUnitData::TurnSingleBodyCondition(BodyCond_t hySingleBodyCondition, BOOL bSet)");
	if(m_pCharacterInfo)
	{
		if(bSet)
		{
			m_pCharacterInfo->TurnOnSingleBodyCondition(hySingleBodyCondition);
		}
		else
		{
			m_pCharacterInfo->TurnOffSingleBodyCondition(hySingleBodyCondition);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CUnitData::ChangeUnitBodyCondition(BodyCond_t hyBodyCondition)
/// \brief		���ְ��� �ٵ�������� �����Ѵ�.
/// \author		dhkwon
/// \date		2004-04-10 ~ 2004-04-10
/// \warning	BODYCON_CLIENT_SINGLE�� ���Ե� ���� ~BODYCON_CLIENT_SINGLE�� ��츦 ������ �����Ѵ�.
///				BODYCON_CLIENT_SINGLE�� �ϳ��� ���õȴ�.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ChangeUnitBodyCondition(BodyCond_t hyBodyCondition)
{
	FLOG("CUnitData::ChangeUnitBodyCondition(BodyCond_t hyBodyCondition)");
	// BODYCON_CLIENT_SINGLE
	if(hyBodyCondition & BODYCON_CLIENT_SINGLE)
	{
		ChangeSingleBodyCondition(hyBodyCondition & BODYCON_CLIENT_SINGLE);
	}
	// ~BODYCON_CLIENT_SINGLE
//	if(hyBodyCondition & ~BODYCON_CLIENT_SINGLE)
	{
		ChangeKeepingBodyCondition(hyBodyCondition & ~BODYCON_CLIENT_SINGLE);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CUnitData::ChangeKeepingBodyCondition(BodyCond_t hyBodyCondition)
/// \brief		~BODYCON_CLIENT_SINGLE�� ��츦 ó���Ѵ�.
/// \author		dhkwon
/// \date		2004-04-10 ~ 2004-04-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ChangeKeepingBodyCondition(BodyCond_t hyBodyCondition)
{
	FLOG("CUnitData::ChangeKeepingBodyCondition(BodyCond_t hyBodyCondition)");
	if(m_pCharacterInfo)
	{
		hyBodyCondition &= ~BODYCON_CLIENT_SINGLE;
		BOOL bSet = (BODYCON_DAMAGE1_MASK & hyBodyCondition) ? TRUE : FALSE;
		TurnSingleBodyCondition(BODYCON_DAMAGE1_MASK, bSet );
		bSet = (BODYCON_DAMAGE2_MASK & hyBodyCondition) ? TRUE : FALSE;
		TurnSingleBodyCondition(BODYCON_DAMAGE2_MASK, bSet );
		bSet = (BODYCON_DAMAGE3_MASK & hyBodyCondition) ? TRUE : FALSE;
		TurnSingleBodyCondition(BODYCON_DAMAGE3_MASK, bSet );
		bSet = (BODYCON_NIGHTFLY_MASK & hyBodyCondition) ? TRUE : FALSE;
		TurnSingleBodyCondition(BODYCON_NIGHTFLY_MASK, bSet );
	}
}

void CUnitData::ChangeSingleBodyCondition( BodyCond_t hySingleBodyCondition )
{
	FLOG("CUnitData::ChangeSingleBodyCondition( BodyCond_t hySingleBodyCondition )");
	if(!m_pCharacterInfo)
		return;
	// ���� �ٵ����� BODYCON_CLIENT_SINGLE�� ���� �ٵ����� hySingleBodyCondition�� �ƴ� �ٵ����� �����.
 	hySingleBodyCondition &= ~BODYCON_FLY_MASK;// delete BODYCON_FLY_MASK
	BodyCond_t hyDeleteBodyCon = m_pCharacterInfo->m_nCurrentBodyCondition & (~hySingleBodyCondition & BODYCON_CLIENT_SINGLE);
	// 2006-01-21 by ispark, ĳ����
	if(hyDeleteBodyCon & BODYCON_CHARACTER_MODE_STOP)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_CHARACTER_MODE_STOP);
	}
	if(hyDeleteBodyCon & BODYCON_CHARACTER_MODE_WALK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_CHARACTER_MODE_WALK);
	}
	if(hyDeleteBodyCon & BODYCON_CHARACTER_MODE_RUN)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_CHARACTER_MODE_RUN);
	}
	// 2006-01-21 by ispark, ���
	if(hyDeleteBodyCon & BODYCON_EXPLOSION_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_EXPLOSION_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_TAKEOFF_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_TAKEOFF_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_SIEGE_ON_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_SIEGE_ON_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_SIEGE_OFF_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_SIEGE_OFF_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_LANDING_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_LANDING_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_LANDED_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_LANDED_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_DEAD_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_DEAD_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_NOT_USED1_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_NOT_USED1_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_BOOSTER1_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_BOOSTER1_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_BOOSTER2_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_BOOSTER2_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_BOOSTER4_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_BOOSTER4_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_BOOSTER5_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_BOOSTER5_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_BOOSTER3_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_BOOSTER3_MASK);
		if(m_dwPartType == _SHUTTLE)
		{
			CItemInfo * pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos( POS_REAR );
			//ITEM* pITEM = g_pStoreData->GetItemInfoFromItemGeneral((ITEM_GENERAL*)pItemInfo);
			if( pItemInfo )
			{
				ITEM* pITEM = pItemInfo->GetRealItemInfo();
				if(pITEM)
				{
//					g_pShuttleChild->m_fMouseRate = pITEM->RangeAngle; // ������ RangeAngle�� shuttle�� Paramfactor�� ������� �ʴ´�.
					g_pShuttleChild->m_fMouseRate = CAtumSJ::GetEngineRangeAngle(pITEM, &g_pShuttleChild->m_paramFactor); // ������ RangeAngle�� shuttle�� Paramfactor�� ������� �ʴ´�.
				}
			}
			CAppEffectData* pEffect = g_pScene->FindEffect( RC_EFF_BOOSTER );
			if(pEffect)
			{
				pEffect->ChangeBodyCondition(BODYCON_BOOSTER_OFF);
			}
		}
		// 2006-10-25 by dgwoo
		else if(m_dwPartType == _ADMIN)
		{
			CAppEffectData* pEffect = g_pScene->FindEffect( RC_EFF_BOOSTER );
			if(pEffect)
			{
				pEffect->ChangeBodyCondition(BODYCON_BOOSTER_OFF);
			}
		}
		if(m_pRadar)
		{
			// ���̴� �ٵ������ ����( ���� ���� , ���� ������ ���� ��� )
			m_pRadar->ChangeBodyCondition(RADAR_BODYCON_BOOSTER_OFF << GetShiftWeaponBodyconditionByUnitKind());
		}
	}
	if(hySingleBodyCondition & BODYCON_BOOSTER3_MASK)
	{
		if(m_pRadar)
		{
			// ���̴� �ٵ������ ����
			m_pRadar->ResetBodyCondition(RADAR_BODYCON_BOOSTER_ON << GetShiftWeaponBodyconditionByUnitKind());
		}
	}
	// Turn on bodycon
#ifdef _DEBUG
	BodyCond_t hyTemp = 1;
	BodyCond_t hyLast = 0;
	int nBodyConNumber = 0;
	while( hyTemp )
	{
		if(hyTemp & hySingleBodyCondition & BODYCON_CLIENT_SINGLE)
		{
			nBodyConNumber++;
			hyLast = hyTemp;
		}
		hyTemp <<= 1;
	}
	if(nBodyConNumber > 1)
	{
//		DBGOUT("CUnitData::ChangeSingleBodyCondition( %016I64X --> %016I64X) (nBodyConNumber > 1) So Set Last BodyCondition \n",hySingleBodyCondition,hyLast);
		hySingleBodyCondition = hyLast;
	}
#endif // _DEBUG_endif
	m_pCharacterInfo->TurnOnSingleBodyCondition(hySingleBodyCondition);
	if( hySingleBodyCondition & BODYCON_BOOSTER3_MASK || 
		hySingleBodyCondition & BODYCON_BOOSTER2_MASK ||
		hySingleBodyCondition & BODYCON_BOOSTER4_MASK ||
		hySingleBodyCondition & BODYCON_BOOSTER1_MASK)
	{
		int nUnitKind = 0;
		if(m_dwPartType == _SHUTTLE)
		{
			nUnitKind = g_pShuttleChild->m_myShuttleInfo.UnitKind;
			if(!(hySingleBodyCondition & BODYCON_BOOSTER3_MASK))
			{
				if(IS_BT(nUnitKind) || IS_ST(nUnitKind) )
				{
					g_pD3dApp->m_pSound->StopD3DSound( SOUND_HIGH_BOOSTER );
					// 2010. 07. 07 by jskim ������ ���� ���� ����
					//g_pD3dApp->m_pSound->PlayD3DSound( SOUND_FLYING_B_I_GEAR, m_vPos, FALSE);
					g_pD3dApp->m_pSound->PlayD3DSound( SOUND_FLYING_B_I_GEAR, m_vPos, TRUE);
					//end 2010. 07. 07 by jskim ������ ���� ���� ����
				}
				else
				{
					g_pD3dApp->m_pSound->StopD3DSound( SOUND_LOW_BOOSTER );
					// 2010. 07. 07 by jskim ������ ���� ���� ����
					//g_pD3dApp->m_pSound->PlayD3DSound( SOUND_FLYING_M_A_GEAR, m_vPos, FALSE);
					g_pD3dApp->m_pSound->PlayD3DSound( SOUND_FLYING_M_A_GEAR, m_vPos, TRUE);
					//end 2010. 07. 07 by jskim ������ ���� ���� ����
				}
			}
			if(hySingleBodyCondition & BODYCON_BOOSTER4_MASK)
			{
				if(IS_BT(nUnitKind) || IS_ST(nUnitKind) )
				{
					// 2010. 07. 07 by jskim ������ ���� ���� ����
					//g_pD3dApp->m_pSound->PlayD3DSound( SOUND_HIGH_BOOSTER_END, m_vPos, FALSE);
					g_pD3dApp->m_pSound->PlayD3DSound( SOUND_HIGH_BOOSTER_END, m_vPos, TRUE);
					//end 2010. 07. 07 by jskim ������ ���� ���� ����
				}
				else
				{
					// 2010. 07. 07 by jskim ������ ���� ���� ����
					//g_pD3dApp->m_pSound->PlayD3DSound( SOUND_LOW_BOOSTER_END, m_vPos, FALSE);
					g_pD3dApp->m_pSound->PlayD3DSound( SOUND_LOW_BOOSTER_END, m_vPos, TRUE);
					//end 2010. 07. 07 by jskim ������ ���� ���� ����
				}
			}
		}
		else if(m_dwPartType == _ENEMY)
		{
			nUnitKind = ((CEnemyData*)this)->m_infoCharacter.CharacterInfo.UnitKind;
		}

//		float fDistance = (g_pScene->m_fFogEndValue*g_pSOption->sUnitDetail)/MAX_OPTION_VALUE;
		float fDistance = (RANGE_OF_VISION*g_pSOption->sUnitDetail)/MAX_OPTION_VALUE;
		// 2006-01-20 by ispark, ����
//		if( this == g_pShuttleChild ||
//		    (g_pD3dApp->m_bDegree != 0 && 
//			 D3DXVec3Length(&(g_pD3dApp->m_pCamera->GetEyePt() - m_vPos)) < fDistance ))
//		{
//			// ��� ���� �ܻ� ����Ʈ
//			CAppEffectData* pGearTrace = g_pScene->FindEffect(GetGearTrace(nUnitKind), this);
//			if( !pGearTrace )
//			{
//				CAppEffectData* pData = new CAppEffectData(this,GetGearTrace(nUnitKind),D3DXVECTOR3(0,0,0));
//				g_pD3dApp->m_pEffectList->AddChild(pData);
//			}
//		}
	}
	else
	{
		int nUnitKind = 0;
		if(m_dwPartType == _SHUTTLE)
		{
			g_pD3dApp->m_pSound->StopD3DSound( SOUND_FLYING_M_A_GEAR );
			g_pD3dApp->m_pSound->StopD3DSound( SOUND_HIGH_BOOSTER );
			g_pD3dApp->m_pSound->StopD3DSound( SOUND_FLYING_B_I_GEAR );
			g_pD3dApp->m_pSound->StopD3DSound( SOUND_LOW_BOOSTER );
			nUnitKind = g_pShuttleChild->m_myShuttleInfo.UnitKind;
		}
		else if(m_dwPartType == _ENEMY)
		{
			nUnitKind = ((CEnemyData*)this)->m_infoCharacter.CharacterInfo.UnitKind;
		}
		
//		if(this == g_pShuttleChild)
//		{
//			CAppEffectData* pData = g_pScene->FindEffect(GetGearTrace(nUnitKind),this);
//			if(pData)
//			{
//				pData->m_bUsing = FALSE;
//				pData->Tick();
//			}
//		}
		CAppEffectData* pData = g_pScene->FindEffect(GetGearTrace(nUnitKind),this);
		if(pData)
		{
			pData->m_bUsing = FALSE;
			//pData->Tick();
		}
	}
	// 2010-06-08 by dgwoo, ��ý��� �߰�. 
	
	BOOL Update_BodyCon = TRUE;
	if((hySingleBodyCondition & (BODYCON_LANDING_MASK|BODYCON_LANDED_MASK|BODYCON_TAKEOFF_MASK)) &&
		IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind))
	{
		Update_BodyCon = FALSE;
	}
	if(m_pPartner && Update_BodyCon)
	{
        // 2010-06-15 by shcho&hslee ��ý��� - ���� �̵� ó��
        //m_pPartner->ChangeBodyCondition(hySingleBodyCondition);
		if( g_pD3dApp->m_bCharacter == TRUE &&								// ĳ���� �󿡼��� ���̴� �ٵ� �����
			(hySingleBodyCondition & BODYCON_CHARACTER_MODE_STOP ||
			hySingleBodyCondition & BODYCON_CHARACTER_MODE_WALK  ||
			hySingleBodyCondition & BODYCON_CHARACTER_MODE_RUN))
		{
			m_pPartner->ChangeBodyCondition(hySingleBodyCondition);
		}
		if (g_pD3dApp->m_bCharacter == FALSE &&
			hySingleBodyCondition != NULL &&
			!(hySingleBodyCondition & BODYCON_CHARACTER_MODE_STOP ||
			hySingleBodyCondition & BODYCON_CHARACTER_MODE_WALK  ||
			hySingleBodyCondition & BODYCON_CHARACTER_MODE_RUN))
		{
			m_pPartner->ChangeBodyCondition(hySingleBodyCondition);
		}
	}
	
	if(m_pPartner1 && Update_BodyCon)
	{								
        // 2010-06-15 by shcho&hslee ��ý��� - ���� �̵� ó��
        //m_pPartner->ChangeBodyCondition(hySingleBodyCondition);
		if( hySingleBodyCondition & BODYCON_BOOSTER1_MASK ||
			(g_pD3dApp->m_bCharacter == TRUE &&								// ĳ���� �󿡼��� ���̴� �ٵ� �����
			(hySingleBodyCondition & BODYCON_CHARACTER_MODE_STOP ||
			hySingleBodyCondition & BODYCON_CHARACTER_MODE_WALK ||
			hySingleBodyCondition & BODYCON_CHARACTER_MODE_RUN)))
		{
			m_pPartner1->ChangeBodyCondition(hySingleBodyCondition);
		}
		if (g_pD3dApp->m_bCharacter == FALSE &&	
			hySingleBodyCondition != NULL &&
			!(hySingleBodyCondition & BODYCON_CHARACTER_MODE_STOP ||
			hySingleBodyCondition & BODYCON_CHARACTER_MODE_WALK  ||
			hySingleBodyCondition & BODYCON_CHARACTER_MODE_RUN))
		{
			m_pPartner1->ChangeBodyCondition(hySingleBodyCondition);
		}
	}
	// 2010-06-08 by dgwoo, ��ý��� �߰�. 

	// engine bodycon
	if(hySingleBodyCondition & BODYCON_UNIT_TO_ENGINE_LANDED)
	{
		hySingleBodyCondition = BODYCON_LANDED_MASK;
	}
 	if( BODYCON_ENGINE_STATE_MASK & hySingleBodyCondition && m_pEngine)
	{
//		ASSERT_ASSERT(m_pEngine);
		m_pEngine->ResetBodyCondition( BODYCON_ENGINE_STATE_MASK & hySingleBodyCondition );
	}

	// 2006-06-28 by ispark, �ð� ���� ���� �Ǽ��縮 �ٵ������
	// 2006-08-21 by ispark, ��Ÿ �Ǽ��縮 �߰�
	if( hySingleBodyCondition & BODYCON_BOOSTER1_MASK ||
		(g_pD3dApp->m_bCharacter == TRUE &&								// ĳ���� �󿡼��� ���̴� �ٵ� �����
		(hySingleBodyCondition & BODYCON_CHARACTER_MODE_STOP ||
		hySingleBodyCondition & BODYCON_CHARACTER_MODE_WALK ||
		hySingleBodyCondition & BODYCON_CHARACTER_MODE_RUN)))
	{
		if(m_pContainer)
		{
			m_pContainer->ChangeBodyCondition(hySingleBodyCondition);
		}
		// 2006-08-18 by ispark, ��Ÿ �ٵ� �����
		if(m_pAccessories)
		{
			m_pAccessories->ChangeBodyCondition(hySingleBodyCondition);
		}
		if(m_pWingIn)
		{
			m_pWingIn->ChangeBodyCondition(hySingleBodyCondition);
		}	
	}
/*	if( this == g_pShuttleChild )
	{
		if( hySingleBodyCondition == BODYCON_BOOSTER4_MASK )
		{
			CAppEffectData* pEffect = g_pScene->FindEffect( RC_EFF_BOOSTER );
			if(pEffect)
			{
				pEffect->ChangeBodyCondition(BODYCON_BOOSTER_OFF);
			}
		}
	}*/
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ResetBodyCondition( BodyCond_t hyBody ),ChangeBodyCondition
/// \brief		������ bodycon�� �ٲٰ�, ���� �ִ� bodycon�� �ִϸ��̼��� �ٽ� �����Ѵ�.
/// \author		dhkwon
/// \date		2004-03-27 ~ 2004-03-27
/// \warning	BODYCON_CLIENT_SINGLE�� �ϳ��� ����,
///				BODYCON_FLY_MASK�� �����Ѵ�.
///				BOOSTER bodycon�� ���� �ٵ� ����ǵ� �ٲ۴�.
///				BODYCON_CLIENT_SINGLE�� �ƴ� �ٵ����� ó������ �ʴ´�.(TurnOn, TurnOff�� �̿��ؾ� �Ѵ�.)
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ResetBodyCondition( BodyCond_t hyBody )
{
	FLOG("CUnitData::ResetBodyCondition( BodyCond_t hyBody )");
	if(m_pCharacterInfo)
	{
		hyBody &= ~BODYCON_FLY_MASK;// delete BODYCON_FLY_MASK
	//	if(BODYCON_CLIENT_SINGLE & hyBody )
	//	{
	//		BodyCond_t hyTemp = m_pCharacterInfo->m_nCurrentBodyCondition;
	//		SET_CLIENT_BODYCON_BIT(hyTemp,hyBody);
	//		m_pCharacterInfo->ResetBodyCondition(hyTemp);
	//	}
		BodyCond_t hySingleBodyCon = BODYCON_CLIENT_SINGLE & m_pCharacterInfo->m_nCurrentBodyCondition;
		if( BODYCON_CLIENT_SINGLE & hyBody )
		{
			SET_CLIENT_BODYCON_BIT(hySingleBodyCon,BODYCON_CLIENT_SINGLE & hyBody);
		}
		BodyCond_t hyTemp = ~BODYCON_CLIENT_SINGLE & m_pCharacterInfo->m_nCurrentBodyCondition;
		DBGOUT("ResetBODYCONDITION 0x%016I64x\n",hySingleBodyCon | hyTemp);
		m_pCharacterInfo->ResetBodyCondition(hySingleBodyCon | hyTemp);
		if(hyBody & BODYCON_UNIT_TO_ENGINE_LANDED)
		{
			hyBody = BODYCON_LANDED_MASK;
		}
		if( (BODYCON_ENGINE_STATE_MASK & hyBody) && m_pEngine)
		{
			m_pEngine->ResetBodyCondition( BODYCON_ENGINE_STATE_MASK & hyBody );
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ChangeBodyCondition( BodyCond_t hyBody )
/// \brief		������ �ٵ�������� �ٲ۴�.
/// \author		dhkwon
/// \date		2004-03-27 ~ 2004-03-27
/// \warning	BODYCON_BOOSTER_EX_STATE_CLEAR_MASK,BODYCON_EX_STATE_CLEAR_MASK�� �ϳ��� ����,
///				BODYCON_FLY_MASK�� �����Ѵ�.
///				BOOSTER bodycon�� ���� �ٵ� ����ǵ� �ٲ۴�.
///				BODYCON_BOOSTER_EX_STATE_CLEAR_MASK,BODYCON_EX_STATE_CLEAR_MASK�� �ƴ� �ٵ����� ó������ �ʴ´�.
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ChangeBodyCondition( BodyCond_t hyBody )
{
	FLOG("CUnitData::ChangeBodyCondition( BodyCond_t hyBody )");
	if(m_pCharacterInfo)
	{
		hyBody &= ~BODYCON_FLY_MASK;// delete BODYCON_FLY_MASK
		BodyCond_t hySingleBodyCon = BODYCON_CLIENT_SINGLE & m_pCharacterInfo->m_nCurrentBodyCondition;
		if( BODYCON_CLIENT_SINGLE & hyBody )
		{
			SET_CLIENT_BODYCON_BIT(hySingleBodyCon,BODYCON_CLIENT_SINGLE & hyBody);
		}
		// 2008-04-15 by bhsohn �ٵ� ����� �� �ȵǴ� ���� �ذ�
		//BodyCond_t hyTemp = ~BODYCON_CLIENT_SINGLE & m_pCharacterInfo->m_nCurrentBodyCondition;		
		// ���� �ٵ� ������� �ƴ� ���ο� �ٵ� ��������� �������־���Ѵ�.
		BodyCond_t hyTemp = ~BODYCON_CLIENT_SINGLE & hyBody;		
		//hyTemp |= (~BODYCON_CLIENT_SINGLE & m_pCharacterInfo->m_nCurrentBodyCondition);
		// end 2008-04-15 by bhsohn �ٵ� ����� �� �ȵǴ� ���� �ذ�
		
		//DBGOUT("ChangeBODYCONDITION 0x%016I64x\n",hySingleBodyCon | hyTemp);

		m_pCharacterInfo->ChangeBodyCondition(hySingleBodyCon | hyTemp);
		if(hyBody & BODYCON_UNIT_TO_ENGINE_LANDED)
		{
			hyBody = BODYCON_LANDED_MASK;
		}
		if( (BODYCON_ENGINE_STATE_MASK & hyBody) && m_pEngine )
		{
			m_pEngine->ChangeBodyCondition( BODYCON_ENGINE_STATE_MASK & hyBody );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::SetFlyBodyCondition( BodyCond_t hyBody )
/// \brief		���� ��Ʈ�ѽÿ� ���� �ٵ�������� ������ �����Ѵ�.
/// \author		dhkwon
/// \date		2004-03-27 ~ 2004-03-27
/// \warning	Booster bodycon�� ���� �ȴ�. ���� �ٸ�(BOOSTER�� �ƴ�) �ٵ�������� �����ȴ�.
///				���ڴ� Booster bodycon�� �ϳ��� ����.
/// \param		hyBody : BODYCON_BOOSTERx_MASK(x:1,2,3,4,5) �� �ϳ��� ����.
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::SetFlyBodyCondition( BodyCond_t hyBody )
{
	FLOG("CUnitData::SetFlyBodyCondition( BodyCond_t hyBody )");
	ASSERT_IMPLIES(m_pCharacterInfo, ((BODYCON_BOOSTER_EX_STATE_CLEAR_MASK & hyBody) | !hyBody) );
	BodyCond_t hyTemp = m_pCharacterInfo->m_nCurrentBodyCondition;
	if(hyTemp & hyBody)
	{
		DBGOUT("Already SetBodyCondition (this:0x%08x, bodycon:%016I64x)\n", this, hyBody);//���� �ٵ������ ����
		return;
	}

#ifdef _DEBUG
	if(hyBody & BODYCON_FLY_MASK)
	{
		DBGOUT("Don't Set BODYCON_FLY_MASK in CUnitData::SetFlyBodyCondition \n");//CUnitData::SetFlyBodyCondition������ BODYCON_FLY_MASK�� ���ؼ��� �ȵȴ�.\n
		hyBody |= ~BODYCON_FLY_MASK;
	}
#else
	hyBody |= ~BODYCON_FLY_MASK;
#endif // _DEBUG_endif

	CLEAR_BODYCON_BIT(hyTemp, BODYCON_BOOSTER_EX_STATE_CLEAR_MASK);
	m_pCharacterInfo->ResetBodyCondition(hyTemp | hyBody);// Reset�� �Ѵ�.

	if(m_pEngine)
	{
		m_pEngine->ChangeBodyCondition( hyBody );
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::SetExBodyCondition( BodyCond_t hyBody )
/// \brief		������ �Ϲ� �ٵ�������� ���Ѵ�.(BodyCond_t)
///				(~BODYCON_FLY_MASK | BODYCON_LANDING_MASK | BODYCON_LANDED_MASK | BODYCON_DEAD_MASK | BODYCON_NOT_USED1_MASK)
/// \author		dhkwon
/// \date		2004-03-28 ~ 2004-03-28
/// \warning	BODYCON_FLY_MASK�� �����Ѵ�.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::SetExBodyCondition( BodyCond_t hyBody )
{
	FLOG("CUnitData::SetStateBodyCondition( BodyCond_t hyBody )");
	ASSERT_IMPLIES(m_pCharacterInfo, ((BODYCON_EX_STATE_CLEAR_MASK & hyBody) | !hyBody) );
	BodyCond_t hyTemp = m_pCharacterInfo->m_nCurrentBodyCondition;
	if(hyTemp & hyBody)
	{
		DBGOUT("Already SetBodyCondition (this:0x%08x, bodycon:%016I64x)\n", this, hyBody);
		return;
	}
	CLEAR_BODYCON_BIT(hyTemp, BODYCON_EX_STATE_CLEAR_MASK);
#ifdef _DEBUG
	if(hyBody & BODYCON_FLY_MASK)
	{
		//CUnitData::SetExBodyCondition������ BODYCON_FLY_MASK�� ���ؼ��� �ȵȴ�.���� �ν��͹ٵ������:0x%016I64d\n
		DBGOUT("Don't Set BODYCON_FLY_MASK in CUnitData::SetExBodyCondition. Current Booster Bodycondition:0x%016I64d\n", 
			m_pCharacterInfo->m_nCurrentBodyCondition & BODYCON_BOOSTER_EX_STATE_CLEAR_MASK);
		hyBody |= ~BODYCON_FLY_MASK;
	}
#endif // _DEBUG_endif
	//m_pCharacterInfo->m_nCurrentBodyCondition |= hyBody;
	m_pCharacterInfo->ResetBodyCondition(hyTemp | hyBody);// Reset�� �Ѵ�.
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			float CUnitData::GetCurrentAnimationTime(void)
/// \brief		������ ���� �ִϸ��̼� Ÿ��
/// \author		dhkwon
/// \date		2004-03-26 ~ 2004-03-26
/// \warning	m_pCharacterInfo->SetCharacterAnimationBodyConditionMask(BODYCON_CHARACTER_ANIMATION_TIME);
///				���� �Լ��� �����ʱ�ȭ�ÿ� ������� ��� �Ѵ�.
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CUnitData::GetCurrentAnimationTime(void)
{
	FLOG("CUnitData::GetCurrentAnimationTime(void)");
#ifdef _DEBUG
/*	if(this == g_pShuttleChild)
	{
		wsprintf( g_pD3dApp->m_strDebug, "AniBodyCon[0x%08X, %016I64X] CurrentBodyCon[%016I64X]", 
			m_pCharacterInfo->m_pCharacterAnimationBodyCondition,
			m_pCharacterInfo->m_pCharacterAnimationBodyCondition ? m_pCharacterInfo->m_pCharacterAnimationBodyCondition->m_nBodyCondition : 0, 
			m_pCharacterInfo->m_nCurrentBodyCondition );
	}*/
#endif // _DEBUG_endif
	return m_pCharacterInfo ? m_pCharacterInfo->GetCurrentCharacterAnimationTime() : 0;
}

float CUnitData::GetCurrentBodyConditionEndAnimationTime(void)
{
	FLOG("CUnitData::GetCurrentAnimationTime(void)");
	return m_pCharacterInfo ? m_pCharacterInfo->GetCurrentBodyConditionEndAnimationTime() : 0;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::CreatePrimaryWeaponItem(ITEM* pItem) 
/// \brief		Primary weapon�� �����Ѵ�.(1-1, 1-2)
/// \author		dhkwon
/// \date		2004-03-27 ~ 2004-03-27
/// \warning	�������� ���� ��� ��Ʈ�� �ε��Ѵ�. ���� �߻� ��ǥ�� �����´�.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////

// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
//void CUnitData::CreatePrimaryWeaponItem(ITEM* pItem)
void CUnitData::CreatePrimaryWeaponItem( ITEM* pItem, ITEM* pShapeItem )
// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

{
	FLOG("CUnitData::CreatePrimaryWeaponItem(ITEM* pItem) ");
	ITEM* pNULL = NULL;

	int type = 0;
	if(m_dwPartType == _SHUTTLE)
		type = GetUnitTypeDecimal(((CShuttleChild *)this)->m_myShuttleInfo.UnitKind);
	else if(m_dwPartType == _ENEMY)// || pNode->m_dwPartType == _PARTY)
		type = GetUnitTypeDecimal(((CEnemyData *)this)->m_infoCharacter.CharacterInfo.UnitKind);

	if(NULL == pItem)
	{
		DeleteWearItem( WEAR_ITEM_KIND_WEAPON_11 );
		DeleteWearItem( WEAR_ITEM_KIND_WEAPON_12 );
//		CreateWearItem( WEAR_ITEM_KIND_WEAPON_11, EFFECT_NUMBER_WEAR_WEAPON_11(pNULL, type) );
//		CreateWearItem( WEAR_ITEM_KIND_WEAPON_12, EFFECT_NUMBER_WEAR_WEAPON_12(pNULL, type) );
	}
	else if(IS_PRIMARY_WEAPON_1(pItem->Kind))
	{
		// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
		//CreateWearItem( WEAR_ITEM_KIND_WEAPON_11, EFFECT_NUMBER_WEAR_WEAPON_11(pItem, type) );

		if( pShapeItem )
			CreateWearItem( WEAR_ITEM_KIND_WEAPON_11, EFFECT_NUMBER_WEAR_WEAPON_11( pShapeItem, type ) );
		else
			CreateWearItem( WEAR_ITEM_KIND_WEAPON_11, EFFECT_NUMBER_WEAR_WEAPON_11( pItem, type ) );
		// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

		DeleteWearItem( WEAR_ITEM_KIND_WEAPON_12 );
//		CreateWearItem( WEAR_ITEM_KIND_WEAPON_12, EFFECT_NUMBER_WEAR_WEAPON_12(pNULL, type) );
		if( m_pWeapon1_1_1 && m_pWeapon1_1_1->m_pCharacterInfo)
		{
			// 2006-01-12 by ispark, ����
// 			m_PrimaryAttack.vSidePos = m_pWeapon1_1_1->m_pCharacterInfo->GetEffectPos( 
// 				WEAPON_BODYCON_FIRE_1 << GetShiftWeaponBodyconditionByUnitKind(), RC_EFF_DUMMY_POSITION );
 			m_PrimaryAttack.vSideBackPos = m_pWeapon1_1_1->m_pCharacterInfo->GetEffectPos( 
 				WEAPON_BODYCON_FIRE_1 << GetShiftWeaponBodyconditionByUnitKind(), RC_EFF_DUMMY_BACK_POSITION );
			// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����
			//SetWeaponPosition();
			SetPrimaryWeaponPosition();
			// end 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����
			
			// 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
			if( IsRobotArmor() )
			{
				// �κ��Ƹ��� ��� ���� ����Ʈ�� ���߰�
				ShowWeapon( FALSE );

				// A����� ���
				if( type == 2 )
				{
					// A���� �ӽ� ������Ʈ�� ����
					ITEM* pAGearWeapon = g_pDatabase->GetServerItemInfo( A_GEAR_ROBOT_PRIMARY_WEAPON_ITEM_NUM );
					if( pAGearWeapon )
					{
						CreateRobotAGearWeapon( pAGearWeapon, type );
					}
				}
			}
			// 2010. 03. 18 by jskim ���ͺ��� ī��
			else if(m_MonsterTransformer)
			{
				ShowWeapon( FALSE );
			}
			//end 2010. 03. 18 by jskim ���ͺ��� ī��
			// end 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
		}
	}
	else if(IS_PRIMARY_WEAPON_2(pItem->Kind))
	{
		DeleteWearItem( WEAR_ITEM_KIND_WEAPON_11 );
//		CreateWearItem( WEAR_ITEM_KIND_WEAPON_11, EFFECT_NUMBER_WEAR_WEAPON_11(pNULL, type) );
		CreateWearItem( WEAR_ITEM_KIND_WEAPON_12, EFFECT_NUMBER_WEAR_WEAPON_12(pItem, type) );
		if( m_pWeapon1_2 && m_pWeapon1_2->m_pCharacterInfo)
		{
			m_PrimaryAttack.vSidePos = m_pWeapon1_2->m_pCharacterInfo->GetEffectPos( 
				WEAPON_BODYCON_FIRE_1 << GetShiftWeaponBodyconditionByUnitKind(), RC_EFF_DUMMY_POSITION );
			m_PrimaryAttack.vSideBackPos = m_pWeapon1_2->m_pCharacterInfo->GetEffectPos( 
				WEAPON_BODYCON_FIRE_1 << GetShiftWeaponBodyconditionByUnitKind(), RC_EFF_DUMMY_BACK_POSITION );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::CreateSecondaryWeaponItem(ITEM* pItem) 
/// \brief		Secondary weapon�� �����Ѵ�.(2-1, 2-2)
/// \author		dhkwon
/// \date		2004-03-27 ~ 2004-03-27
/// \warning	�������� ���� ��� ��Ʈ�� �ε��Ѵ�. ���� �߻� ��ǥ�� �����´�.(���� ���� ����)
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////

// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
//void CUnitData::CreateSecondaryWeaponItem(ITEM* pItem)
void CUnitData::CreateSecondaryWeaponItem( ITEM* pItem, ITEM* pShapeItem )
// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

{
	FLOG("CUnitData::CreateSecondaryWeaponItem(ITEM* pItem)");
	ITEM* pNULL = NULL;

	int type = 0;
	if(m_dwPartType == _SHUTTLE)
		type = GGetUnitKindHexToDeimal(((CShuttleChild *)this)->m_myShuttleInfo.UnitKind);
//		type = GetUnitTypeDecimal(((CShuttleChild *)this)->m_myShuttleInfo.UnitKind);
	else if(m_dwPartType == _ENEMY)// || pNode->m_dwPartType == _PARTY)
		type = GGetUnitKindHexToDeimal(((CEnemyData *)this)->m_infoCharacter.CharacterInfo.UnitKind);
//		type = GetUnitTypeDecimal(((CEnemyData *)this)->m_infoCharacter.CharacterInfo.UnitKind);

	if(NULL == pItem)
	{
//		CreateWearItem( WEAR_ITEM_KIND_WEAPON_21, EFFECT_NUMBER_WEAR_WEAPON_21(pNULL, type) );
//		CreateWearItem( WEAR_ITEM_KIND_WEAPON_22, EFFECT_NUMBER_WEAR_WEAPON_22(pNULL, type) );
		DeleteWearItem( WEAR_ITEM_KIND_WEAPON_21 );
		DeleteWearItem( WEAR_ITEM_KIND_WEAPON_22 );
	}
	else if(IS_SECONDARY_WEAPON_1(pItem->Kind))
	{
		
		// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
		//CreateWearItem( WEAR_ITEM_KIND_WEAPON_21, EFFECT_NUMBER_WEAR_WEAPON_21(pItem, type) );
		if( pShapeItem )
			CreateWearItem( WEAR_ITEM_KIND_WEAPON_21, EFFECT_NUMBER_WEAR_WEAPON_21( pShapeItem, type) );
		else
			CreateWearItem( WEAR_ITEM_KIND_WEAPON_21, EFFECT_NUMBER_WEAR_WEAPON_21( pItem, type) );
		// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

//		CreateWearItem( WEAR_ITEM_KIND_WEAPON_22, EFFECT_NUMBER_WEAR_WEAPON_22(pNULL, type) );
		DeleteWearItem( WEAR_ITEM_KIND_WEAPON_22 );
		// 2006-01-12 by ispark, ����
//		if( m_pWeapon2_1_1 && m_pWeapon2_1_1->m_pCharacterInfo)
//		{
//			m_SecondaryAttack.vSidePos = m_pWeapon2_1_1->m_pCharacterInfo->GetEffectPos( WEAPON_BODYCON_FIRE_1 << GetShiftWeaponBodyconditionByUnitKind(), RC_EFF_DUMMY_POSITION );
//		}
		// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����
		//SetWeaponPosition();
		SetSecondaryWeaponPosition();
		// end 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����

		// 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
		// 2010. 03. 18 by jskim ���ͺ��� ī��
// 		if( IsRobotArmor() )
// 		{
// 			// �κ��Ƹ��� ��� 2�� ����� �׻� ������ ����
// 			ShowWeapon( FALSE );
// 		}
		if( IsRobotArmor() || ((CShuttleChild *)this)->GetMonsterTransformer())
		{
			// �κ��Ƹ��� ��� 2�� ����� �׻� ������ ����
			ShowWeapon( FALSE );
		}
		//end 2010. 03. 18 by jskim ���ͺ��� ī��
		// end 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
	}
	else if(IS_SECONDARY_WEAPON_2(pItem->Kind))
	{
//		CreateWearItem( WEAR_ITEM_KIND_WEAPON_21, EFFECT_NUMBER_WEAR_WEAPON_21(pNULL, type) );
		DeleteWearItem( WEAR_ITEM_KIND_WEAPON_21 );
		CreateWearItem( WEAR_ITEM_KIND_WEAPON_22, EFFECT_NUMBER_WEAR_WEAPON_22(pItem, type) );
		if( m_pWeapon2_2 && m_pWeapon2_2->m_pCharacterInfo)
		{
			m_SecondaryAttack.vSidePos = m_pWeapon2_2->m_pCharacterInfo->GetEffectPos( WEAPON_BODYCON_FIRE_1 << GetShiftWeaponBodyconditionByUnitKind(), RC_EFF_DUMMY_POSITION );
		}

		// ������� ��� ������̸� TOGGLE_WEAPON_BODYCON_USING, ������� �ƴϸ� TOGGLE_WEAPON_BODYCON_NORMAL
		if(m_dwPartType == _SHUTTLE)
		{
			if( pItem->Kind == ITEMKIND_SHIELD || pItem->Kind == ITEMKIND_DECOY ) // ����� 2-2�� ��������� )
			{
				if(g_pShuttleChild->m_pSecondaryWeapon->IsUsingToggleWeapon() == TRUE)
				{ // ��� ���� �����
					ResetSecondaryWeaponBodyCondition(TOGGLE_WEAPON_BODYCON_USING);
					ResetSecondaryWeaponBodyCondition(TOGGLE_WEAPON_BODYCON_USING, FALSE);
				}
				else
				{ // ��� ���� ����߾ƴ�
					ResetSecondaryWeaponBodyCondition(TOGGLE_WEAPON_BODYCON_NORMAL);
					ResetSecondaryWeaponBodyCondition(TOGGLE_WEAPON_BODYCON_NORMAL, FALSE);
				}
			}
		}
		else if( m_dwPartType == _ENEMY )
		{
			ResetSecondaryWeaponBodyCondition(TOGGLE_WEAPON_BODYCON_NORMAL);
			ResetSecondaryWeaponBodyCondition(TOGGLE_WEAPON_BODYCON_NORMAL, FALSE);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::CreateWearItem( int nWearItemKind, int nEffectNum, BOOL bCharacter /*= FALSE*/ )
/// \brief		������ �� �κк��� �����Ѵ�., ĳ���� ������ �ʿ��Ѱ��� ������ 3��° ���ڸ� �ִ´�..
///				�⺻���� ��� ����(FALSE)�̴�.
///				���� ���� ����ڴ�(WEAR_ITEM_KIND_WINGIN, WEAR_ITEM_KIND_ACCESSORIES, WEAR_ITEM_KIND_ATTACHMENT)
/// \author		dhkwon
/// \date		2004-03-23 ~ 2004-03-23
/// \warning	nWearItemKind : ������ ����, 
///				���Ŀ��� CreateWearItemEffect�Լ��� �����Ѵ�.(m_pWearEffect[] ->�迭 �������� ������)
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::CreateWearItem( int nWearItemKind, int nEffectNum, BOOL bCharacter /*= FALSE*/ )
{
	FLOG("CUnitData::CreateWearItem( int nWearItemKind, int nEffectNum )");
	// 2005-10-21 by ispark
//	if(m_dwPartType == _ADMIN)
//		return;

	switch( nWearItemKind )
	{
	case WEAR_ITEM_KIND_WEAPON_11:
		{
//			DBGOUT("1�� ������ ����(KIND:%d)(NUM:%d)\n", nWearItemKind, nEffectNum);
			// 2006-01-02 by ispark, ���� ���
			m_pWeapon1_1_1 = CreateWearItemEffect( nWearItemKind, 
												 nEffectNum, 
												 D3DXVECTOR3(0,0,0), 
												 WEAPON_BODYCON_LOADING,
												 WEAPON_POSITION_1_1_1 + ENEMY_WEAPON_INDEX_DUMMY);	  // 2012-09-20 by jhahn ������ �̸����� ���� ���� - ��ũ, ��Ʈ�� ������		
			m_pWeapon1_1_2 = CreateWearItemEffect( nWearItemKind, 
												 nEffectNum + 1, 
												 D3DXVECTOR3(0,0,0), 
												 WEAPON_BODYCON_LOADING,
												 WEAPON_POSITION_1_1_2 +  ENEMY_WEAPON_INDEX_DUMMY);  // 2012-09-20 by jhahn ������ �̸����� ���� ���� - ��ũ, ��Ʈ�� ������		
		}
		break;
	case WEAR_ITEM_KIND_WEAPON_12:
		{
			m_pWeapon1_2 = CreateWearItemEffect( nWearItemKind, 
												 nEffectNum, 
												 D3DXVECTOR3(0,0,0), 
												 WEAPON_BODYCON_LOADING );
		}
		break;
	case WEAR_ITEM_KIND_WEAPON_21:
		{
			// 2006-01-02 by ispark, ���� ���
			m_pWeapon2_1_1 = CreateWearItemEffect( nWearItemKind, 
												 nEffectNum, 
												 D3DXVECTOR3(0,0,0), 
												 WEAPON_BODYCON_LOADING,
												 WEAPON_POSITION_2_1_1 +ENEMY_WEAPON_INDEX_DUMMY);	 // 2012-09-20 by jhahn ������ �̸����� ���� ���� - ��ũ, ��Ʈ�� ������		
			m_pWeapon2_1_2 = CreateWearItemEffect( nWearItemKind, 
												 nEffectNum + 1, 
												 D3DXVECTOR3(0,0,0), 
												 WEAPON_BODYCON_LOADING,
												 WEAPON_POSITION_2_1_2 +ENEMY_WEAPON_INDEX_DUMMY);	 // 2012-09-20 by jhahn ������ �̸����� ���� ���� - ��ũ, ��Ʈ�� ������		
		}
		break;
	case WEAR_ITEM_KIND_WEAPON_22:
		{
			m_pWeapon2_2 = CreateWearItemEffect( nWearItemKind, 
												 nEffectNum, 
												 D3DXVECTOR3(0,0,0),
												 WEAPON_BODYCON_LOADING );
		}
		break;
	case WEAR_ITEM_KIND_PROW:
		{
			if(m_dwPartType == _ADMIN)
				return;

			m_pRadar = CreateWearItemEffect( nWearItemKind, 
											 nEffectNum, 
											 D3DXVECTOR3(0,0,0),
											 WEAR_ITEM_BODYCON_NORMAL );
		}
		break;
	case WEAR_ITEM_KIND_CENTER:
		{
//			m_pArmor = CreateWearItemEffect( nWearItemKind, 
//											 nEffectNum, 
//											 D3DXVECTOR3(0,0,0), 
//											 WEAR_ITEM_BODYCON_NORMAL );
		}
		break;
	case WEAR_ITEM_KIND_ACCESSORY_UNLIMITED:
		{
			m_pContainer = CreateWearItemEffect( nWearItemKind, 
												 nEffectNum, 
												 D3DXVECTOR3(0,0,0),
												 WEAR_ITEM_BODYCON_NORMAL);
			if(bCharacter)
			{
				if(m_pContainer)
				{
					m_pContainer->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
				}
			}
			else
			{
				if(m_pContainer)
				{
					m_pContainer->ChangeBodyCondition(BODYCON_BOOSTER1_MASK);
				}
			}
		}
		break;
	case WEAR_ITEM_KIND_ENGINE:
		{
			m_pEngine = CreateWearItemEffect( nWearItemKind,
											  nEffectNum, 
											  D3DXVECTOR3(0,0,0), 
											  WEAR_ITEM_BODYCON_NORMAL,
											  ENGINE_POSITION );
		}
		break;
	case WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT:
		{				
			m_pAccessories = CreateWearItemEffect( nWearItemKind,
											  nEffectNum, 
											  D3DXVECTOR3(0,0,0), 
											  WEAR_ITEM_BODYCON_NORMAL);

			if(bCharacter)
			{
				if(m_pAccessories)
				{
					m_pAccessories->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
				}
			}
			else
			{
				if(m_pAccessories)
				{
					m_pAccessories->ChangeBodyCondition(BODYCON_BOOSTER1_MASK);
				}
			}
		}
		break;
	case WEAR_ITEM_KIND_WINGIN:
		{
			m_pWingIn = CreateWearItemEffect( nWearItemKind, 
												 nEffectNum, 
												 D3DXVECTOR3(0,0,0),
												 WEAR_ITEM_BODYCON_NORMAL);
			if(bCharacter)
			{
				if(m_pWingIn)
				{
					m_pWingIn->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
				}
			}
			else
			{
				if(m_pWingIn)
				{
					m_pWingIn->ChangeBodyCondition(BODYCON_BOOSTER1_MASK);
				}
			}
		}
		break;

	// 2010-06-08 by dgwoo, ��ý��� �߰�. 
	case WEAR_ITEM_KIND_PET:
		{
			if(bCharacter)
			{				
				m_pPartner = CreateWearItemEffect( nWearItemKind,
													nEffectNum, 
													D3DXVECTOR3(0,0,0), 
 													WEAR_ITEM_BODYCON_NORMAL);
 				if ( m_pPartner )
 					m_pPartner->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
			}
			else
			{
				m_pPartner = CreateWearItemEffect( nWearItemKind,
													nEffectNum, 
													D3DXVECTOR3(0,0,0), 
 													WEAR_ITEM_BODYCON_NORMAL);
				
				m_pPartner1 = CreateWearItemEffect( nWearItemKind,
													nEffectNum, 
													D3DXVECTOR3(0,0,0), 
 													WEAR_ITEM_BODYCON_NORMAL);

				BodyCond_t Update_BodyCond = 0;
				if((g_pShuttleChild->GetCurrentBodyCondition() & (BODYCON_LANDING_MASK|BODYCON_LANDED_MASK)) &&
					!IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind))
				{
					Update_BodyCond = BODYCON_LANDED_MASK;
				}
				else
				{
					Update_BodyCond = BODYCON_BOOSTER1_MASK;
				}

				if ( m_pPartner )
					m_pPartner->ChangeBodyCondition( Update_BodyCond );
	
				if ( m_pPartner1 )
 					m_pPartner1->ChangeBodyCondition( Update_BodyCond );

				CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_PET ); 
				if( pAllItem == NULL )
				{
					return;
				}

				tPET_CURRENTINFO * tempCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData( pAllItem->UniqueNumber );
				if( tempCurrentInfo == NULL )
				{
					return;
				}	
				tPET_LEVEL_DATA* tempLevelData = g_pDatabase->GetPtr_PetLevelData( tempCurrentInfo->PetIndex,tempCurrentInfo->PetLevel );
				if( tempLevelData == NULL )
				{
					return;
				}

				//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
				ITEM * pITEM = g_pDatabase->GetServerItemInfo( tempLevelData->UseWeaponIndex );	
				int nEffectNum2 = EFFECT_NUMBER_WEAR_WEAPON_11( pITEM, GetUnitTypeDecimal(((CShuttleChild *)this)->m_myShuttleInfo.UnitKind) );

				m_pDummyPartner = CreateWearItemEffect( nWearItemKind,
														nEffectNum2, 					   // 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
														D3DXVECTOR3(0,0,0), 
 														WEAR_ITEM_BODYCON_NORMAL);

				m_pDummyPartner1 = CreateWearItemEffect( nWearItemKind,
														nEffectNum2, 					   // 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
														D3DXVECTOR3(0,0,0), 
														WEAR_ITEM_BODYCON_NORMAL);
			}
				//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
		}
		break;

	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CAttEffectData * CUnitData::CreateWearItemEffect( int nWearItemKind, int nEffectNum, D3DXVECTOR3 vPos, int nBodyCondition, CAtumNode* pNode )
/// \brief		������ �������� ����
/// \author		dhkwon
/// \date		2004-03-23 ~ 2004-03-23
/// \warning	pEffect�� Effect������ ���� ���(m_pCharacterInfo == NULL)�̸� �ٷ� �����ȴ�.
///				Static �Լ��� Create,Select �� �ٸ� ��ġ������ ���� �����ϴ�.
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CAppEffectData * CUnitData::CreateWearItemEffect( int nWearItemKind, 
												  int nEffectNum, 
												  D3DXVECTOR3 vPos, 
												  int nBodyCondition,
												  int nWeaponPositionIndex )
{
	FLOG("CUnitData::CreateWearItemEffect( int nWearItemKind, ... )");
	DBGOUT_EFFECT("		CreateWearItemEffect [nWearItemKind:%d, nEffectNum:%d, bodycon:%I64X]\n", 
		nWearItemKind, nEffectNum, nBodyCondition );
	// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
	int temp = _NOTHING_PRIORITY;
	if(g_pShuttleChild->GetUnitNum() == m_nUnitNum && 
		// 2010-06-15 by shcho&hslee ��ý��� - ���� ������ Kind�� ó�� ����.
		(nWearItemKind >= 0 && nWearItemKind < 11 ))
		//(nWearItemKind >= 0 && nWearItemKind < 10))
	{
		temp = _MY_CHARACTER_PRIORITY;
	}
	//CAppEffectData * pEffect = new CAppEffectData(this,nEffectNum,vPos, nWeaponPositionIndex);
	CAppEffectData * pEffect = new CAppEffectData(this,nEffectNum,vPos, nWeaponPositionIndex, temp );
	//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
	pEffect = (CAppEffectData*)g_pD3dApp->m_pEffectList->AddChild(pEffect);
	if(pEffect)
	{
		ResetWearItemBodyCondition( nWearItemKind, pEffect, nBodyCondition );
	}
	return pEffect;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CAppEffectData * CreateWearItemEffectAtSelectMenu( GUIUnitRenderInfo *pInfo, int nEffectNum, int nBodyCondition )
/// \brief		select, create menu���� ������ ������ ���� ����ϴ� �Լ�
/// \author		dhkwon 
/// \date		2004-03-24 ~ 2004-03-24
/// \warning	static �Լ�
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CAppEffectData * CUnitData::CreateWearItemEffectAtSelectMenu( GUIUnitRenderInfo *pInfo, 
												   int nEffectNum,
												   BodyCond_t nBodyCondition )
{
	FLOG("CUnitData::CreateWearItemEffect( GUIUnitRenderInfo *pInfo, ... )");
	CAppEffectData * pEffect = new CAppEffectData(pInfo,nEffectNum);
	if(pEffect->m_pCharacterInfo)
		pEffect->m_pCharacterInfo->ChangeBodyCondition(nBodyCondition);
	pEffect = (CAppEffectData*)g_pD3dApp->m_pEffectList->AddChild(pEffect);
#ifdef DBGOUT_EFFECT
	if(!pEffect)
	{
		DBGOUT_EFFECT("		==>Delete (Non Existent BodyCondition)[Effect Number:%d, bodycon:%I64X]\n", nEffectNum, nBodyCondition );//	==>���� (�ٵ�����Ǿ���)[����Ʈ��ȣ:%d, bodycon:%I64X]\n
	}
#endif
	return pEffect;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ResetWearItemBodyCondition( CAttEffectData * pEffect, BodyCond_t hyBodyCon)
/// \brief		������ �������� �ٵ������ ����. ���ְ� ���� ����.
/// \author		dhkwon
/// \date		2004-03-23 ~ 2004-03-23
/// \warning	������ ���ܵȴ�.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ResetWearItemBodyCondition( int nWearItemKind, CAppEffectData * pEffect, BodyCond_t hyBodyCon)
{
	FLOG("CUnitData::ResetWearItemBodyCondition( int nWearItemKind, CAppEffectData * pEffect, BodyCond_t hyBodyCon)");
	if(pEffect->m_pCharacterInfo)
	{
		int nShiftValue = GET_SHIFT_BODYCON_OF_UNITKIND_EXCEPT_ENGINE;

		pEffect->m_bodyCondition = hyBodyCon << nShiftValue;//GET_SHIFT_BODYCON_OF_UNITKIND_EXCEPT_ENGINE;
		pEffect->m_pCharacterInfo->ResetBodyCondition(hyBodyCon <<  nShiftValue);//GET_SHIFT_BODYCON_OF_UNITKIND_EXCEPT_ENGINE);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::DeleteWearItem( int nWearItemKind )
/// \brief		�ش� ��ġ�� �������� �����.
/// \author		dhkwon
/// \date		2004-03-23 ~ 2004-03-23
/// \warning	�Ŀ� CreateWearItem�� ���� �ƶ����� DeleteWearItemEffect�� ���ľ� �Ѵ�.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::DeleteWearItem( int nWearItemKind )
{
	FLOG("CUnitData::DeleteWearItem( int nWearItemKind )");
	switch( nWearItemKind )
	{
	case WEAR_ITEM_KIND_WEAPON_11:
		{
			DeleteWearItemEffect( m_pWeapon1_1_1 );
			m_pWeapon1_1_1 = NULL;
			DeleteWearItemEffect( m_pWeapon1_1_2 );
			m_pWeapon1_1_2 = NULL;
		}
		break;
	case WEAR_ITEM_KIND_WEAPON_12:
		{
			DeleteWearItemEffect( m_pWeapon1_2 );
			m_pWeapon1_2 = NULL;
		}
		break;
	case WEAR_ITEM_KIND_WEAPON_21:
		{
			DeleteWearItemEffect( m_pWeapon2_1_1 );
			m_pWeapon2_1_1 = NULL;
			DeleteWearItemEffect( m_pWeapon2_1_2 );
			m_pWeapon2_1_2 = NULL;
		}
		break;
	case WEAR_ITEM_KIND_WEAPON_22:
		{
			DeleteWearItemEffect( m_pWeapon2_2 );
			m_pWeapon2_2 = NULL;
		}
		break;
	case WEAR_ITEM_KIND_PROW:
		{
			DeleteWearItemEffect( m_pRadar );
			m_pRadar = NULL;
		}
		break;
	case WEAR_ITEM_KIND_CENTER:
		{
//			DeleteWearItemEffect( m_pArmor );
		}
		break;
	case WEAR_ITEM_KIND_ACCESSORY_UNLIMITED:
		{
			DeleteWearItemEffect( m_pContainer );
			m_pContainer = NULL;
		}
		break;
	case WEAR_ITEM_KIND_ENGINE:
		{
			DeleteWearItemEffect( m_pEngine );
			m_pEngine = NULL;
		}
		break;
	case WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT:
		{
			DeleteWearItemEffect( m_pAccessories );
			m_pAccessories = NULL;
		}
		break;
	case WEAR_ITEM_KIND_WINGIN:
		{
			DeleteWearItemEffect( m_pWingIn );
			m_pWingIn = NULL;
		}
		break;
	// 2010-06-08 by dgwoo, ��ý��� �߰�. 
	case WEAR_ITEM_KIND_PET:
		{
			if(m_pPartner)
			{
				DeleteWearItemEffect(m_pPartner);	
			}
			if(m_pPartner1)
			{
				DeleteWearItemEffect(m_pPartner1);
			}

			if(m_pDummyPartner)
			{
				DeleteWearItemEffect(m_pDummyPartner);
			}

			if(m_pDummyPartner1)
			{
				DeleteWearItemEffect(m_pDummyPartner1);
			}
			//util::del(m_pPartner);
			m_pPartner = NULL;
			m_pPartner1 = NULL;
			m_pDummyPartner = NULL;
			m_pDummyPartner1 = NULL;
		}
		break;
	// 2010-06-08 by dgwoo, ��ý��� �߰�. 
	}


}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::DeleteWearItemEffect( int nWearItemType )
/// \brief		������ �������� ����
/// \author		dhkwon
/// \date		2004-03-23 ~ 2004-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::DeleteWearItemEffect( CAppEffectData * pEffect )
{
	FLOG("CUnitData::DeleteWearItemEffect( CAppEffectData * pEffect )");
	if(pEffect)
	{
		pEffect->m_bUsing = FALSE;
		pEffect = NULL;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::CreatePrimaryWeaponByFieldServer( CUnitData* pTarget, 
///												  MSG_FC_BATTLE_ATTACK_RESULT_PRIMARY *pPrimary, 
///												  MSG_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY* pItemPrimary  )
/// \brief		Field server���� Primaryweapon������ ����� �������� ��� �����ϴ� �ڵ�
/// \author		dhkwon
/// \date		2004-03-21 ~ 2004-03-21
/// \warning	pPrimary, pItemPrimary ���� �ϳ��� NULL�� �ƴϾ�� �Ѵ�.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
/*
void CUnitData::CreatePrimaryWeaponByFieldServer( CUnitData* pTarget, 
												  MSG_FC_BATTLE_ATTACK_RESULT_PRIMARY *pPrimary, 
												  MSG_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY* pItemPrimary  )
{
	FLOG("CUnitData::CreatePrimaryWeaponByFieldServer( CUnitData* pTarget, ...");
	ASSERT_ASSERT( (pPrimary && !pItemPrimary) || (!pPrimary && pItemPrimary) );
	ITEM * pItem = g_pDatabase->GetServerItemInfo(MSG_PRIMARY_WEAPONITEMNUMBER(pPrimary,pItemPrimary));
	if(!pItem)
	{
		DBGOUT("������ ������ ���� ��û���� ���� ���� �ȵ�(index:%d)\n",MSG_PRIMARY_WEAPONITEMNUMBER(pPrimary,pItemPrimary));
		return;
	}
	if(pPrimary)
	{
		if(m_dwPartType == _MONSTER && m_PrimaryAttackData1.WeaponItemNumber != pPrimary->WeaponItemNumber)
		{
			m_PrimaryAttack.fCheckAttackTime = 0.0f;
		}
		memcpy(&m_PrimaryAttackData1,pPrimary,sizeof(MSG_FC_BATTLE_ATTACK_RESULT_PRIMARY));
		m_bIsItemAttack = FALSE;
	}
	else
	{
		memcpy(&m_PrimaryAttackData2,pItemPrimary,sizeof(MSG_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY));
		m_bIsItemAttack = TRUE;
	}
	m_pPrimaryAttackTarget = pTarget;
	if(this == g_pShuttleChild || m_dwPartType == _ENEMY)
	{
		CheckPrimaryAttack(TRUE, NULL);
	}
	else// monster
	{
		CheckPrimaryAttack(TRUE, pItem);
	}
}
*/
///////////////////////////////////////////////////////////////////////////////
/// \fn			CUnitData::CreateWeaponByFieldServer( MSG_FC_BATTLE_ATTACK_OK* pMsg )
/// \brief		Attack ��� ó��(ȭ�鿡 �Ѿ� ����Ʈ ����)
///				1. Attack index�� ������ �Ǻ� : CAtumApplication::FieldSocketBattleAttackOk
///				3. 1,2�� ���� ( this function )
///				2. AttackType���� ���ݹ�� �Ǻ� : CUnitData::CheckAttack
///				4. Target ó��(������,����) : Weapon constructor
/// \author		dhkwon
/// \date		2004-09-07 ~ 2004-09-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::CreateWeaponByFieldServer( MSG_FC_BATTLE_ATTACK_OK* pMsg )
{
	if(IS_PRIMARY_ATT_TYPE(pMsg->AttackType))
	{// 1��
		m_PrimaryAttack.AttackData = *pMsg;

		// 2004-10-26 by jschoi
		// ���� �߻��� 1�� ����� �߻� �������� �̹� �Ѿ��� ��������Ƿ� ���̻� �̰����� �������� �ʴ´�.
//		if( m_dwPartType == _SHUTTLE || 
		if(	m_dwPartType == _ENEMY || 
			m_dwPartType == _ADMIN)
		{
			CheckAttack( m_PrimaryAttack, TRUE, NULL );			
		}
		else if(m_dwPartType != _SHUTTLE) // monster
		{
			ITEM* pWeaponITEM = g_pDatabase->GetServerItemInfo(pMsg->ItemNum);
			if(pWeaponITEM)
			{
				CheckAttack( m_PrimaryAttack, TRUE, pWeaponITEM);
			}
			else
			{
				DBGOUT("Primary Monster Weapon Not DB(%d)\n", pMsg->ItemNum);
			}
		}
	}
	else if(IS_SECONDARY_ATT_TYPE(pMsg->AttackType))
	{// 2��
		// 2005-08-25 by ispark
		// �ּ� �����ӽ� ������ �ش� ���� �ʴ� 2��������� ����
		// 2006-09-27 by ispark, ��ũ�������� �ƴ϶�� �ؿ� ���ǹ� ����
//#ifndef _SCREENSHOT
		if((g_pSOption->sLowQuality ||
			g_pD3dApp->GetFPS() < 20.0f) &&
			(pMsg->TargetInfo.TargetIndex != g_pShuttleChild->m_myShuttleInfo.ClientIndex 
			&& m_nTargetIndex != g_pShuttleChild->m_myShuttleInfo.ClientIndex
			&& m_dwPartType != _SHUTTLE
			&& !pMsg->SkillNum// ��ų�� �ִ� 2������� ������ ���δ�.// 2008-03-14 by bhsohn ���� ��ų �Ⱥ��̴� ���� ����
			))
		{
			return;
		}
//#endif
		m_SecondaryAttack.AttackData = *pMsg;

		if( m_dwPartType == _SHUTTLE || 
			m_dwPartType == _ENEMY || 
			m_dwPartType == _ADMIN)
		{
			CheckAttack( m_SecondaryAttack, TRUE, NULL);
		}
		else // monster
		{
			ITEM* pWeaponITEM = g_pDatabase->GetServerItemInfo(pMsg->ItemNum);
			if(pWeaponITEM)
			{
				CheckAttack( m_SecondaryAttack, TRUE, pWeaponITEM );
			}
			else
			{
				DBGOUT("Secondary Monster Weapon Not DB(%d)\n", pMsg->ItemNum);
			}
		}
	}
	// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����
	else if( ATT_TYPE_PET_GENERAL == pMsg->AttackType )
	{
		// 2010-06-15 by shcho&hslee ��ý���
		m_PetAttack.AttackData = *pMsg;

		if(	m_dwPartType == _ENEMY || 
			m_dwPartType == _ADMIN)
		{
			CheckAttack( m_PetAttack, TRUE, NULL );			
		}
		else if(m_dwPartType != _SHUTTLE) // monster
		{
			ITEM* pWeaponITEM = g_pDatabase->GetServerItemInfo(pMsg->ItemNum);
			if(pWeaponITEM)
			{
				CheckAttack( m_PetAttack, TRUE, pWeaponITEM);
			}
			else
			{
				DBGOUT("Pet Not DB(%d)\n", pMsg->ItemNum);
			}
		}
	}
	// end 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����
	#ifdef _DEBUG
	else
	{
		DBGOUT("ATTACK TYPE Is Wrong.\n");//ATTACK TYPE �߸���.\n
	}
	#endif // _DEBUG_endif
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::CreatePrimaryWeapon( CUnitData* pTarget, 
///									 ITEM* pItem,
///									 MSG_FC_BATTLE_ATTACK_RESULT_PRIMARY *pPrimary, 
///									 MSG_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY* pItemPrimary  )
/// \brief		1�� ���⸦ �����ؼ�, Scene�� m_pWeaponData�� ���δ�.
/// \author		dhkwon
/// \date		2004-03-20 ~ 2004-03-20
/// \warning	pPrimary�� pItemPrimary�� ���� �ϳ��� NULL�̾� �Ѵ�. �Ѵ� NULL�� �ȵȴ�.
///				���� �ϳ��� NULL�� �ƴϸ� Protocol�� ���� ó���̰�,
///				�ϳ��� NULL�̶�� Ŭ���̾�Ʈ ó���̴�.
///				ATTACK_RESULT, ATTACK_ITEM_RESULT �� �����Ұ�.
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
/*
void CUnitData::CreatePrimaryWeapon( CUnitData* pTarget, 
									 ITEM* pItem,
									 MSG_FC_BATTLE_ATTACK_RESULT_PRIMARY *pPrimary, 
									 MSG_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY* pItemPrimary  )
{
	FLOG("CUnitData::CreatePrimaryWeapon( CUnitData* pTarget, ...");
	ASSERT_ASSERT( (pPrimary && !pItemPrimary) || (!pPrimary && pItemPrimary) );
	switch(pItem->OrbitType)
	{
	case ORBOT_SINGLE_CROSS_FIRE:
	case ORBIT_SINGLE_FIRE_200:
	case ORBIT_SINGLE_FIRE_250:
	case ORBIT_SINGLE_FIRE_300:
	case ORBIT_SINGLE_FIRE_320:
	case ORBIT_SINGLE_FIRE_350:
		{
			CWeaponFastData * pWeapon = CREATE_PRIMARY_SINGLE_WEAPON(pPrimary, pItemPrimary);
			g_pScene->m_pWeaponData->AddChild(pWeapon);
			ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
		}
		break;
	case ORBIT_TWIN_CROSS_FIRE:
	case ORBIT_TWIN_FIRE_200:
	case ORBIT_TWIN_FIRE_250:
	case ORBIT_TWIN_FIRE_300:
	case ORBIT_TWIN_FIRE_320:
	case ORBIT_TWIN_FIRE_350:
		{
			CWeaponFastData * pWeapon = CREATE_PRIMARY_DOUBLE_WEAPON(pPrimary, pItemPrimary, 0);			
			g_pScene->m_pWeaponData->AddChild(pWeapon);
			pWeapon = CREATE_PRIMARY_DOUBLE_WEAPON(pPrimary, pItemPrimary, 1);			
			g_pScene->m_pWeaponData->AddChild(pWeapon);
			ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
		}
		break;
	case ORBIT_CROSS_FIRE:
		{
			int nWeaponIndex = pPrimary ? pPrimary->WeaponIndex : pItemPrimary->WeaponIndex;
			m_PrimaryAttack.bZigZagWeapon = !m_PrimaryAttack.bZigZagWeapon;
			if(m_PrimaryAttack.bZigZagWeapon)
			{
				CWeaponFastData * pWeapon = CREATE_PRIMARY_DOUBLE_WEAPON(pPrimary, pItemPrimary, 0);			
				g_pScene->m_pWeaponData->AddChild(pWeapon);
				ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			}
			else
			{
				CWeaponFastData * pWeapon = CREATE_PRIMARY_DOUBLE_WEAPON(pPrimary, pItemPrimary, 1);			
				g_pScene->m_pWeaponData->AddChild(pWeapon);
				ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_2);
			}
		}
		break;
	case ORBIT_FIRE:
		{
			CWeaponFireData * pWeapon = CREATE_PRIMARY_FIRE_WEAPON_BY_ITEM_INDEX(pPrimary, pItemPrimary);			
			g_pScene->m_pWeaponData->AddChild(pWeapon);
			ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
		}
		break;
	case ORBIT_ATTACK_ALL:
		{
			int nItemNum = 0;
			if(pPrimary)
				nItemNum = pPrimary->WeaponItemNumber;
			else if(pItemPrimary)
				nItemNum = pItemPrimary->WeaponItemNumber;
			CWeaponAllAttackData* pWeapon = new CWeaponAllAttackData(this, 
																	NULL, 
																	BODYCON_FIRE_MASK, 
																	nItemNum );
			g_pScene->m_pWeaponData->AddChild(pWeapon);
		}
		break;
	default:
		{
			DBGOUT("\n\nOrbitType�� �߸���(ItemNum:%d, OrbitType:%d)\n\n", pItem->ItemNum, pItem->OrbitType);
			return;
		}
	}
}

*/
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ResetPrimaryWeaponBodyCondition( BodyCond_t hyBodyCon)
/// \brief		1�� ������ �ٵ�������� �����Ѵ�. (ResetBodycondition : ������������ �ٵ�������̸�
///				�ִϸ��̼��� �ٽ� �����Ѵ�.)
/// \author		dhkwon
/// \date		2004-03-20 ~ 2004-03-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ResetPrimaryWeaponBodyCondition( BodyCond_t hyBodyCon)
{
	FLOG("CUnitData::ResetPrimaryWeaponBodyCondition( BodyCond_t hyBodyCon)");
	if(m_pWeapon1_1_1 && m_pWeapon1_1_1->m_pCharacterInfo)
	{
		m_pWeapon1_1_1->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon1_1_1->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
//		DBGOUT("CUnitData::ResetPrimaryWeaponBodyCondition( %016I64X )\n", hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(m_pWeapon1_1_2 && m_pWeapon1_1_2->m_pCharacterInfo)
	{
		m_pWeapon1_1_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon1_1_2->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
//		DBGOUT("CUnitData::ResetPrimaryWeaponBodyCondition( %016I64X )\n", hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(m_pWeapon1_2 && m_pWeapon1_2->m_pCharacterInfo)
	{
		m_pWeapon1_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon1_2->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ChangePrimaryWeaponBodyCondition( BodyCond_t hyBodyCon)
/// \brief		1�� ������ �ٵ�������� ��ü�Ѵ�. (ChangeBodycondition : ������������ �ٵ����� �׳� �д�.)
/// \author		dhkwon
/// \date		2004-03-22 ~ 2004-03-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ChangePrimaryWeaponBodyCondition( BodyCond_t hyBodyCon)
{
	FLOG("CUnitData::ChangePrimaryWeaponBodyCondition( BodyCond_t hyBodyCon)");
	if(m_pWeapon1_1_1 && m_pWeapon1_1_1->m_pCharacterInfo)
	{
		m_pWeapon1_1_1->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon1_1_1->m_pCharacterInfo->ChangeBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(m_pWeapon1_1_2 && m_pWeapon1_1_2->m_pCharacterInfo)
	{
		m_pWeapon1_1_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon1_1_2->m_pCharacterInfo->ChangeBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(m_pWeapon1_2 && m_pWeapon1_2->m_pCharacterInfo)
	{
		m_pWeapon1_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon1_2->m_pCharacterInfo->ChangeBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ResetSecondaryWeaponBodyCondition( BodyCond_t hyBodyCon)
/// \brief		2�� ������ �ٵ�������� �����Ѵ�. (ResetBodycondition : ������������ �ٵ�������̸�
///				�ִϸ��̼��� �ٽ� �����Ѵ�.)
/// \author		dhkwon
/// \date		2004-03-20 ~ 2004-03-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ResetSecondaryWeaponBodyCondition( BodyCond_t hyBodyCon, BOOL i_bIsLeft/*=TRUE*/)
{
	FLOG("CUnitData::ResetSecondaryWeaponBodyCondition( BodyCond_t hyBodyCon)");

	if(i_bIsLeft && m_pWeapon2_1_1 && m_pWeapon2_1_1->m_pCharacterInfo)
	{
		m_pWeapon2_1_1->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon2_1_1->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(FALSE == i_bIsLeft && m_pWeapon2_1_2 && m_pWeapon2_1_2->m_pCharacterInfo)
	{
		m_pWeapon2_1_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon2_1_2->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(m_pWeapon2_2 && m_pWeapon2_2->m_pCharacterInfo)
	{
		m_pWeapon2_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon2_2->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
//	DBGOUT("CUnitData::ResetSecondaryWeaponBodyCondition(%016I64X)\n", hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ChangeSecondaryWeaponBodyCondition( BodyCond_t hyBodyCon)
/// \brief		2�� ������ �ٵ�������� ��ü�Ѵ�. (ChangeBodycondition : ������������ �ٵ����� �׳� �д�.)
/// \author		dhkwon
/// \date		2004-03-22 ~ 2004-03-22
/// \warning	������ ����
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ChangeSecondaryWeaponBodyCondition( BodyCond_t hyBodyCon)
{
	FLOG("CUnitData::ChangeSecondaryWeaponBodyCondition( BodyCond_t hyBodyCon)");
	if(m_pWeapon2_1_1 && m_pWeapon2_1_1->m_pCharacterInfo)
	{
		m_pWeapon2_1_1->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon2_1_1->m_pCharacterInfo->ChangeBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(m_pWeapon2_1_2 && m_pWeapon2_1_2->m_pCharacterInfo)
	{
		m_pWeapon2_1_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon2_1_2->m_pCharacterInfo->ChangeBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(m_pWeapon2_2 && m_pWeapon2_2->m_pCharacterInfo)
	{
		m_pWeapon2_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon2_2->m_pCharacterInfo->ChangeBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	DBGOUT("CUnitData::ChangeSecondaryWeaponBodyCondition(%016I64X)\n", hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ResetPetWeaponBodyCondition( BodyCond_t hyBodyCon)
/// \brief		�� ������ �ٵ�������� �����Ѵ�. (ResetBodycondition : ������������ �ٵ�������̸�
///				�ִϸ��̼��� �ٽ� �����Ѵ�.)
/// \author		shcho&hslee
/// \date		2010-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ResetPetWeaponBodyCondition(BodyCond_t hyBodyCon, BOOL i_bIsLeft/* =TRUE */)
{
	FLOG("CUnitData::ResetPrimaryWeaponBodyCondition( BodyCond_t hyBodyCon)");
	if(m_pDummyPartner && m_pDummyPartner->m_pCharacterInfo && i_bIsLeft)
	{
		m_pDummyPartner->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pDummyPartner->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
//		DBGOUT("CUnitData::ResetPrimaryWeaponBodyCondition( %016I64X )\n", hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(m_pDummyPartner1 && m_pDummyPartner1->m_pCharacterInfo && i_bIsLeft)
	{
		m_pDummyPartner1->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pDummyPartner1->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
//		DBGOUT("CUnitData::ResetPrimaryWeaponBodyCondition( %016I64X )\n", hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
}

/*
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::CreateSecondaryWeaponByFieldServer( CUnitData* pTarget, 
///												  MSG_FC_BATTLE_ATTACK_RESULT_SECONDARY *pPrimary, 
///												  MSG_FC_BATTLE_ATTACK_ITEM_RESULT_SECONDARY* pItemPrimary  )
/// \brief		Field server���� Secondary weapon������ ����� �������� ��� �����ϴ� �ڵ�
/// \author		dhkwon
/// \date		2004-03-21 ~ 2004-03-21
/// \warning	pPrimary, pItemPrimary ���� �ϳ��� NULL�� �ƴϾ�� �Ѵ�.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::CreateSecondaryWeaponByFieldServer( CUnitData* pTarget, 
												    MSG_FC_BATTLE_ATTACK_RESULT_SECONDARY *pPrimary, 
												    MSG_FC_BATTLE_ATTACK_ITEM_RESULT_SECONDARY* pItemPrimary  )
{
	FLOG("CUnitData::CreateSecondaryWeaponByFieldServer(CUnitData * pTarget,...)");

	ITEM * pItem = g_pDatabase->GetServerItemInfo(MSG_SECONDARY_WEAPONITEMNUMBER(pPrimary,pItemPrimary));
	if(!pItem)
	{
		DBGOUT("������ ������ ���� ��û���� ���� ���� �ȵ�(index:%d)\n",MSG_SECONDARY_WEAPONITEMNUMBER(pPrimary,pItemPrimary));
		return;
	}
	CreateSecondaryWeapon( pTarget, pItem, pPrimary, pItemPrimary );
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::CreateSecondaryWeapon( CUnitData* pTarget, 
//									 ITEM* pItem,
///									 MSG_FC_BATTLE_ATTACK_RESULT_PRIMARY *pPrimary, 
///									 MSG_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY* pItemPrimary  )
/// \brief		2�� ���⸦ �����ؼ�, Scene�� m_pWeaponData�� ���δ�.
/// \author		dhkwon
/// \date		2004-03-20 ~ 2004-03-20
/// \warning	pPrimary�� pItemPrimary�� ���� �ϳ��� NULL�̾� �Ѵ�. �Ѵ� NULL�� �ȵȴ�.
///				ATTACK_RESULT, ATTACK_ITEM_RESULT �� �����Ұ�.
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::CreateSecondaryWeapon( CUnitData* pTarget, 
									   ITEM* pItem,
									   MSG_FC_BATTLE_ATTACK_RESULT_SECONDARY *pPrimary, 
									   MSG_FC_BATTLE_ATTACK_ITEM_RESULT_SECONDARY* pItemPrimary  )
{
	FLOG("CUnitData::CreateSecondaryWeapon( CUnitData* pTarget, ");
	ASSERT_ASSERT( g_pScene && g_pScene->m_pWeaponData && ((pPrimary && !pItemPrimary) || (!pPrimary && pItemPrimary)) );
	CWeapon* pWeapon = NULL;
	switch(pItem->OrbitType)
	{
	case ORBIT_STRAIGHT_ROCKET_250:
	case ORBIT_FALL_ROCKET_250:
	case ORBIT_LEFT_ROCKET_200:
	case ORBIT_RIGHT_ROCKET_200:
	case ORBIT_THREE_ROCKET:
	case ORBIT_CHILD_ROCKET:
		{
			CWeaponRocketData *pRocketWeapon = CREATE_SECONDARY_WEAPON_ROCKET(pPrimary, pItemPrimary);
			pRocketWeapon->m_pItemData = pItem;
			pRocketWeapon->InitData();
			pWeapon = (CWeapon*)pRocketWeapon;
		}
		break;
	case ORBIT_STRAIGHT_MISSILE_300:
	case ORBIT_FALL_MISSILE_300:
	case ORBIT_LEFT_MISSILE_300:
	case ORBIT_RIGHT_MISSILE_270:
	case ORBIT_UPDOWN_ROCKET_270:
	case ORBIT_UPDOWN_MISSILE_300:
		{
			CWeaponMissileData *pMissileWeapon = CREATE_SECONDARY_WEAPON_MISSILE(pPrimary, pItemPrimary);
			pMissileWeapon->m_pItemData = pItem;
			pMissileWeapon->InitData();
			pWeapon = (CWeapon*)pMissileWeapon;
		}
		break;
	default:
		{
			DBGOUT("\n\nOrbitType�� �߸���(ItemNum:%d, OrbitType:%d)\n\n", pItem->ItemNum, pItem->OrbitType);
			return;
		}
	}
	g_pScene->m_pWeaponData->AddChild(pWeapon);
	ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
}
*/

// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
//void CUnitData::CreateWeapon(ATTACK_DATA& attackData, ITEM* pWeaponITEM)
//void CUnitData::CreateWeapon( ATTACK_DATA& attackData, ITEM* pWeaponITEM, ITEM* pEffectItem /* = NULL */ )
// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
void CUnitData::CreateWeapon( ATTACK_DATA& attackData, ITEM* pWeaponITEM, ITEM* pEffectItem /* = NULL */, int LoadingPriority )
//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
{
	if(!pWeaponITEM)
		return;										//2013-06-03 by ssjung Canada �ͼ��� �����ڵ� �߰� 

	// 2013-08-06 by bhsohn ĳ���� �����ڵ� �߰�
	if(!g_pScene || !g_pScene->m_pWeaponData)
	{
		return;
	}
	// END 2013-08-06 by bhsohn ĳ���� �����ڵ� �߰�
 	CWeapon* pWeapon = NULL;
	switch(pWeaponITEM->OrbitType)
	{
	case ORBIT_OBJBEAM:
	case ORBIT_SINGLE_CROSS_FIRE:
	case ORBIT_SINGLE_FIRE_100:
	case ORBIT_SINGLE_FIRE_150:
	case ORBIT_SINGLE_FIRE_200:
	case ORBIT_SINGLE_FIRE_250:
	case ORBIT_SINGLE_FIRE_300:
	case ORBIT_SINGLE_FIRE_320:
	case ORBIT_SINGLE_FIRE_350:
		{
			// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
			// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			//pWeapon = new CWeaponFastData(this, pWeaponITEM, attackData);
			//pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem );
			// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem, LoadingPriority);
			//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

			g_pScene->m_pWeaponData->AddChild(pWeapon);
			// 2010-06-15 by shcho&hslee ��ý��� - ���� ����Ʈ �߰�
			//ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			if( IS_PRIMARY_ATT_TYPE(attackData.AttackData.AttackType) )
			{
				ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			}
			else if(attackData.AttackData.AttackType == ATT_TYPE_PET_GENERAL)
			{
				ResetPetWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			}
			// end 2010-06-15 by shcho&hslee ��ý��� - ���� ����Ʈ �߰�
		}
		break;
	case ORBIT_TWIN_CROSS_FIRE:
	case ORBIT_TWIN_FIRE_100:
	case ORBIT_TWIN_FIRE_150:
	case ORBIT_TWIN_FIRE_200:
	case ORBIT_TWIN_FIRE_250:
	case ORBIT_TWIN_FIRE_300:
	case ORBIT_TWIN_FIRE_320:
	case ORBIT_TWIN_FIRE_350:
		{
			attackData.bZigZagWeapon = FALSE;

 			// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
			// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			//pWeapon = new CWeaponFastData(this, pWeaponITEM, attackData);
			//pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem );
			// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem, LoadingPriority);
			//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����


			g_pScene->m_pWeaponData->AddChild(pWeapon);
			attackData.bZigZagWeapon = TRUE;
			if( attackData.AttackData.SkillNum == 0 )
			{
				
			// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
			// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			//pWeapon = new CWeaponFastData(this, pWeaponITEM, attackData);
			//pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem );
			// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem, LoadingPriority);
			//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

				g_pScene->m_pWeaponData->AddChild(pWeapon);
				// 2010-06-15 by shcho&hslee ��ý��� - ������Ʈ ���� ó��
				//ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
				if( IS_PRIMARY_ATT_TYPE(attackData.AttackData.AttackType) )
				{
					ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
				}
				else if(attackData.AttackData.AttackType == ATT_TYPE_PET_GENERAL)
				{
					ResetPetWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
				}
				// end 2010-06-15 by shcho&hslee ��ý��� - ������Ʈ ���� ó��
				
			}
			else
			{
				// �ٽ� ����. by dhkwon, 2006-10-19
				if(m_dwPartType == _SHUTTLE && 
					attackData.AttackData.AttackIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
				{
					// ��ų�� �ִ� �Ѿ� �߻�ÿ��� ReattackTime���� �Ѿ��� ��� ������ �Ѵ�.
					g_pShuttleChild->m_pPrimaryWeapon->SetReattackCheckTime(0);
					g_pShuttleChild->m_pPrimaryWeapon->SetAttackCount(0);
				}
				attackData.AttackData.SkillNum = 0;
			}
		}
		break;
	case ORBIT_CROSS_FIRE:
		{
			attackData.bZigZagWeapon = !attackData.bZigZagWeapon;
 			// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
			// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			//pWeapon = new CWeaponFastData(this, pWeaponITEM, attackData);
			//pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem );
			// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

			pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem, LoadingPriority);
			//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

			g_pScene->m_pWeaponData->AddChild(pWeapon);
			if(attackData.bZigZagWeapon)
			{
				ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1, attackData.bZigZagWeapon);
//				ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_2);
			}
			else
			{
				ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1, attackData.bZigZagWeapon);
			}
			// 2010-06-15 by shcho&hslee ��ý��� - ���� ����Ʈ �߰�

			if(attackData.AttackData.AttackType == ATT_TYPE_PET_GENERAL)
			{
				ResetPetWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			}
			// end 2010-06-15 by shcho&hslee ��ý��� - ���� ����Ʈ �߰�
		}
		break;
	case ORBIT_FIRE:
		{
 			// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
			// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			//pWeapon = new CWeaponFastData(this, pWeaponITEM, attackData);
			//pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem );
			// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem, LoadingPriority);
			//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

			g_pScene->m_pWeaponData->AddChild(pWeapon);
			// 2010-06-15 by shcho&hslee ��ý��� - ���� ����Ʈ �߰�
			//ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			if( IS_PRIMARY_ATT_TYPE(attackData.AttackData.AttackType) )
			{
				ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			}
			else if(attackData.AttackData.AttackType == ATT_TYPE_PET_GENERAL)
			{
				ResetPetWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			}
			// end 2010-06-15 by shcho&hslee ��ý��� - ���� ����Ʈ �߰�
		}
		break;
	case ORBIT_BODYSLAM:
		{
 			// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
			// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			//pWeapon = new CWeaponFastData(this, pWeaponITEM, attackData);
			//pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem );
			// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem, LoadingPriority);
			//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

			g_pScene->m_pWeaponData->AddChild(pWeapon);
			// 2010-06-15 by shcho&hslee ��ý��� - ���� ����Ʈ �߰�
			//ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			if( IS_PRIMARY_ATT_TYPE(attackData.AttackData.AttackType) )
			{
				ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			}
			else if(attackData.AttackData.AttackType == ATT_TYPE_PET_GENERAL)
			{
				ResetPetWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			}
			// end 2010-06-15 by shcho&hslee ��ý��� - ���� ����Ʈ �߰�
		}
		break;
	case ORBIT_ATTACK_ALL:
		{
			int nItemNum = attackData.AttackData.ItemNum;

			// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
			// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			//pWeapon = new CWeaponAllAttackData(this, NULL, BODYCON_FIRE_MASK, nItemNum);
			//pWeapon = new CWeaponAllAttackData( this, NULL, BODYCON_FIRE_MASK, nItemNum, pEffectItem );
			pWeapon = new CWeaponAllAttackData( this, NULL, BODYCON_FIRE_MASK, nItemNum, pEffectItem, LoadingPriority);
			// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

			g_pScene->m_pWeaponData->AddChild(pWeapon);
		}
		break;
	case ORBIT_STRAIGHT_ROCKET_250:
	case ORBIT_FALL_ROCKET_250:
	case ORBIT_LEFT_ROCKET_200:
	case ORBIT_RIGHT_ROCKET_200:
	case ORBIT_THREE_ROCKET:
	case ORBIT_CHILD_ROCKET:
		{
			attackData.bZigZagWeapon = !attackData.bZigZagWeapon;
			if(attackData.AttackData.FirePosition.x < 0)
			{
				attackData.bZigZagWeapon = TRUE;
			}
			else if(attackData.AttackData.FirePosition.x > 0)
			{
				attackData.bZigZagWeapon = FALSE;
			}
			attackData.fWarheadSpeed = CAtumSJ::GetWarHeadSpeed(pWeaponITEM,&m_paramFactor);

			// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
			// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			//CWeaponRocketData* pWeaponRocketData = new CWeaponRocketData(this, pWeaponITEM, attackData);
			//CWeaponRocketData* pWeaponRocketData = new CWeaponRocketData( this, pWeaponITEM, attackData, pEffectItem);
			// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			CWeaponRocketData* pWeaponRocketData = new CWeaponRocketData( this, pWeaponITEM, attackData, pEffectItem, LoadingPriority);
			//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����


			pWeapon = (CWeapon*)pWeaponRocketData;
			g_pScene->m_pWeaponData->AddChild(pWeapon);
//			ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1, (attackData.AttackData.FirePosition.x<0)?TRUE:FALSE);
			// 2006-10-09 by ispark, ���Ͽ��� bZigZagWeapon���� �� ������ �ݴ� �����̴�.
			ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1, !attackData.bZigZagWeapon);
//			DBGOUT("������� %d\n", attackData.bZigZagWeapon);
			// 2006-10-19 by ispark, ���� üũ
			// �ٽ� �ѹ� ����. by dhkwon, 2006-10-19
			if(m_dwPartType == _SHUTTLE &&
				g_pShuttleChild->m_pSecondaryWeapon->GetItemGeneral()->CurrentCount <= 0)
			{
				ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_RELOAD, !attackData.bZigZagWeapon);
			}
		}
		break;
	case ORBIT_STRAIGHT_MISSILE_300:
	case ORBIT_FALL_MISSILE_300:
	case ORBIT_LEFT_MISSILE_300:
	case ORBIT_RIGHT_MISSILE_270:

//	case ORBIT_UPDOWN_MISSILE_300:
		{
			attackData.bZigZagWeapon = !attackData.bZigZagWeapon;
			if(attackData.AttackData.FirePosition.x < 0)
			{
				attackData.bZigZagWeapon = FALSE;
			}
			else if(attackData.AttackData.FirePosition.x > 0)
			{
				attackData.bZigZagWeapon = TRUE;
			}
			attackData.fExplosionRange = CAtumSJ::GetExplosionRange(pWeaponITEM, &g_pShuttleChild->m_paramFactor);
			// 2007-06-15 by dgwoo �Ʒ� �Լ��� �����.
			attackData.fWarheadSpeed = CAtumSJ::GetWarHeadSpeed(pWeaponITEM,&m_paramFactor);
			//pWeaponITEM->RepeatTime * (1.0f+m_paramFactor.pfm_WARHEAD_SPEED);

			// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
			// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			//CWeaponMissileData *pWeaponMissileData = new CWeaponMissileData(this, pWeaponITEM, attackData);
			//CWeaponMissileData *pWeaponMissileData = new CWeaponMissileData( this, pWeaponITEM, attackData, pEffectItem );
			// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			CWeaponMissileData *pWeaponMissileData = new CWeaponMissileData( this, pWeaponITEM, attackData, pEffectItem,LoadingPriority);	
			//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

			pWeapon = (CWeapon*)pWeaponMissileData;
			g_pScene->m_pWeaponData->AddChild(pWeapon);
			// 2006-10-09 by ispark, �̻��Ͽ��� WEAPON_BODYCON_FIRE_1�� �ǹ̰� ����.
			ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1, (attackData.AttackData.FirePosition.x<0)?TRUE:FALSE);

			// 2006-10-19 by ispark, ���� üũ
			// �ٽ� �ѹ� ����, 2006-10-19 by dhkwon
			if(m_dwPartType == _SHUTTLE &&
				g_pShuttleChild->m_pSecondaryWeapon->GetItemGeneral()->CurrentCount <= 0)
			{
				ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_RELOAD);
				ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_RELOAD, FALSE);
			}
		}
		break;
	case ORBIT_UPDOWN_ROCKET_270:
		{
			attackData.fWarheadSpeed = CAtumSJ::GetWarHeadSpeed(pWeaponITEM,&m_paramFactor);

			// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
			// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			//CWeaponRocketData* pWeaponRocketData = new CWeaponRocketData(this, pWeaponITEM, attackData);
			//CWeaponRocketData* pWeaponRocketData = new CWeaponRocketData( this, pWeaponITEM, attackData, pEffectItem);
			// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			CWeaponRocketData* pWeaponRocketData = new CWeaponRocketData( this, pWeaponITEM, attackData, pEffectItem);
			//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

			pWeapon = (CWeapon*)pWeaponRocketData;
			g_pScene->m_pWeaponData->AddChild(pWeapon);
			ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1, (attackData.AttackData.FirePosition.x<0)?TRUE:FALSE);
		}
		break;
	case ORBIT_UPDOWN_MISSILE_300:
		{
			// 2006-08-14 by ispark, �������� ���� ���̴� ���� �ִ�. �׷��Ƿ� �ӽ� ������ �ٽ� ����
			BOOL bSaveZigZagTemp = attackData.bZigZagWeapon;
			attackData.bZigZagWeapon = 0;
			attackData.fWarheadSpeed = CAtumSJ::GetWarHeadSpeed(pWeaponITEM,&m_paramFactor);

			// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
			// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			//CWeaponMissileData *pWeaponMissileData = new CWeaponMissileData(this, pWeaponITEM, attackData);
			//CWeaponMissileData *pWeaponMissileData = new CWeaponMissileData( this, pWeaponITEM, attackData, pEffectItem );
			// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
			CWeaponMissileData *pWeaponMissileData = new CWeaponMissileData( this, pWeaponITEM, attackData, pEffectItem);
			//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

			pWeapon = (CWeapon*)pWeaponMissileData;
			g_pScene->m_pWeaponData->AddChild(pWeapon);
			ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1, (attackData.AttackData.FirePosition.x<0)?TRUE:FALSE);
			// �ٽ� ����
			attackData.bZigZagWeapon = bSaveZigZagTemp;
		}
		break;
	default:
		{
			DBGOUT("\n\nOrbitType is Wrong(ItemNum:%d, OrbitType:%d)\n\n", pWeaponITEM->ItemNum, pWeaponITEM->OrbitType);
			return;
		}
	}
}

float CUnitData::GetAutomaticAttackTime( BYTE nOrbitType )
{
	switch(nOrbitType)
	{
	case ORBIT_SINGLE_FIRE_100:
	case ORBIT_TWIN_FIRE_100:
		return 0.1f;
	case ORBIT_TWIN_FIRE_150:
	case ORBIT_SINGLE_FIRE_150:
		return 0.15f;
	case ORBIT_SINGLE_FIRE_200:
	case ORBIT_TWIN_FIRE_200:
	case ORBIT_LEFT_ROCKET_200:
	case ORBIT_RIGHT_ROCKET_200:
		return 0.2f;
	case ORBIT_SINGLE_FIRE_250:
	case ORBIT_TWIN_FIRE_250:
	case ORBIT_STRAIGHT_ROCKET_250:
	case ORBIT_FALL_ROCKET_250:
		return 0.25f;
	case ORBIT_RIGHT_MISSILE_270:
	case ORBIT_UPDOWN_ROCKET_270:
		return 0.27f;
	case ORBIT_SINGLE_FIRE_300:
	case ORBIT_STRAIGHT_MISSILE_300:
	case ORBIT_FALL_MISSILE_300:
	case ORBIT_LEFT_MISSILE_300:
	case ORBIT_TWIN_FIRE_300:
	case ORBIT_UPDOWN_MISSILE_300:
	case ORBIT_MINE_300:
	case ORBIT_RANGE_ROCKET_300:
		return 0.3f;
	case ORBIT_SINGLE_FIRE_320:
	case ORBIT_TWIN_FIRE_320:
		return 0.32f;
	case ORBIT_SINGLE_FIRE_350:
	case ORBIT_TWIN_FIRE_350:
		return 0.35f;
	case ORBIT_MINE_400:
	case ORBIT_RANGE_ROCKET_400:
		return 0.4f;
	default:
		return 0.3f;
	}
}

/*
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::CheckPrimaryAttack(BOOL bReset, ITEM* pPrimary)
/// \brief		Primary attack�� üũ�Ͽ� ���� ����
/// \author		dhkwon
/// \date		2004-03-21 ~ 2004-03-21
/// \warning	CShuttleChild, CEnemy, CMonster
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::CheckPrimaryAttack(BOOL bReset, ITEM* pPrimary)
{
	FLOG("CUnitData::CheckPrimaryAttack(BOOL bReset, ITEM* pPrimary)");

	ASSERT_ASSERT(pPrimary);
	if(bReset)
	{
		if( IS_CLIENT_SET_AUTOMATIC_TIMER(pPrimary->OrbitType) )
		{
			m_PrimaryAttack.bShootingWeapon = TRUE;
		}
		else
		{
			m_PrimaryAttack.nAttackCount = CAtumSJ::GetTotalPrimaryShotCountPerReattackTime(pPrimary, &m_paramFactor);
			m_fPrimaryAttackTime = CAtumSJ::GetShotCountReattackTime( pPrimary, &m_paramFactor );
		}
		//return;m_bPrimaryWeaponPrepare,m_bPrimaryAttackOrder
	}
	if(IS_CLIENT_SET_AUTOMATIC_TIMER(pPrimary->OrbitType))
	{
//		m_fAutoMaticTimer -= g_pD3dApp->GetElapsedTime();
		if(m_PrimaryAttack.fCheckAutoMaticTimer <= 0.0f && 
			m_PrimaryAttack.bShootingWeapon && 
			m_PrimaryAttack.nAttackCount <= 0)
		{
			m_PrimaryAttack.nAttackCount = CAtumSJ::GetTotalPrimaryShotCountPerReattackTime(pPrimary, &m_paramFactor);
			m_fPrimaryAttackTime = GetPrimaryAttackTime(pPrimary->OrbitType);
			m_PrimaryAttack.fCheckAutoMaticTimer = (float)CAtumSJ::GetReattackTime(pPrimary, &m_paramFactor)/1000.0f;
		}
	}
//	if(m_fCheckPrimaryAttackTime > 0.0f)
//		m_fCheckPrimaryAttackTime -= g_pD3dApp->GetElapsedTime();
	if(m_PrimaryAttack.nAttackCount>0 && m_PrimaryAttack.fCheckAttackTime <= 0.0f)
	{
		m_PrimaryAttack.fCheckAttackTime = m_fPrimaryAttackTime;
		if(!m_bIsItemAttack)
		{
			CreatePrimaryWeapon( (CUnitData *)m_pPrimaryAttackTarget, pPrimary, &m_PrimaryAttackData1, NULL );
		}
		else
		{
			CreatePrimaryWeapon( (CUnitData *)m_pPrimaryAttackTarget, pPrimary, NULL, &m_PrimaryAttackData2 );
		}
		m_PrimaryAttack.nAttackCount--;
		if(m_PrimaryAttack.nAttackCount == 0)
		{
			m_PrimaryAttack.bShootingWeapon = FALSE;
		}
	}
}
*/
USHORT CUnitData::GetShiftWeaponBodyconditionByUnitKind()
{
	FLOG("CUnitData::GetShiftWeaponBodyconditionByUnitKind()");
	return UNIT_KIND_SHIFT_DEFAULT;
}


void CUnitData::DeleteItemPoint()
{
	FLOG( "CUnitData::DeleteItemPoint()" );
//	if(m_pWeapon1_1)
//	{
//		m_pWeapon1_1->m_bUsing = FALSE;
//		m_pWeapon1_1 = NULL;
//	}
	if(m_pWeapon1_1_1)
	{
		m_pWeapon1_1_1->m_bUsing = FALSE;
		m_pWeapon1_1_1 = NULL;
	}
	if(m_pWeapon1_1_2)
	{
		m_pWeapon1_1_2->m_bUsing = FALSE;
		m_pWeapon1_1_2 = NULL;
	}
	if(m_pWeapon1_2)
	{
		m_pWeapon1_2->m_bUsing = FALSE;
		m_pWeapon1_2 = NULL;
	}
//	if(m_pWeapon2_1)
//	{
//		m_pWeapon2_1->m_bUsing = FALSE;
//		m_pWeapon2_1 = NULL;
//	}
	if(m_pWeapon2_1_1)
	{
		m_pWeapon2_1_1->m_bUsing = FALSE;
		m_pWeapon2_1_1 = NULL;
	}
	if(m_pWeapon2_1_2)
	{
		m_pWeapon2_1_2->m_bUsing = FALSE;
		m_pWeapon2_1_2 = NULL;
	}
	if(m_pWeapon2_2)
	{
		m_pWeapon2_2->m_bUsing = FALSE;
		m_pWeapon2_2 = NULL;
	}

	// 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
	if( m_pRobotAGearWeapon_1 )
	{
		m_pRobotAGearWeapon_1->m_bUsing		= FALSE;
		m_pRobotAGearWeapon_1				= NULL;
	}
	// end 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)


	if(m_pEngine)
	{
		m_pEngine->m_bUsing = FALSE;
		m_pEngine = NULL;
	}
	if(m_pRadar)
	{
		m_pRadar->m_bUsing = FALSE;
		m_pRadar = NULL;
	}
//	if(m_pArmor)
//	{
//		m_pArmor->m_bUsing = FALSE;
//		m_pArmor = NULL;
//	}
	if(m_pContainer)
	{
		m_pContainer->m_bUsing = FALSE;
		m_pContainer = NULL;
	}
	if(m_pAccessories)
	{
		m_pAccessories->m_bUsing = FALSE;
		m_pAccessories = NULL;
	}
	if(m_pWingIn)
	{
		m_pWingIn->m_bUsing = FALSE;
		m_pWingIn = NULL;
	}
	if(m_pPartner)
	{
		m_pPartner->m_bUsing = FALSE;
		m_pPartner = NULL;
	}
    // 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����
	if(m_pPartner1)
	{
		m_pPartner1->m_bUsing = FALSE;
		m_pPartner1 = NULL;
	}

	if(m_pDummyPartner)
	{
		m_pDummyPartner->m_bUsing = FALSE;
		m_pDummyPartner = NULL;
	}
	if(m_pDummyPartner1)
	{
		m_pDummyPartner1->m_bUsing = FALSE;
		m_pDummyPartner1 = NULL;
	}
    // end 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����
}

// 2010. 03. 18 by jskim ���ͺ��� ī��
//void CUnitData::ChangeUnitCharacterInfo(int nDefenseItemNum, int nUnitKind, BodyCond_t nBodyCon, BOOL bCharacter)
void CUnitData::ChangeUnitCharacterInfo(int nDefenseItemNum, int nUnitKind, BodyCond_t nBodyCon, BOOL bCharacter, BOOL bMonsterTransformer)
//end 2010. 03. 18 by jskim ���ͺ��� ī��
{
	int nTemp;
//	if(m_dwPartType == _ADMIN)
//	{
//		nTemp = 10000000;
//	}
//	else
	// 2010. 03. 18 by jskim ���ͺ��� ī��
//	{
//		nTemp = ::GetUnitNum( m_bDegree, nDefenseItemNum, nUnitKind, bCharacter);
//	}
	if(!bMonsterTransformer)
	{
		nTemp = ::GetUnitNum( m_bDegree, nDefenseItemNum, nUnitKind, bCharacter);
	}
	else
	{
		nTemp = nDefenseItemNum;
	}
	//end 2010. 03. 18 by jskim ���ͺ��� ī��
	if(nTemp != m_nUnitNum)
	{
   		m_nUnitNum = nTemp;
		char strFileName[16];
		wsprintf( strFileName, "%08d", m_nUnitNum );
		// 2010. 03. 18 by jskim ���ͺ��� ī��
		//LoadCharacterEffect(strFileName);
		LoadCharacterEffect(strFileName, m_MonsterTransformer, m_MonsterTransScale);
		//end 2010. 03. 18 by jskim ���ͺ��� ī��
		if(m_pCharacterInfo)
		{
			// 2010. 03. 18 by jskim ���ͺ��� ī��
			if(bMonsterTransformer)
			{
				m_pCharacterInfo->m_MonsterTransformer = m_MonsterTransformer;
				m_pCharacterInfo->m_MonsterTransScale  = m_MonsterTransScale;
 			}
			//end 2010. 03. 18 by jskim ���ͺ��� ī��
			m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
			m_pCharacterInfo->SetCharacterAnimationBodyConditionMask(BODYCON_CHARACTER_ANIMATION_TIME);
			m_pCharacterInfo->SetBodyCondition(nBodyCon);
			// 2006-01-13 by ispark
			if(bCharacter == FALSE && !bMonsterTransformer)
			{
			// 2010. 03. 18 by jskim ���ͺ��� ī��
				//SetWeaponPosition();
				SetPrimaryWeaponPosition();
				SetSecondaryWeaponPosition();
				SetPetPosition();
				SetMarkPosition(); // 2011-03-21 by jhAhn ��ũ�ý��� ��ũ ��ġ ����
			}
			//end 2010. 03. 18 by jskim ���ͺ��� ī��
		}
	}
}

int CUnitData::GetUnitNumFromCharacter(int nDefenseItemNum, int nUnitKind, BodyCond_t nBodyCon, BOOL bCharacter, int nDegree)
{
	int nTemp = 0;
//	if(m_dwPartType == _ADMIN)
//	{
//		nTemp = 10000000;
//	}
//	else
	{
		nTemp = ::GetUnitNum( nDegree, nDefenseItemNum, nUnitKind, bCharacter);
	}

	return nTemp;
	
}

HRESULT CUnitData::InitDeviceObjects()
{
	// skill effect ����
	m_pSkillEffect = new CSkillEffect(this);
	m_pSkillEffect->InitDeviceObjects();

	m_pChaffData.clear();

	return S_OK;
}

HRESULT CUnitData::RestoreDeviceObjects()
{
	// shadow
	RestoreShadow();
	// skill effect
	m_pSkillEffect->RestoreDeviceObjects();
	return S_OK;
}

HRESULT CUnitData::InvalidateDeviceObjects()
{
	// shadow
	InvalidateShadow();
	// skill effect
	m_pSkillEffect->InvalidateDeviceObjects();

	// ����
	vector<CItemData*>::iterator it = m_pChaffData.begin();
	while(it != m_pChaffData.end())
	{
		util::del(*it);
		it++;
	}
	m_pChaffData.clear();
	return S_OK;
}

HRESULT CUnitData::DeleteDeviceObjects()
{
 	m_pSkillEffect->DeleteDeviceObjects();
	util::del(m_pSkillEffect);

	vector<CItemData*>::iterator it = m_pChaffData.begin();
	while(it != m_pChaffData.end())
	{
		util::del(*it);
		it++;
	}
	m_pChaffData.clear();

	return S_OK;
}

HRESULT CUnitData::RestoreShadow()
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

HRESULT CUnitData::InvalidateShadow()
{
	SAFE_RELEASE(m_pVBShadow);
	return S_OK;
}

void CUnitData::SetPkState( DWORD dwPkFlag, BOOL bSet )
{
	if(bSet)
	{
		m_dwPkState |= dwPkFlag;
	}
	else
	{
		m_dwPkState &= ~dwPkFlag;
	}
#ifdef _DEBUG
	if(m_dwPartType == _SHUTTLE)
	{
		DBGOUT("ShuttleChild PK Setting(%d)(0x%08x)\n", bSet, m_dwPkState);
	}
	else if(m_dwPartType == _ENEMY)
	{
		DBGOUT("Enemy(%s) PK Setting(%d)(0x%08x)\n", ((CEnemyData*)this)->m_infoCharacter.CharacterInfo.CharacterName, bSet, m_dwPkState);
	}
#endif
}

BOOL CUnitData::IsPkState( DWORD dwPkFlag )
{
	if(m_dwPkState & dwPkFlag )
	{
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CUnitData::CheckAttack(ATTACK_DATA &attackData, BOOL bReset, ITEM* pPrimary)
/// \brief		1,2�� CheckAttack
/// \author		dhkwon
/// \date		2004-09-07 ~ 2004-09-07
/// \warning	����ġ���� �Լ�
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
//void CUnitData::CheckAttack(ATTACK_DATA &attackData, BOOL bReset, ITEM* pWeaponITEM)
void CUnitData::CheckAttack(ATTACK_DATA &attackData, BOOL bReset, ITEM* pWeaponITEM/* = NULL*/, ITEM* pEffectItem/* = NULL*/)
// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
{
	FLOG("CUnitData::CheckPrimaryAttack(BOOL bReset, ITEM* pPrimary)");

	if(pWeaponITEM)											//2013-06-03 by ssjung Canada �ͼ��� �����ڵ� �߰� 
		ASSERT_ASSERT(pWeaponITEM);

	if(bReset && attackData.bShootingWeapon==FALSE)
	{
		attackData.bShootingWeapon = TRUE;
		if(pWeaponITEM && IS_CLIENT_SET_AUTOMATIC_TIMER(pWeaponITEM->OrbitType) == FALSE)			//2013-06-03 by ssjung Canada �ͼ��� �����ڵ� �߰� 
		{
			attackData.nAttackCount = GetTotalShotNumPerReattackTime(pWeaponITEM, &m_paramFactor);
		}
	}
	if(pWeaponITEM && IS_CLIENT_SET_AUTOMATIC_TIMER(pWeaponITEM->OrbitType))						//2013-06-03 by ssjung Canada �ͼ��� �����ڵ� �߰� 
	{
		if( attackData.fCheckAutoMaticTimer <= 0.0f && 
			attackData.bShootingWeapon && 
			attackData.nAttackCount <= 0)
		{
			attackData.nAttackCount = GetTotalShotNumPerReattackTime(pWeaponITEM, &m_paramFactor);
			attackData.fCheckAutoMaticTimer = (float)CAtumSJ::GetReattackTime(pWeaponITEM, &m_paramFactor)/1000.0f;
		}
	}
	if( (attackData.nAttackCount>0 && 
		attackData.fCheckAttackTime <= 0.0f &&
		attackData.bShootingWeapon==TRUE) ||
//		((m_dwPartType == _ENEMY || m_dwPartType == _SHUTTLE ) && 
//		IS_SECONDARY_WEAPON(pWeaponITEM->Kind)==TRUE))
		(bReset && IS_SECONDARY_ATT_TYPE(attackData.AttackData.AttackType)))
	{
		
		// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
		// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
		//CreateWeapon( attackData, pWeaponITEM );
		int temp = _BULLET_PRIORITY;
		if(g_pShuttleChild->GetUnitNum() == m_nUnitNum)
		{
			temp = _MY_CHARACTER_PRIORITY;
		}		
		//CreateWeapon( attackData, pWeaponITEM, pEffectItem );
		// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
		CreateWeapon( attackData, pWeaponITEM, pEffectItem, temp);
		//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

		attackData.nAttackCount--;
		if(attackData.nMultiNumCount>0)
		{
			attackData.nMultiNumCount--;
		}
		if(attackData.nAttackCount == 0)
		{
			attackData.bShootingWeapon = FALSE;
		}
		if(attackData.nMultiNumCount <= 0)
		{
			if(pWeaponITEM)							//2013-06-03 by ssjung Canada �ͼ��� �����ڵ� �߰� 
			{	
				if(IS_CLIENT_SET_AUTOMATIC_TIMER(pWeaponITEM->OrbitType))
				{
					attackData.fCheckAttackTime = GetAutomaticAttackTime(pWeaponITEM->OrbitType);
				}
				else
				{
					attackData.fCheckAttackTime = CAtumSJ::GetShotCountReattackTime( pWeaponITEM, &m_paramFactor );
				}
				attackData.nMultiNumCount = GetMultiNum( pWeaponITEM, &m_paramFactor );
			}
		}
	}
}
 

///////////////////////////////////////////////////////////////////////////////
/// \fn			CUnitData::GetTotalShotNumPerReattackTime(ITEM* pWeaponITEM, CParamFactor *pParamFactor)
/// \brief		ReattackTime ���� �߻�Ǵ� �߻� ��(����)
/// \author		dhkwon
/// \date		2004-09-07 ~ 2004-09-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CUnitData::GetTotalShotNumPerReattackTime(ITEM* pWeaponITEM, CParamFactor *pParamFactor)
{
	if(IS_PRIMARY_WEAPON(pWeaponITEM->Kind))
		
		return CAtumSJ::GetTotalPrimaryShotNumPerReattackTime(pWeaponITEM, pParamFactor);

	else return CAtumSJ::GetTotalSecondaryShotNumPerReattackTime(pWeaponITEM, pParamFactor);
}

int CUnitData::GetMultiNum(ITEM* pWeaponITEM, CParamFactor *pParamFactor)
{
	if(IS_PRIMARY_WEAPON(pWeaponITEM->Kind))
	{
		return pWeaponITEM->MultiNum + pParamFactor->pfp_MULTINUM_01;
	}
	// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����
// 	else(
// 	{
// 		return pWeaponITEM->MultiNum + pParamFactor->pfp_MULTINUM_02;
// 	}
	else if(IS_SECONDARY_WEAPON(pWeaponITEM->Kind))
	{
		return pWeaponITEM->MultiNum + pParamFactor->pfp_MULTINUM_02;
	}
	else if(pWeaponITEM->Kind == ITEMKIND_PET_ITEM)
	{
		return pWeaponITEM->MultiNum;
	}
	//end 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CUnitData::SetWeaponVelFromServer( D3DXVECTOR3 vWeaponVel )
/// \brief		Enemy,(Monster)�� WeaponVel����
/// \author		dhkwon
/// \date		2004-10-18 ~ 2004-10-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::SetWeaponVelFromServer( D3DXVECTOR3 vWeaponVel )
{
	m_vWeaponVel = vWeaponVel;
}
// 2010-06-15 by shcho&hslee ��ý��� - ���� ��ġ
void CUnitData::SetPetVelFromServer( D3DXVECTOR3 vPetLeftVel, D3DXVECTOR3 vPetRightVel )
{
	m_vPetLeftVel = vPetLeftVel;
	m_vPetRightVel = vPetRightVel;
}
// end 2010-06-15 by shcho&hslee ��ý��� - ���� ��ġ
// 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
void	CUnitData::ShowWeapon( const BOOL bShow )
{
	if( m_pWeapon1_1_1 )
		m_pWeapon1_1_1->m_bRender	= bShow;

	if( m_pWeapon1_1_2 )
		m_pWeapon1_1_2->m_bRender	= bShow;

	if( m_pWeapon1_2 )
		m_pWeapon1_2->m_bRender		= bShow;

	if( m_pWeapon2_1_1 )
		m_pWeapon2_1_1->m_bRender	= bShow;

	if( m_pWeapon2_1_2 )
		m_pWeapon2_1_2->m_bRender	= bShow;

	if( m_pWeapon2_2 )
		m_pWeapon2_2->m_bRender		= bShow;
}
// 2010-06-15 by shcho&hslee ��ý��� - ���� ��ġ
void CUnitData::HidePet( const BOOL bhide)
{
	if(m_pPartner)
	{
		m_pPartner->m_bRender		= !bhide;		
	}
	if(m_pDummyPartner)
	{
		m_pDummyPartner->m_bRender	= !bhide;
	}
	if(m_pPartner1)
	{
		m_pPartner1->m_bRender		= !bhide;		
	}
	if(m_pDummyPartner1)
	{
		m_pDummyPartner1->m_bRender	= !bhide;
	}
}
// end 2010-06-15 by shcho&hslee ��ý��� - ���� ��ġ

void	CUnitData::CreateRobotAGearWeapon( ITEM* pAGearWeapon, const UINT nUnitType )
{
	DeleteRobotAGearWeapon();

	m_pRobotAGearWeapon_1 = CreateWearItemEffect( WEAR_ITEM_KIND_WEAPON_11, 
												  EFFECT_NUMBER_WEAR_WEAPON_11( pAGearWeapon, nUnitType ),
												  D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
												  WEAPON_BODYCON_LOADING,
												  WEAPON_POSITION_1_1_1 );
}

void	CUnitData::DeleteRobotAGearWeapon( void )
{
	DeleteWearItemEffect( m_pRobotAGearWeapon_1 );
	m_pRobotAGearWeapon_1 = NULL;
}
// end 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)

///////////////////////////////////////////////////////////////////////////////
/// \fn			CEnemyData::DelSecondWeapon()
/// \brief		�׾��� �� �ڽ��� 2������ Ÿ�� ����(����, ���� �ش�)
/// \author		ispark
/// \date		2005-12-14 ~ 2005-12-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::DelSecondWeapon()
{
	ClientIndex_t AttackIndex = 0; 
	if(m_dwPartType == _ENEMY)
	{
		AttackIndex = ((CEnemyData*)this)->m_infoCharacter.CharacterInfo.ClientIndex;
	}
	else if(m_dwPartType == _MONSTER)
	{
		AttackIndex = ((CMonsterData*)this)->m_info.MonsterIndex;
	}
	else
	{
		// �� �ش������ �ƴϹǷ� ����
		return;
	}
	
	if(!g_pScene->m_pWeaponData)
		return;
	CWeapon * pNode = (CWeapon *)g_pScene->m_pWeaponData->m_pChild;
	while(pNode)
	{
		if( (pNode->m_dwPartType == _MISSILE || pNode->m_dwPartType == _ROCKET ) &&
			AttackIndex == ((CWSlowData*)pNode)->m_nClientIndex)
		{
			((CWSlowData*)pNode)->m_pTarget = NULL;
		}

		pNode = (CWeapon *)pNode->m_pNext;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CUnitData::ResetPrimaryWeaponBodyCondition(BodyCond_t hyBodyCon, BOOL bZig)
/// \brief		������� 1�� ���� ����
/// \author		ispark
/// \date		2006-01-03 ~ 2006-01-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ResetPrimaryWeaponBodyCondition(BodyCond_t hyBodyCon, BOOL bZig)
{
	FLOG("CUnitData::ResetPrimaryWeaponBodyCondition( BodyCond_t hyBodyCon)");
	if(m_pWeapon1_1_1 && m_pWeapon1_1_1->m_pCharacterInfo && bZig == TRUE)
	{
		m_pWeapon1_1_1->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon1_1_1->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
//		DBGOUT("CUnitData::ResetPrimaryWeaponBodyCondition( %016I64X )\n", hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(m_pWeapon1_1_2 && m_pWeapon1_1_2->m_pCharacterInfo && bZig == FALSE)
	{
		m_pWeapon1_1_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon1_1_2->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
//		DBGOUT("CUnitData::ResetPrimaryWeaponBodyCondition( %016I64X )\n", hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
//	if(m_pWeapon1_2 && m_pWeapon1_2->m_pCharacterInfo && bZig == FALSE)
//	{
//		m_pWeapon1_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
//		m_pWeapon1_2->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
//	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ChangeWearItemBodyCondition(int nWearItemKind, BodyCond_t hyBodyCondition)
/// \brief		1��, 2�� ���⸦ ������ �ٸ� ������ �ٵ������ ü����
/// \author		ispark
/// \date		2006-06-28 ~ 2006-06-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ChangeWearItemBodyCondition(int nWearItemKind, BodyCond_t hyBodyCondition)
{
	switch( nWearItemKind )
	{
	case WEAR_ITEM_KIND_PROW:
		{
		}
		break;
	case WEAR_ITEM_KIND_CENTER:
		{
		}
		break;
	case WEAR_ITEM_KIND_ACCESSORY_UNLIMITED:
		{
			if(m_pContainer)
			{
				m_pContainer->ChangeBodyCondition(hyBodyCondition);
			}
		}
		break;
	case WEAR_ITEM_KIND_ENGINE:
		{
		}
		break;
	case WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT:
		{
			if(m_pAccessories)
			{
				m_pAccessories->ChangeBodyCondition(hyBodyCondition);
			}
		}
		break;
	case WEAR_ITEM_KIND_WINGIN:
		{
			if(m_pWingIn)
			{
				m_pWingIn->ChangeBodyCondition(hyBodyCondition);
			}
		}
		break;
	// 2010-06-08 by dgwoo, ��ý��� �߰�. 
	case WEAR_ITEM_KIND_PET:
		{
			if(m_pPartner && !COMPARE_BODYCON_BIT( m_pPartner->m_bodyCondition, hyBodyCondition ) )
			{
				m_pPartner->ChangeBodyCondition(hyBodyCondition);
			}
			if(m_pPartner1 && !COMPARE_BODYCON_BIT( m_pPartner1->m_bodyCondition, hyBodyCondition ) )
			{
				m_pPartner1->ChangeBodyCondition(hyBodyCondition);
			}
			if(m_pDummyPartner)
			{
				m_pDummyPartner->ChangeBodyCondition(hyBodyCondition);
			}
			if(m_pDummyPartner1)
			{
				m_pDummyPartner1->ChangeBodyCondition(hyBodyCondition);
			}
		}
		break;
	// 2010-06-08 by dgwoo, ��ý��� �߰�. 
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::SetUnitAlpha(BYTE bySkillState, int nAlphaValue)
/// \brief		���İ� ó��
/// \author		ispark
/// \date		2006-11-21 ~ 2006-11-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::SetUnitAlpha(BYTE bySkillState, int nAlphaValue)
{
	m_bySkillStateFlag = bySkillState;
	m_nAlphaValue = nAlphaValue;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::InitUnitAlpha()
/// \brief		���İ� �ʱ�ȭ
/// \author		ispark
/// \date		2006-11-21 ~ 2006-11-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::InitUnitAlpha()
{
	m_bySkillStateFlag = CL_SKILL_NONE;
	m_nAlphaValue = SKILL_OBJECT_ALPHA_NONE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::AddChaffData(CAtumNode* pUnitData, MSG_FC_BATTLE_DROP_DUMMY_OK* pMsg)
/// \brief		ü�� �߰�
/// \author		ispark
/// \date		2006-12-04 ~ 2006-12-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::AddChaffData(CAtumNode* pUnitData, MSG_FC_BATTLE_DROP_DUMMY_OK* pMsg)
{
	UINT *pChaffIndex = (UINT *)((char *)pMsg + sizeof(MSG_FC_BATTLE_DROP_DUMMY_OK));
	for(int i = 0 ; i < pMsg->DummyCounts ; i++)
	{
		//CItemData *pChaff = FindChaffDataByFieldIndex( pMsg->ItemFieldIndex );
		CItemData *pChaff = FindChaffDataByFieldIndex(*pChaffIndex);
		if(!pChaff)
		{
			pChaff = new CItemData(pUnitData, pMsg->ItemNum,*pChaffIndex);
			m_pChaffData.push_back(pChaff);
		}
		pChaffIndex++;
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::DelChaffData(UINT nItemIndex)
/// \brief		ü�� ����
/// \author		ispark
/// \date		2006-12-04 ~ 2006-12-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::DelChaffData(UINT nItemIndex)
{
	vector<CItemData*>::iterator it = m_pChaffData.begin();
	while(it != m_pChaffData.end())
	{
		if((*it)->m_nItemIndex == nItemIndex)
		{
			util::del(*it);
			m_pChaffData.erase(it);
			
			return;
		}
		it++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::DelChaffDataAll()
/// \brief		ü���� ��� �����Ѵ�.
/// \author		dgwoo
/// \date		2007-06-21 ~ 2007-06-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::DelChaffDataAll()
{
	vector<CItemData*>::iterator it = m_pChaffData.begin();
	while(it != m_pChaffData.end())
	{
		util::del(*it);
		it = m_pChaffData.erase(it);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CItemData* CUnitData::FindChaffDataByFieldIndex( UINT nFieldIndex )
/// \brief		ü�� �ε��� �˻�
/// \author		ispark
/// \date		2006-12-04 ~ 2006-12-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CItemData* CUnitData::FindChaffDataByFieldIndex( UINT nFieldIndex )
{
	vector<CItemData*>::iterator it = m_pChaffData.begin();
	while(it != m_pChaffData.end())
	{
		if((*it)->m_nItemIndex == nFieldIndex)
			return *it;

		it++;
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ChaffDataTick()
/// \brief		
/// \author		ispark
/// \date		2006-12-04 ~ 2006-12-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ChaffDataTick()
{
	vector<CItemData*>::iterator it = m_pChaffData.begin();
	while(it != m_pChaffData.end())
	{
		(*it)->Tick();
		it++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int	CUnitData::GetChaffDataCound()
/// \brief		
/// \author		ispark
/// \date		2006-12-04 ~ 2006-12-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int	CUnitData::GetChaffDataCound()
{
	return m_pChaffData.size();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			FLOAT CUnitData::GetSkillChgTime()
/// \brief		
/// \author		// 2007-04-02 by bhsohn Move��Ŷ���� ���� ���� üũ
/// \date		2007-04-02 ~ 2007-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
FLOAT CUnitData::GetSkillChgTime(int nIdx)
{
	if(nIdx>= UNIT_STATE_MAX || nIdx < 0)
	{
		return 0 ;
	}
	return m_fUnitStateChgCap[nIdx];	
}

void CUnitData::AddSkillChgTime(int nIdx, FLOAT fUnitStateChgCap)
{
	if(nIdx>= UNIT_STATE_MAX || nIdx < 0)
	{
		return;
	}
	if(fUnitStateChgCap < 0)
	{
		// ���� �ʱ�ȭ 
		m_fUnitStateChgCap[nIdx] = 0;		
	}
	else
	{
		m_fUnitStateChgCap[nIdx] += fUnitStateChgCap;	
	}	
}

bool CUnitData::IsUnitState(int nIdx)
{
	if(nIdx>= UNIT_STATE_MAX || nIdx < 0)
	{
		return false;
	}
	if(m_byUnitState & (1 << nIdx))
	{
		return true;
	}
	return false;
}

void CUnitData::SetUnitSkillState(int nIdx, bool bOnOff)
{
	if(nIdx>= UNIT_STATE_MAX || nIdx < 0)
	{
		return ;
	}
	if(bOnOff)
	{
		m_byUnitState |= (1<< nIdx);
	}
	else
	{
		m_byUnitState &= ~(1<< nIdx);
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::IsCheckObjectShow()
/// \brief		������Ʈ �浹 ���� ���� �Ǵ� 
/// \author		// 2007-05-17 by bhsohn ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��	
/// \date		2007-05-17 ~ 2007-05-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CUnitData::IsCheckObjectShow()
{
	return TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CUnitData::IsCheckObjectColl()
/// \brief		������Ʈ �浹 ���� ���� �Ǵ� 
/// \author		// 2007-05-17 by bhsohn ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��	
/// \date		2007-05-17 ~ 2007-05-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CUnitData::IsCheckObjectColl()
{
	return m_bCheckObject;
}

// 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
// 2013-01-08 by jhjang �κ��Ƹӿ� ���� ���� Ŷ�� ��������� ������ �߻��ϴ� �κ� ����
BOOL	CUnitData::IsRobotArmor_Old( void ) // 2013-01-08 by jhjang ���� �κ� �Ƹ� üũ �Լ�
{
	// �Ƹ� �������� ������ �����ۿ� �κ��Ƹ� �÷��װ� ������ true, �ƴϸ� false
	BOOL bRobotAmor = FALSE;
	if( m_dwPartType == _SHUTTLE )
	{
		CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find( POS_CENTER );
		if( it != g_pStoreData->m_mapItemWindowPosition.end()
			&& COMPARE_BIT_FLAG(it->second->ItemInfo->ItemAttribute, ITEM_ATTR_ROBOT_ARMOR )
			// 2010. 03. 18 by jskim ���ͺ��� ī��
			&& !m_MonsterTransformer )
			//end 2010. 03. 18 by jskim ���ͺ��� ī��
			bRobotAmor = TRUE;
	}
	else if( m_dwPartType == _ENEMY )
	{
		ITEM* pItem = g_pDatabase->GetServerItemInfo( ((CEnemyData *)this)->m_infoCharacter.CharacterRenderInfo.RI_Center );
		if( pItem && COMPARE_BIT_FLAG( pItem->ItemAttribute, ITEM_ATTR_ROBOT_ARMOR ) 
			// 2010. 03. 18 by jskim ���ͺ��� ī��
			&& !m_MonsterTransformer )
			//end 2010. 03. 18 by jskim ���ͺ��� ī��
			bRobotAmor = TRUE;
	}

	return bRobotAmor;
}
// end 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)

BOOL	CUnitData::IsRobotArmor( void ) // 2013-01-08 by jhjang ���� ���� ���� �κ� �Ƹ� üũ �Լ�
{
	// �Ƹ� �������� ������ �����ۿ� �κ��Ƹ� �÷��װ� ������ true, �ƴϸ� false
	BOOL bRobotAmor = FALSE;
	ITEM* pItem = NULL;

	if( m_dwPartType == _SHUTTLE )
	{
		CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find( POS_CENTER );
		if( it != g_pStoreData->m_mapItemWindowPosition.end() )
		{
			CItemInfo *pItemInfo = it->second;
			if(pItemInfo->ShapeItemNum)
				pItem = g_pDatabase->GetServerItemInfo(pItemInfo->ShapeItemNum);
			else
				pItem = it->second->ItemInfo;
		}
	}
	else if( m_dwPartType == _ENEMY )
	{
		if(((CEnemyData *)this)->m_infoCharacter.CharacterRenderInfo.RI_Center_ShapeItemNum)
			pItem = g_pDatabase->GetServerItemInfo( ((CEnemyData *)this)->m_infoCharacter.CharacterRenderInfo.RI_Center_ShapeItemNum );
		else
			pItem = g_pDatabase->GetServerItemInfo( ((CEnemyData *)this)->m_infoCharacter.CharacterRenderInfo.RI_Center );
	}
	//ITEM_GENERAL* pGeneralItem = 
	if( pItem && COMPARE_BIT_FLAG( pItem->ItemAttribute, ITEM_ATTR_ROBOT_ARMOR ) 
		&& !m_MonsterTransformer )// 2010. 03. 18 by jskim ���ͺ��� ī��
	{
		bRobotAmor = TRUE;
	}

	return bRobotAmor;
}
// end 2013-01-08 by jhjang �κ��Ƹӿ� ���� ���� Ŷ�� ��������� ������ �߻��ϴ� �κ� ����
