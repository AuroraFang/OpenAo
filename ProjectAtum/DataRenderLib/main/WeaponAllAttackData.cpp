// WeaponAllAttackData.cpp: implementation of the CWeaponAllAttackData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WeaponAllAttackData.h"
#include "UnitData.h"
#include "CharacterInfo.h"
#include "ItemData.h"
#include "SceneData.h"
#include "AtumApplication.h"
#include "AtumDatabase.h"
#include "ShuttleChild.h"
#include "EnemyData.h"
#include "dxutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// \fn			CWeaponAllAttackData , Construction
/// \brief		
/// \author		dhkwon
/// \date		2004-06-26 ~ 2004-06-26
/// \warning	
///									  
/// \param		
///				CAtumNode * pTarget : NULL�̸� ���� ����Ʈ
///									  pTarget->m_dwPartType == _ADMIN,_SHUTTLE, _ENEMY, _MONSTER �̸� ���� Ÿ�� ����Ʈ
///										pTarget->m_bShielding == TRUE �̸� ���� ����Ʈ ����
///									  pTarget->m_dwPartType == _DUMMY ������ Ÿ�� ����Ʈ
/// \return		
///////////////////////////////////////////////////////////////////////////////

// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
// CWeaponAllAttackData::CWeaponAllAttackData(CAtumData * pAttackter, 
// 										   CAtumData * pTarget, 
// 										   BodyCond_t nBodyCondition, 
// 										   UINT nItemNum)

// CWeaponAllAttackData::CWeaponAllAttackData( CAtumData * pAttackter,
// 										    CAtumData * pTarget,
// 											BodyCond_t nBodyCondition,
// 											UINT nItemNum,
// 											ITEM* pEffectItem /* = NULL */ )
// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
 CWeaponAllAttackData::CWeaponAllAttackData( CAtumData * pAttackter,
 										    CAtumData * pTarget,
 											BodyCond_t nBodyCondition,
 											UINT nItemNum,
 											ITEM* pEffectItem, /* = NULL */ 
											int LoadingPriority )
//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
{
	m_pAttacker = pAttackter;
	m_pTarget = pTarget;
	m_dwWeaponState = _NORMAL;
	m_pItemData = NULL;
	m_bodyCondition = nBodyCondition;//	��ü������ BODYCON_FIRE_MASK, BODYCON_HIT_MASK ���� �ϳ�
	m_pItemData = g_pDatabase->GetServerItemInfo(nItemNum);
	if(!m_pItemData)
	{
		DBGOUT("omi.tex error ITEM:%d\n", nItemNum);
		g_pD3dApp->NetworkErrorMsgBox(STRERR_C_RESOURCE_0001);
		return;
	}

	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	m_pEffectItem = pEffectItem;
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

// ����Ʈ �ʱ�ȭ
	char buf[64];
	// 2005-08-23 by ispark
	ITEM* pItem = g_pDatabase->GetServerItemInfo(nItemNum);

	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	// ����Ʈ �������� �ִٸ� �װ����� ����
	if( m_pEffectItem )
		pItem = m_pEffectItem;
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

//	wsprintf(buf,"%08d",nItemNum);
	wsprintf(buf,"%08d",pItem->SourceIndex);
	m_pCharacterInfo = new CCharacterInfo();
	if(m_pCharacterInfo->Load(buf) == FALSE)
	{
		DBGOUT("effectinfo.inf error:%d\n", nItemNum);
		util::del(m_pCharacterInfo);
//		g_pD3dApp->NetworkErrorMsgBox(STRERR_C_RESOURCE_0001);
		return;
	}
	m_pCharacterInfo->InitDeviceObjects();
	m_pCharacterInfo->RestoreDeviceObjects();
	// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
 	m_pCharacterInfo->m_LoadingPriority = LoadingPriority;
 	//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
	m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
	m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);

