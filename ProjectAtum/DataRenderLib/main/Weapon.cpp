// Weapon.cpp: implementation of the CWeapon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Weapon.h"
#include "AtumApplication.h"
//#include "TraceData.h"
#include "AtumData.h"
#include "ShuttleChild.h"
#include "AtumSound.h"
#include "SkillEffect.h"
#include "EnemyData.h"
#include "dxutil.h"
#include "PkNormalTimer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWeapon::CWeapon()
{
	FLOG( "CWeapon()" );
	m_pAttacker = NULL;
	m_pTarget = NULL;						// Position
	m_vPos = D3DXVECTOR3(0,0,0);						// Position
	m_vVel = D3DXVECTOR3(0,0,0);						// ����
	m_vUp = D3DXVECTOR3(0,0,0);						// Position
//	m_mMatrix;
//	m_mFireMatrix;
	m_vTargetPos = D3DXVECTOR3(0,0,0);				// Target Position
	m_vStartPos = D3DXVECTOR3(0,0,0);
	m_dwWeaponState = _NORMAL;			// Weapon�� ���� : 0:normal, 1:exploding

	m_fWeaponLifeTime = 0.0f;
	m_fWeaponLifeCheckTime = 0.0f;
	m_bodyCondition = 0;
	m_pCharacterInfo = NULL;
	m_bSetTarget = FALSE;
	m_bWeaponFlyType = 0;

	m_pItemData = NULL;

	m_vFirePos = D3DXVECTOR3(0,0,0);
	m_nSkillNum = 0;
//	m_pTraceList1 = NULL;
//	m_pTraceList2 = NULL;
	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	m_nDelegateClientIdx	= 0;
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
}

CWeapon::~CWeapon()
{
	FLOG( "~CWeapon()" );
	if(m_pCharacterInfo)//�߰�
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		util::del(m_pCharacterInfo);
	}
//	util::del(m_pTraceList1);
//	util::del(m_pTraceList2);
}

void CWeapon::SetShuttleChildOrderTarget()	// ���� ���� �޾��� ��
{
	if(m_pTarget == (CAtumData*)g_pShuttleChild &&
		g_pShuttleChild->m_pOrderTarget == NULL &&
		g_pShuttleChild->m_bySkillStateFlag != CL_SKILL_INVISIBLE)		// �κ����� ��ų ����� �ƴ϶��
	{
		// 2004-12-15 by jschoi - PK���ɸ��̰ų� PK���϶�(PVP,FVF,GVG)
		if(	IsPkEnableMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) ||
			((CUnitData*)m_pAttacker)->IsPkEnable())
		{
			g_pShuttleChild->m_pOrderTarget = m_pAttacker;
			g_pShuttleChild->m_pPkNormalTimer->SetImmediatePk();
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_LOCKON_PRIMARY, g_pShuttleChild->m_vPos, FALSE);
		}
	}
}


void CWeapon::LoadCharacterEffect(char* strFileName)
{
	FLOG( "CWeapon::LoadCharacterEffect(char* strFileName)" );
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		util::del(m_pCharacterInfo);
	}
	m_pCharacterInfo = new CCharacterInfo();
	if(m_pCharacterInfo->Load(strFileName))
	{
		m_pCharacterInfo->InitDeviceObjects();
		m_pCharacterInfo->RestoreDeviceObjects();
	}
	else
	{
		util::del(m_pCharacterInfo);
		DBGOUT("CWeapon::LoadCharacterEffect,: Can't Find Effect File : %s.\n", strFileName);
	}

}

void CWeapon::SetBodyCondition(BodyCond_t hyBodyCondition)
{
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->ChangeBodyCondition(hyBodyCondition);
	}
}

#define MAX_MISSILE_TURN_RATE	10.0f
#define MAX_MISSILE_TURN_ANGLE	(1.0f/2.0f)

void CWeapon::MoveWeapon(BYTE bType)
{
	FLOG( "CWeapon::MoveWeapon(BYTE bType)" );
	float fElapsedTime = g_pD3dApp->GetElapsedTime();
	D3DXVECTOR3 vVel,vSide,vUp;
	D3DXMATRIX mat;
	float fAngle;
	switch(bType)
	{
	case WEAPON_FLYTYPE_STRAIGHT1X:
		{// �������� ���� ����
			m_vPos += m_fWeaponSpeed*m_vVel*fElapsedTime;
		}
		break;
	case WEAPON_FLYTYPE_STRAIGHT2X:
		{// �������� ����(2��) ����
			m_vPos += 2.0f*m_fWeaponSpeed*m_vVel*fElapsedTime;
		}
		break;
	case WEAPON_FLYTYPE_DOWN:
		{// �Ʒ��� �ϰ� ����
			m_vPos.y -= m_fWeaponSpeed*fElapsedTime;
		}
		break;
	case WEAPON_FLYTYPE_SIN1X:
		{// �ϰ��Ͽ� Ÿ������ ���ư� - Sin �
			D3DXVec3Normalize(&vVel,&(m_vTargetPos - m_vStartPos));
			m_vPos = m_vStartPos + vVel*m_fWeaponLifeCheckTime*m_fWeaponSpeed;
			m_vPos.y +=	0.02f*m_fTargetDistance*sin(3.14f + 3.14f*(D3DXVec3Length(&(m_vPos-m_vTargetPos))/m_fTargetDistance));
		}
		break;
	case WEAPON_FLYTYPE_SIN2X:
		{// �ϰ��Ͽ��ٰ� ����Ͽ� Ÿ������ ���ư� - Sin �
			D3DXVec3Normalize(&vVel,&(m_vTargetPos - m_vStartPos));
			m_vPos = m_vStartPos + vVel*m_fWeaponLifeCheckTime*m_fWeaponSpeed;
			m_vPos.y +=	0.02f*m_fTargetDistance*sin(2.0f*(3.14f + 3.14f*(D3DXVec3Length(&(m_vPos-m_vTargetPos))/m_fTargetDistance)));
		}
		break;
	case WEAPON_FLYTYPE_DOWNSTRAIGHT:
		{// ������(0.5f) �ϰ��ϴٰ� �������� Ÿ������ ����
			if(m_fWeaponLifeCheckTime > 0.8f)
			{
				if(!m_bSetTarget)
				{
					m_bSetTarget = TRUE;
					m_vStartPos = m_vPos;
					if(m_pTarget)
					{
						m_vTargetPos = m_pTarget->m_vPos;
						D3DXVec3Normalize(&m_vVel,&(m_vTargetPos - m_vPos));
					}
					else
					{
						D3DXVec3Normalize(&m_vVel,&(m_vTargetPos - m_vPos));
					}
					D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
					D3DXVec3Cross(&m_vUp,&m_vVel,&vSide);
					m_fTargetDistance = D3DXVec3Length(&(m_vPos - m_vTargetPos));
				}
				m_vPos += m_fWeaponSpeed*m_vVel*fElapsedTime;
			}
			else
			{
				if(m_pTarget)
				{
					D3DXVec3Normalize(&m_vVel,&(m_pTarget->m_vPos - m_vPos));
				}
				m_vPos += 0.05f*m_fWeaponLifeCheckTime*m_fWeaponSpeed*m_vVel*fElapsedTime;
				m_vPos -= 0.05f*m_fWeaponLifeCheckTime*m_fWeaponSpeed*m_vUp*fElapsedTime;
			}
		}
		break;
	case WEAPON_FLYTYPE_DOWNGUIDE:
		{// Ÿ���� �ְ� Ÿ���� �̻��Ϻ��� ��ǥ�� ���������� ���ݿ� �������� ���� �°� �ȴ�.
			if(m_pTarget && m_pTarget->m_vPos.y < m_vPos.y)
			{
				D3DXVec3Normalize(&vVel,&(m_pTarget->m_vPos - m_vPos));
				D3DXVec3Normalize(&m_vVel,&(m_vVel + vVel*fElapsedTime));
			}
			m_vPos += m_fWeaponSpeed*m_vVel*fElapsedTime;
		}
		break;
	case WEAPON_FLYTYPE_GUIDEBASIC:
		{// �⺻ ���� �̻���-Ÿ������ �����Ǹ鼭 �ٰ�����. �־����� �ӵ� ����
			if(m_bSetTarget)
			{
				D3DXVec3Normalize(&vVel,&(m_vTargetPos - m_vPos));
				D3DXVec3Cross(&vSide,&m_vVel,&vVel);
				fAngle = ACOS(D3DXVec3Dot(&vVel,&m_vVel));
				if(fAngle > MAX_MISSILE_TURN_ANGLE)
					fAngle = MAX_MISSILE_TURN_ANGLE;
				float fCheck = 5.0f*fAngle*fElapsedTime;
				if(fCheck > fAngle)
				{
					fCheck = fAngle;
				}
				D3DXMatrixRotationAxis(&mat,&vSide,fCheck);
				D3DXVec3TransformCoord(&m_vVel, &m_vVel, &mat);
//				D3DXVec3Normalize(&m_vVel,&(MAX_MISSILE_TURN_RATE*vVel*fElapsedTime+m_vVel));
			}
			m_vPos += m_fWeaponSpeed*m_vVel*fElapsedTime;
			m_fWeaponSpeed += 60.0f*fElapsedTime;
/*			if(m_pTarget && m_pTarget->m_dwState == _NORMAL)
			{
				// ��ǥ���� �־����������� ���� Vel�� �ٿ��ش�
				if(m_fWeaponSpeed > 0.0f)
				{
					if(D3DXVec3Length(&(m_vPos - m_pTarget->m_vPos)) > D3DXVec3Length(&(m_vOldPos - m_pTarget->m_vPos)))
					{
						m_fWeaponSpeed -= 70.0f*fElapsedTime;
					}
					else
					{
						if(m_fWeaponSpeed < 400.0f)
						{
							m_fWeaponSpeed += 60.0f*fElapsedTime;
						}
					}
				}
				else
				{
					if(m_fWeaponSpeed < 400.0f)
					{
						m_fWeaponSpeed += 60.0f*fElapsedTime;
					}
				}
			}
			else
			{
				if(m_fWeaponSpeed < 400.0f)
				{
					m_fWeaponSpeed += 50.0f*fElapsedTime;
				}
			}
*/
		}
		break;
	case WEAPON_FLYTYPE_GUIDETWIST:
		{// ���� �̻��� - ����������� ����
			if(m_bSetTarget)
			{
				D3DXVec3Normalize(&vVel,&(m_vTargetPos - m_vPos));
				D3DXVec3Cross(&vSide,&m_vVel,&vVel);
				fAngle = ACOS(D3DXVec3Dot(&vVel,&m_vVel));
				if(fAngle > MAX_MISSILE_TURN_ANGLE)
					fAngle = MAX_MISSILE_TURN_ANGLE;
				float fCheck = 5.0f*fAngle*fElapsedTime;
				if(fCheck > fAngle)
				{
					fCheck = fAngle;
				}
				D3DXMatrixRotationAxis(&mat,&vSide,fCheck);
				D3DXVec3TransformCoord( &m_vVel, &m_vVel, &mat );
//				D3DXVec3Normalize(&m_vVel,&(MAX_MISSILE_TURN_RATE*vVel*fElapsedTime+m_vVel));
			}
			m_vOriPos += m_fWeaponSpeed*m_vVel*fElapsedTime;
			m_fWeaponSpeed += 60.0f*fElapsedTime;
/*			if(m_bSetTarget && m_pTarget && m_pTarget->m_dwState == _NORMAL)
			{
				// ��ǥ���� �־����������� ���� Vel�� �ٿ��ش�
				if(m_fWeaponSpeed > 0.0f)
				{
					if(D3DXVec3Length(&(m_vOriPos - m_pTarget->m_vPos)) > D3DXVec3Length(&(m_vOldPos - m_pTarget->m_vPos)))
						m_fWeaponSpeed -= 70.0f*fElapsedTime;
					else
					{
						if(m_fWeaponSpeed < 400.0f)
						{
							m_fWeaponSpeed += 60.0f*fElapsedTime;
						}
					}
				}
				else
				{
					if(m_fWeaponSpeed < 400.0f)
					{
						m_fWeaponSpeed += 60.0f*fElapsedTime;
					}
				}
			}
			else
			{
				if(m_fWeaponSpeed < 400.0f)
				{
					m_fWeaponSpeed += 50.0f*fElapsedTime;
				}
			}
*/			if(m_bWeaponFlyType == 0)
			{
				D3DXMatrixRotationAxis(&mat,&m_vVel,5.0f*m_fWeaponLifeCheckTime);
			}
			else
			{
				D3DXMatrixRotationAxis(&mat,&m_vVel,5.0f*m_fWeaponLifeCheckTime+3.14f);
			}
			D3DXVec3TransformCoord( &vUp, &m_vUp, &mat );
			m_vPos = m_vOriPos + 3.5f*vUp;
		}
		break;
	case WEAPON_FLYTYPE_GUIDEBACK:
		{
			if(m_bSetTarget)
			{
				D3DXVec3Normalize(&vVel,&(m_vTargetPos - m_vPos));
				D3DXVec3Cross(&vSide,&m_vVel,&vVel);
				fAngle = ACOS(D3DXVec3Dot(&vVel,&m_vVel));
				if(fAngle > MAX_MISSILE_TURN_ANGLE)
					fAngle = MAX_MISSILE_TURN_ANGLE;
				float fCheck = 5.0f*fAngle*fElapsedTime;
				if(fCheck > fAngle)
				{
					fCheck = fAngle;
				}
				D3DXMatrixRotationAxis(&mat,&vSide,fCheck);
				D3DXVec3TransformCoord( &m_vVel, &m_vVel, &mat );
//				D3DXVec3Normalize(&m_vVel,&(MAX_MISSILE_TURN_RATE*vVel*fElapsedTime+m_vVel));
			}
/*			else
			{
				D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
				D3DXMatrixRotationAxis(&mat,&vSide,-(3.14f/1.0f)*fElapsedTime);
				D3DXVec3TransformCoord( &m_vVel, &m_vVel, &mat );
			}
*/			m_vPos += m_fWeaponSpeed*m_vVel*fElapsedTime;
			m_fWeaponSpeed += 60.0f*fElapsedTime;
/*			if(m_bSetTarget && m_pTarget && m_pTarget->m_dwState == _NORMAL)
			{
				// ��ǥ���� �־����������� ���� Vel�� �ٿ��ش�
				if(m_fWeaponSpeed > 0.0f)
				{
					if(D3DXVec3Length(&(m_vPos - m_pTarget->m_vPos)) > D3DXVec3Length(&(m_vOldPos - m_pTarget->m_vPos)))
						m_fWeaponSpeed -= 70.0f*fElapsedTime;
					else
					{
						if(m_fWeaponSpeed < 400.0f)
						{
							m_fWeaponSpeed += 60.0f*fElapsedTime;
						}
					}
				}
				else
				{
					if(m_fWeaponSpeed < 400.0f)
					{
						m_fWeaponSpeed += 60.0f*fElapsedTime;
					}
				}
			}
			else
			{
				if(m_fWeaponSpeed < 400.0f)
				{
					m_fWeaponSpeed += 50.0f*fElapsedTime;
				}
			}
*/		}
		break;
	case WEAPON_FLYTYPE_DOWNX:
		{// X-���·� �������� �߻�
			if(m_fWeaponLifeCheckTime > 0.8f)
			{
				if(!m_bSetTarget)
				{
					m_bSetTarget = TRUE;
					m_vStartPos = m_vPos;
					if(m_pTarget)
					{
						m_vTargetPos = m_pTarget->m_vPos;
						D3DXVec3Normalize(&m_vVel,&(m_vTargetPos - m_vPos));
					}
					else
					{
						D3DXVec3Normalize(&m_vVel,&(m_vTargetPos - m_vPos));
					}
					D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
					D3DXVec3Cross(&m_vUp,&m_vVel,&vSide);
					m_fTargetDistance = D3DXVec3Length(&(m_vPos - m_vTargetPos));
				}
				m_vPos += m_fWeaponSpeed*m_vVel*fElapsedTime;
			}
			else
			{
				if(m_pTarget)
				{
					D3DXVec3Normalize(&m_vVel,&(m_pTarget->m_vPos - m_vPos));
				}
				D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
				m_vPos += 0.05f*m_fWeaponLifeCheckTime*m_fWeaponSpeed*m_vVel*fElapsedTime;
				m_vPos -= 0.01f*m_fWeaponLifeCheckTime*m_fWeaponSpeed*m_vUp*fElapsedTime;
				if(m_bWeaponFlyType == 0)
				{
					m_vPos += 0.05f*m_fWeaponLifeCheckTime*m_fWeaponSpeed*vSide*fElapsedTime;
				}
				else
				{
					m_vPos -= 0.05f*m_fWeaponLifeCheckTime*m_fWeaponSpeed*vSide*fElapsedTime;
				}
			}
		}
		break;
	case WEAPON_FLYTYPE_DOWNLR:
		{// �¿�� - ���·� �������� �߻�
			if(m_fWeaponLifeCheckTime > 0.8f)
			{
				if(!m_bSetTarget)
				{
					m_bSetTarget = TRUE;
					m_vStartPos = m_vPos;
					if(m_pTarget)
					{
						m_vTargetPos = m_pTarget->m_vPos;
						D3DXVec3Normalize(&m_vVel,&(m_vTargetPos - m_vPos));
					}
					else
					{
						D3DXVec3Normalize(&m_vVel,&(m_vTargetPos - m_vPos));
					}
					D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
					D3DXVec3Cross(&m_vUp,&m_vVel,&vSide);
					m_fTargetDistance = D3DXVec3Length(&(m_vPos - m_vTargetPos));
				}
				m_vPos += m_fWeaponSpeed*m_vVel*fElapsedTime;
			}
			else
			{
				if(m_pTarget)
				{
					D3DXVec3Normalize(&m_vVel,&(m_pTarget->m_vPos - m_vPos));
				}
				D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
				m_vPos += 0.05f*m_fWeaponLifeCheckTime*m_fWeaponSpeed*m_vVel*fElapsedTime;
				m_vPos -= 0.01f*m_fWeaponLifeCheckTime*m_fWeaponSpeed*m_vUp*fElapsedTime;
				if(m_bWeaponFlyType == 0)
				{
					m_vPos -= 0.05f*m_fWeaponLifeCheckTime*m_fWeaponSpeed*vSide*fElapsedTime;
				}
				else
				{
					m_vPos += 0.05f*m_fWeaponLifeCheckTime*m_fWeaponSpeed*vSide*fElapsedTime;
				}
			}
		}
		break;
	}
}