// �ڽ� Ÿ�� ����Ʈ ����
	if(m_pTarget == NULL && m_bodyCondition == BODYCON_FIRE_MASK)
	{
		//2011-07-08 by jhahn ����3�� �������� ��
		//if(D3DXVec3Length( &(pAttackter->m_vPos - g_pShuttleChild->m_vPos) ) < m_pItemData->Range )
		if(D3DXVec3Length( &(pAttackter->m_vPos - g_pShuttleChild->m_vPos) ) < m_pItemData->ExplosionRange )
		//end 2011-07-08 by jhahn ����3�� �������� ����
		{
			CreateHitEffect( g_pShuttleChild );
		}
		CMapEnemyIterator it = g_pScene->m_mapEnemyList.begin();
		while(it != g_pScene->m_mapEnemyList.end())
		{
			CEnemyData* pEnemy = it->second;
			
			//2011-07-08 by jhahn �������� ����
	//		if( D3DXVec3Length( &(pAttackter->m_vPos - pEnemy->m_vPos) ) < m_pItemData->Range )
			if( D3DXVec3Length( &(pAttackter->m_vPos - pEnemy->m_vPos) ) < m_pItemData->ExplosionRange )
			//end 2011-07-08 by jhahn �������� ����
			{
				CreateHitEffect( pEnemy );
			}
			it++;
		}
	}
// ����Ʈ ��ġ ����
	if(m_pTarget && 
		m_pTarget->m_dwPartType <= _MONSTER ) //_ADMIN,_SHUTTLE, _ENEMY, _MONSTER
	{
		m_vVel = m_pTarget->m_vPos - m_pAttacker->m_vPos;
		D3DXVec3Normalize(&m_vVel,&m_vVel);
		// �ڽ�(Ÿ��) ����Ʈ
		if(!m_pTarget->m_bShielding)
		{
			// �����ϰ� ��ġ�� �����Ѵ�.
			D3DXVECTOR3 vVel;
			D3DXVec3Normalize(&vVel, &(m_pAttacker->m_vPos - m_pTarget->m_vPos));
			vVel = D3DXVECTOR3( vVel.x*((float)(RANDI(0, SIZE_OF_PRIMARY_ATTACK_RANDOM_DISTANCE - 1))),
								vVel.y*((float)(RANDI(0, SIZE_OF_PRIMARY_ATTACK_RANDOM_DISTANCE - 1))),
								vVel.z*((float)(RANDI(0, SIZE_OF_PRIMARY_ATTACK_RANDOM_DISTANCE - 1))));
			m_vPos = m_pTarget->m_vPos + vVel;
		}
		else
		{// ���� Hit ����Ʈ
			D3DXVECTOR3 vVel;
			D3DXVec3Normalize(&vVel, &(m_pAttacker->m_vPos - m_pTarget->m_vPos));
			m_vPos = m_pTarget->m_vPos + vVel*SIZE_OF_SHIELD_EFFECT;
			((CUnitData*)m_pTarget)->CreateSecondaryShieldDamage(m_pAttacker->m_vPos);
		}
		DBGOUT("CWeaponAllAttackData Child Create\n");
	}
	else
	{
		// ���� ����Ʈ
		m_vVel = m_pAttacker->m_vVel;
		m_vPos = m_pAttacker->m_vPos;
		DBGOUT("CWeaponAllAttackData Create\n");
	}
	m_vUp = D3DXVECTOR3(0,1,0);
	D3DXMatrixLookAtRH( &m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
}

CWeaponAllAttackData::~CWeaponAllAttackData()
{
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		util::del(m_pCharacterInfo);
	}
	DBGOUT("CWeaponAllAttackData Delete\n");
}

void CWeaponAllAttackData::Tick()
{
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
		m_pCharacterInfo->Tick(g_pD3dApp->GetElapsedTime());
		if(m_pCharacterInfo->IsUsing() == FALSE )
		{
			m_bUsing = FALSE;
		}
	}
}

void CWeaponAllAttackData::CreateHitEffect(CAtumData* pTarget)
{
	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
// 	CWeaponAllAttackData* pData = new CWeaponAllAttackData( m_pAttacker, 
// 															pTarget, 
// 															BODYCON_HIT_MASK, 
// 															m_pItemData->ItemNum);

	CWeaponAllAttackData* pData = new CWeaponAllAttackData( m_pAttacker,
															pTarget,
															BODYCON_HIT_MASK,
															m_pItemData->ItemNum,
															m_pEffectItem );
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

	g_pScene->m_pWeaponData->AddChild( pData );
}