void CWeapon::SetBodyConditionMatrixForSkillEffect(BodyCond_t hyBodyConditon, D3DXMATRIX mat)
{	
	CUnitData* pUnitData = (CUnitData*)m_pAttacker;
	if(pUnitData && pUnitData->m_pSkillEffect)
	{
		pUnitData->m_pSkillEffect->SetBodyConditionMatrix(hyBodyConditon,mat);
	}
}

void CWeapon::ChangeBodyConditionForSkillEffect(int nSkillNum,BodyCond_t hyBodyConditon)
{
	m_bodyCondition = hyBodyConditon;
	CUnitData* pUnitData = (CUnitData*)m_pAttacker;
	if(pUnitData && pUnitData->m_pSkillEffect)
	{
		pUnitData->m_pSkillEffect->ChangeBodyCondition(nSkillNum,hyBodyConditon);
	}
}

void CWeapon::DeleteSkillEffect(int nSkillNum)
{
	CUnitData* pUnitData = (CUnitData*)m_pAttacker;
	if(pUnitData && pUnitData->m_pSkillEffect)
	{
		pUnitData->m_pSkillEffect->DeleteSkillEffect(nSkillNum);
	}
}

void CWeapon::DeleteChargingStateSkillEffect()
{
	CUnitData* pUnitData = (CUnitData*)m_pAttacker;
	if(pUnitData && pUnitData->m_pSkillEffect)
	{
		pUnitData->m_pSkillEffect->DeleteChargingStateSkillEffect();
	}
}

