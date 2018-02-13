// Skill.cpp: implementation of the CSkill class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Skill.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "Chat.h"
#include "AtumDatabase.h"
#include "SkillInfo.h"
#include "StoreData.h"
#include "FieldWinSocket.h"
#include "INFGameMain.h"
#include "INFSkill.h"
#include "SceneData.h"
#include "EnemyData.h"
#include "SkillEffect.h"
#include "ClientParty.h"
#include "StoreData.h"
#include "ItemInfo.h"
#include "dxutil.h"
#include "INFInvenExtend.h"

#include "FreeWar.h"

// 2008-12-29 by bhsohn ��� �� ���� ��ų ���� �ý���
#define		TERM_TICK_MUST_SERVER_CHECK_SKILL_DEADSTAT_REATTACK_TIME		60000	// (1000(1��)*60��(1��))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkill::CSkill()
{
	m_bSmartSpeed = FALSE;

	// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
	m_bMonsterHold	= FALSE;	// Hold����?
	// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����

	m_nSkillPatternType = 0;
	m_nTargetIndex = 0;
	m_nCharacterUID = 0;
	m_bSkillTargetState = FALSE;
	m_nChargingShotState = SKILL_STATE_READY;
	m_pSecSkillInfo = NULL;
	m_pPriSkillInfo = NULL;
	m_bScan			= FALSE;
	//m_fScanRange	= 0.f;

	// 2007-04-05 by bhsohn ��� ���� ó��
	m_nPreSkillItemNum = 0;

	// 2007-04-19 by bhsohn ��ġ���� ������ �߰�
	m_nScanState = 0;
	for(int nCnt = 0;nCnt < MAX_SCAN;nCnt++)
	{
		m_fScanRange[nCnt]	= 0.f;
		memset(&m_vScanPosition[nCnt], 0x00, sizeof(D3DXVECTOR3));
	}
	
}

CSkill::~CSkill()
{
	map<int, CSkillInfo*>::iterator itSkillInfo = m_mapSkill.begin();
	while(itSkillInfo != m_mapSkill.end())
	{
		util::del(itSkillInfo->second);
		itSkillInfo++;
	}
	m_mapSkill.clear();

	itSkillInfo = m_mapEnemySkillInfo.begin();
	while(itSkillInfo != m_mapEnemySkillInfo.end())
	{
		util::del(itSkillInfo->second);
		itSkillInfo++;
	}
	m_mapEnemySkillInfo.clear();
	
}

CSkillInfo* CSkill::PutSkill(ITEM_SKILL* pItemSkill)
{
	CSkillInfo* pSkillInfo = FindItemSkill(pItemSkill->ItemNum);
	if(pSkillInfo)
	{// 2007-01-05 by dgwoo, �̹� ��ų�� �����ϴ��� üũ
		return pSkillInfo;
	}

	pSkillInfo = new CSkillInfo(pItemSkill);
	m_mapSkill[pItemSkill->ItemNum] = pSkillInfo;
	if(pSkillInfo->ItemInfo &&
		pSkillInfo->ItemInfo->SkillType == SKILLTYPE_PERMANENT)
	{	
		for(int i=0;i< SIZE_MAX_POS;i++)
		{
			CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(i);
			if(	pItemSkill->ItemInfo->ReqItemKind == ITEMKIND_ALL_ITEM ||
				(it != g_pStoreData->m_mapItemWindowPosition.end() &&
				CAtumSJ::CheckReqItemKind(pItemSkill->ItemInfo->ReqItemKind,it->second->GetRealItemInfo()->Kind)) )
			{
				UseSkill(pSkillInfo);
				return pSkillInfo;
			}
		}
	}
	return pSkillInfo;
}

void CSkill::DeleteSkill(LONGLONG UniqueNumber)
{
	FLOG( "CSkill::DeleteSkill(LONGLONG UniqueNumber)" );
	map<int, CSkillInfo*>::iterator itSkillInfo = m_mapSkill.begin();
	for(; itSkillInfo != m_mapSkill.end(); itSkillInfo++)
	{
		if(itSkillInfo->second->UniqueNumber == UniqueNumber)
		{
			ReleaseSkill(itSkillInfo->second);

			// 2007-01-05 by dgwoo, ��ų ���Խ� �̱� ������ ��� ��ų�� ��� �Ѵ�
			CancelSkillByBaseNum(itSkillInfo->second->ItemNum, TRUE);

			util::del(itSkillInfo->second);
			m_mapSkill.erase(itSkillInfo);
			break;
		}
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkill::ReleseSkill()
/// \brief		����,�κ����� ��ų�� �ߵ����϶� ������Ų��.
/// \author		dgwoo
/// \date		2006-11-28 ~ 2006-11-28
/// \warning	
///
/// \param		int nType : ����� ��ų
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::ReleseSkill(int nType, INT nAttackSkillItemNum)
{
	if(nType == DES_SKILL_SCANNING)
	{
		vector<CSkillInfo*>::iterator itvecSkillInfo = m_vecUsingSkill.begin();
		while(itvecSkillInfo != m_vecUsingSkill.end())
		{
			if(SKILL_BASE_NUM((*itvecSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_CAMOUFLAGE ||
				SKILL_BASE_NUM((*itvecSkillInfo)->ItemNum) == BGEAR_SKILL_BASENUM_INVISIBLE )
			{
				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK, nAttackSkillItemNum);
			}
			itvecSkillInfo++;
		}
	}
	else if(nType == DES_SKILL_CAMOUFLAGE)
	{
		vector<CSkillInfo*>::iterator itvecSkillInfo = m_vecUsingSkill.begin();
		while(itvecSkillInfo != m_vecUsingSkill.end())
		{
			if(SKILL_BASE_NUM((*itvecSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_CAMOUFLAGE)
				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			itvecSkillInfo++;
		}
	}
	else if(nType == DES_SKILL_CANCELALL)
	{
// 2007-01-29 by dgwoo ��ȭ ��ų ���� �� ���ð����� �ʱ�ȭ ��Ų��� ����.
		// ���� �� ��ų.
		vector<CSkillInfo*>::iterator itvecSkillInfo = m_vecUsingSkill.begin();
		while(itvecSkillInfo != m_vecUsingSkill.end())
		{
//			if((*itvecSkillInfo)->ItemInfo->AttackTime > 0)
//			{
//				g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
//			}
			
			if((*itvecSkillInfo)->GetSkillState() <= SKILL_STATE_USING)
			{
				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_USING);
				g_pShuttleChild->m_pSkillEffect->DeleteSkillEffect((*itvecSkillInfo)->ItemInfo->ItemNum);
				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
				// 2007-03-06 by dgwoo 10���� �������� ����.

				// 2009. 04. 06 by ckPark ���� ��ų ���� �Ӽ� �߰�
// 				if((*itvecSkillInfo)->ItemInfo->ReAttacktime < TERM_TICK_MUST_SERVER_CHECK_SKILL_REATTACK_TIME
// 					&& !(*itvecSkillInfo)->IsMustOneTargetSkill((*itvecSkillInfo)->ItemInfo->SkillTargetType))
				if( !COMPARE_BIT_FLAG((*itvecSkillInfo)->ItemInfo->ItemAttribute, SKILL_ATTR_STORE_USING_TIME)
					&& !(*itvecSkillInfo)->IsMustOneTargetSkill((*itvecSkillInfo)->ItemInfo->SkillTargetType))
				// end 2009. 04. 06 by ckPark ���� ��ų ���� �Ӽ� �߰�
					(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_RELEASE);
			}
			itvecSkillInfo++;
		}
		// �ٸ������� �� ��ų.
		itvecSkillInfo = m_vecEnemyUsingSkillForMe.begin();
		while(itvecSkillInfo != m_vecEnemyUsingSkillForMe.end())
		{
			// 2009. 08. 11 by ckPark ��� ���� ��ų
			//if((*itvecSkillInfo)->GetSkillState() == SKILL_STATE_USING)
			if( (*itvecSkillInfo)->GetSkillState() == SKILL_STATE_USING
				&& !COMPARE_BIT_FLAG( (*itvecSkillInfo)->ItemInfo->ItemAttribute, SKILL_ATTR_PARTY_FORMATION_SKILL ) 
				&& !IS_INFUENCE_BUFF((*itvecSkillInfo)->ItemInfo)
#ifdef _OUTPOST_BUFFS
				&& !(*itvecSkillInfo)->ItemInfo->IsExistDesParam(DES_SKILLTYPE_OUTPOST_BUFF)
#endif
				) // 2013-05-28 by bhsohn ���¹����� [��ȭ]�� �� ���󰡴� ���� ó��
			// end 2009. 08. 11 by ckPark ��� ���� ��ų
			{
				if((*itvecSkillInfo)->ItemInfo->AttackTime > 0)
				{
					g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
				}
				// �ٸ� ������ �� ��ų�� ����
				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_RELEASE);

			}
			itvecSkillInfo++;
		}

// 2007-01-29 by dgwoo ��ȭ ��ų ���� ����ð��� ����.
//			if((*itvecSkillInfo)->GetSkillState() != SKILL_STATE_WAIT_REATTACK)
//			{
//				if((*itvecSkillInfo)->ItemInfo->AttackTime > 0)
//				{
//					g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
//				}
//				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
//			}
//			itvecSkillInfo++;
//		}
//		// 2006-12-28 by dgwoo �ٸ� ������ �� ��ų�� �Բ� �����Ѵ�.
//		itvecSkillInfo = m_vecEnemyUsingSkillForMe.begin();
//		while(itvecSkillInfo != m_vecEnemyUsingSkillForMe.end())
//		{
//			if((*itvecSkillInfo)->GetSkillState() != SKILL_STATE_WAIT_REATTACK)
//			{
//				if((*itvecSkillInfo)->ItemInfo->AttackTime > 0)
//				{
//					g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
//				}
//				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
//
//			}
//			itvecSkillInfo++;
//		}
// 2007-02-01 by dgwoo ����Ŀ��ų�� ����Ͽ��� ��������ų�� ��ҽ�Ű�� �ʴ´�.
//	}else if(nType == IGEAR_SKILL_BASENUM_BERSERKER)
//	{
//		vector<CSkillInfo*>::iterator itvecSkillInfo = m_vecUsingSkill.begin();
//		while(itvecSkillInfo != m_vecUsingSkill.end())
//		{
//			if(SKILL_BASE_NUM((*itvecSkillInfo)->ItemNum) == IGEAR_SKILL_BASENUM_FRENZY)
//				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
//			itvecSkillInfo++;
//		}
	}
	// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
	// ������� ������� ����
	else if( nType == DES_SKILL_RELEASE )
	{
		vector<CSkillInfo*>::iterator itvecSkillInfo = m_vecEnemyUsingSkillForMe.begin();
		while(itvecSkillInfo != m_vecEnemyUsingSkillForMe.end())
		{
			if( IS_ORBIT_SKILL( (*itvecSkillInfo)->ItemInfo->OrbitType )
				&& (*itvecSkillInfo)->GetSkillState() == SKILL_STATE_USING )
			{
				if((*itvecSkillInfo)->ItemInfo->AttackTime > 0)
				{
					g_pShuttleChild->m_pSkillEffect->DeleteCastingStateMonsterSkillEffect();
				}
				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_RELEASE);
			}
			itvecSkillInfo++;
		}
	}
	// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
}

void CSkill::UseSkill(CSkillInfo* pItemSkill)
{
	FLOG( "CSkill::UseSkill(CSkillInfo* pItemSkill)" );
	if( pItemSkill->ItemInfo->SkillType != SKILLTYPE_PERMANENT &&
		g_pD3dApp->m_dwGameState == _CITY )
	{
		// ���ÿ����� �нú� ��ų�� �����ϰ� �ٸ� ��ų�� ����� �� ����.
		return;
	}
	
	// 2004-11-26 by jschoi - �����߿��� ��ų�� ����� �� ����.
	if( g_pShuttleChild->m_nCurrentPatternNumber == PATTERN_UNIT_WARP_OUT ||
		g_pShuttleChild->m_nCurrentPatternNumber == PATTERN_UNIT_WARP_IN)
	{
		return;
	}

	if(pItemSkill->ItemInfo->SkillType != SKILLTYPE_PERMANENT)
	{
		vector<CSkillInfo*>::iterator itvecSkillInfo = m_vecUsingSkill.begin();
		while(itvecSkillInfo != m_vecUsingSkill.end())
		{
			if(((*itvecSkillInfo)->ItemNum == pItemSkill->ItemNum) &&
				((*itvecSkillInfo)->GetAttackIndex() == g_pShuttleChild->m_myShuttleInfo.ClientIndex))
			{
				if((*itvecSkillInfo)->IsEnableStateForReUsing())
				{
					// �ð��� ��ų ���� : �ð��� ��ų�� ��� ReattackTime�� ������ ������ �����ϴ�.
					//(*itvecSkillInfo)->ReUsingSkill();
					// 2012-08-09 by mspark, �ð��� ��ų �ߵ� �ð��� 10�� ���Ϸ� �������� ��� �κ����� ���¿����� ��ߵ��Ǵ� ���� ��ߵ� �ȵǵ��� ����
					if(!IsUseSkill(pItemSkill))
					{
					(*itvecSkillInfo)->ReUsingSkill();
					}
					// end 2012-08-09 by mspark, �ð��� ��ų �ߵ� �ð��� 10�� ���Ϸ� �������� ��� �κ����� ���¿����� ��ߵ��Ǵ� ���� ��ߵ� �ȵǵ��� ����
					// 2006-11-28 by dgwoo ���� ���彺ų�� ��ų�� ������̸� ��ų�� Ǭ��.
					if(g_pShuttleChild->GetUnitAlphaState())
						ReleseSkill(DES_SKILL_CAMOUFLAGE);
				}
				else
				{
					g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0008,COLOR_SKILL_CANCEL);//"��ߵ� �ð��� ���ҽ��ϴ�."
				}
				return;
			}

			itvecSkillInfo++;
		}
	}

	// 2005-11-26 by ispark, ��ų�� ��� �����Ѱ�?
	if(IsUseSkill(pItemSkill))
	{
		return;
	}
	// 2006-12-04 by dgwoo ����� ��ų�� ����ϸ� �ڵ����� Ǯ����.
	if(g_pShuttleChild->GetUnitAlphaState())
		ReleseSkill(DES_SKILL_CAMOUFLAGE);
	// ���� ������ ��ų�� ������� �ƴ϶��
	map<int,CSkillInfo*>::iterator itmapSkillInfo = m_mapSkill.find(pItemSkill->ItemNum);
	if(itmapSkillInfo != m_mapSkill.end())
	{
		// icon
		if(NULL == g_pShuttleChild->m_pSkill->FindUsingSkillInfo(itmapSkillInfo->second->ItemNum))
		{
			// 2007-04-05 by bhsohn ��� ���� ó��
			m_nPreSkillItemNum = pItemSkill->ItemNum;
			//DBGOUT("CSkill::UseSkill [%d]\n", pItemSkill->ItemNum);

			itmapSkillInfo->second->Init();
			
			if(itmapSkillInfo->second->ItemInfo->AttackTime > 0)
			{// 2007-01-06 by dgwoo AttackTime �ִ°͸� ���� ����ϴ� vector�� �̸� ����Ѵ�.
				g_pGameMain->m_pInfSkill->InsertSkillInfo(itmapSkillInfo->second);
				
			}
			// 2009-02-17 by bhsohn ��ĵ ���� ����
			itmapSkillInfo->second->SetSkillEnable(TRUE);
			// end 2009-02-17 by bhsohn ��ĵ ���� ����
			m_vecUsingSkill.push_back(itmapSkillInfo->second);
			if(itmapSkillInfo->second->ItemInfo->SkillType != SKILLTYPE_PERMANENT)
			{
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_SKILL_0001,COLOR_SKILL_USE,CHAT_TAB_SYSTEM);	//"��ų�� �����մϴ�."
			}			

			// 2008-03-04 by bhsohn �������� ��������� ���ߴ� ����ó��
			if(IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind)
				&&(SKILL_BASE_NUM(itmapSkillInfo->second->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEMODE 
					||SKILL_BASE_NUM(itmapSkillInfo->second->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEDEFENSEMODE 
					||SKILL_BASE_NUM(itmapSkillInfo->second->ItemNum) == AGEAR_SKILL_BASENUM_AIRSIEGEMODE) )
			{
				// ��� ������, â�� �ݴ´�.
				g_pGameMain->RightWindowShow(FALSE,g_pGameMain->m_nRightWindowInfo);				
				g_pGameMain->LeftWindowShow(FALSE,g_pGameMain->m_nLeftWindowInfo);
				
				// 2010-10-14 by jskim �Ѹ��� ������� ���� ��������� Ÿ�� �������� ���� ����
				if( ( g_pShuttleChild->IsMyShuttleRolling() ) )
				{
					g_pShuttleChild->InitRolling();
					g_pShuttleChild->m_bMouseMoveLock = FALSE;				// 2005-08-05 by ispark		
					g_pShuttleChild->m_bIsCameraPattern = FALSE;
				}
				// end 2010-10-14 by jskim �Ѹ��� ������� ���� ��������� Ÿ�� �������� ���� ����
			}
			// end 2008-03-04 by bhsohn �������� ��������� ���ߴ� ����ó��
			
			if(itmapSkillInfo->second->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE ||
				itmapSkillInfo->second->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCLUDE_ME ||
				itmapSkillInfo->second->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE ||
				itmapSkillInfo->second->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME ||
				itmapSkillInfo->second->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCEPT_OURS)
			{ // Ÿ���� Ŭ���ؾ��ϴ� �ִ� ��ų Ÿ��
				SetPrimarySkill(itmapSkillInfo->second);
				SetSkillTargetState(TRUE);	// ��ų ���콺 Ÿ�� ����
			}
		}
	}
	else
	{
		DBGOUT("Use Skill : Can't Find Skill Information .\n");
	}
}

void CSkill::ReleaseSkill(CSkillInfo* pItemSkill)
{
	FLOG( "CSkill::ReleaseSkill(CSkillInfo* pItemSkill)" );

	CSkillInfo *pSkillInfo = FindUsingSkillInfo(pItemSkill->ItemNum);
	if(NULL == pSkillInfo)
	{
		return;
	}

	pSkillInfo->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
}

void CSkill::Tick(float fElaspedTime)
{
	// 2007-02-13 by dgwoo ��ų�� ���� �ð����� ���.
	fElaspedTime = g_pD3dApp->GetCheckElapsedTime();
	if(GetSkillPatternType() != 0 && g_pShuttleChild->m_bSkillMoveIsUse == FALSE)	
	{// ��ų ������ ������ �����
		CSkillInfo* pSkillInfo = FindUsingSkillInfoByBaseNum(GetSkillPatternType());
		if(	pSkillInfo &&
			SKILL_KIND(pSkillInfo->ItemNum) == SKILL_KIND_CONTROL &&
			pSkillInfo->ItemInfo->SkillType == SKILLTYPE_CLICK )	// ������ �ִ� ��ų�̴�.(SKILL_KIND_CONTROL �̰�, Ŭ�����̴�. == ������ �ִ� ��ų�̴�.)
		{
			pSkillInfo->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
		}
	}
	
	vector<CSkillInfo*>::iterator itvecSkillInfo = m_vecUsingSkill.begin();
	while(itvecSkillInfo != m_vecUsingSkill.end())
	{
		if((*itvecSkillInfo)->GetSkillState() == SKILL_STATE_RELEASE)
		{
			// 2007-01-29 by dgwoo ��ȭ��ų ����ð����� �ʱ�ȭ�Ѵ�.
			(*itvecSkillInfo)->SetInitReattackTime();
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itvecSkillInfo, g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber);
			SafeDeleteSkillInfo(*itvecSkillInfo);
			itvecSkillInfo = m_vecUsingSkill.erase(itvecSkillInfo);
		}
		else
		{
			(*itvecSkillInfo)->Tick(fElaspedTime);
			itvecSkillInfo++;
		}
	}
	itvecSkillInfo = m_vecEnemyUsingSkillForMe.begin();
	while(itvecSkillInfo != m_vecEnemyUsingSkillForMe.end())
	{
		if((*itvecSkillInfo)->GetSkillState() == SKILL_STATE_RELEASE)
		{
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itvecSkillInfo);
			SafeDeleteSkillInfo(*itvecSkillInfo);
			itvecSkillInfo = m_vecEnemyUsingSkillForMe.erase(itvecSkillInfo);
		}
		else
		{
			(*itvecSkillInfo)->Tick(fElaspedTime);
			itvecSkillInfo++;
		}
	}
}

char* CSkill::GetSkillName(int nSkillItemNumber)
{
	FLOG( "CSkill::GetSkillName(int nSkillItemNumber)" );
	ITEM* pItem = g_pDatabase->GetServerItemInfo( nSkillItemNumber );
	if(pItem == NULL)
	{
		return NULL;
	}
	return pItem->ItemName;
}

int	CSkill::FindSkillItemNum(LONGLONG UniqueNumber)
{
	FLOG( "CSkill::FindSkillItemNum(LONGLONG UniqueNumber)" );
	int nSkillItemNum = 0;
	map<int, CSkillInfo*>::iterator itSkillInfo = m_mapSkill.begin();	
	while(itSkillInfo != m_mapSkill.end())
	{
		if(itSkillInfo->second->UniqueNumber == UniqueNumber)
		{
			nSkillItemNum = itSkillInfo->second->ItemNum;
			break;
		}
		itSkillInfo++;
	}
	return nSkillItemNum;
}

ITEM* CSkill::FindItem(int nSkillItemNumber)
{
	FLOG( "CSkill::FindItem(int nSkillItemNumber)" );
	return g_pDatabase->GetServerItemInfo(nSkillItemNumber);
}

CSkillInfo* CSkill::FindItemSkill(int nSkillItemNumber)
{
	FLOG( "CSkill::FindItemSkill(int nSkillItemNumber)" );
	map<int, CSkillInfo*>::iterator itSkillInfo = m_mapSkill.find(nSkillItemNumber);
	if(itSkillInfo != m_mapSkill.end())
	{	
		return itSkillInfo->second;
	}
	return NULL;
}

int	CSkill::FindItemSkillLevelByBaseNum(int nSkillBaseNum)
{
	map<int, CSkillInfo*>::iterator itSkillInfo = m_mapSkill.begin();
	while(itSkillInfo != m_mapSkill.end())
	{	
		if(SKILL_BASE_NUM(itSkillInfo->second->ItemNum) == nSkillBaseNum)
		{
			return SKILL_LEVEL(itSkillInfo->second->ItemNum);
		}
		itSkillInfo++;
	}
	return NULL;
}

CSkillInfo* CSkill::FindUsingSkillInfo(int nSkillItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if((*itSkillInfo)->ItemNum == nSkillItemNum)
		{
			return (*itSkillInfo);
		}
		itSkillInfo++;
	}
	return NULL;
}

CSkillInfo* CSkill::FindEnemyUsingSkillForMeInfo(int nSkillItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecEnemyUsingSkillForMe.begin();
	while(itSkillInfo != m_vecEnemyUsingSkillForMe.end())
	{
		if((*itSkillInfo)->ItemNum == nSkillItemNum)
		{
			return (*itSkillInfo);
		}
		itSkillInfo++;
	}
	return NULL;

}

CSkillInfo* CSkill::FindEnemyUsingSkillForMeInfoByBaseNum(int nSkillItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecEnemyUsingSkillForMe.begin();
	while(itSkillInfo != m_vecEnemyUsingSkillForMe.end())
	{
		if(SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == SKILL_BASE_NUM(nSkillItemNum))
		{
			return (*itSkillInfo);
		}
		itSkillInfo++;
	}
	return NULL;

}

CSkillInfo* CSkill::FindUsingSkillInfoByBaseNum(int nSkillItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if(SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == SKILL_BASE_NUM(nSkillItemNum))
		{
			if((*itSkillInfo)->GetSkillState() <= SKILL_STATE_USING)
				return (*itSkillInfo);
		}
		itSkillInfo++;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkillInfo* CSkill::FindUsingSkillExistInfoByBaseNum(int nSkillItemNum)
/// \brief		m_vecUsingSkill�� �����ϴ� ���� ã���ش�.(BaseNum)
/// \author		dgwoo
/// \date		2007-01-15 ~ 2007-01-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CSkillInfo* CSkill::FindUsingSkillExistInfoByBaseNum(int nSkillItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if(SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == SKILL_BASE_NUM(nSkillItemNum))
		{
			return (*itSkillInfo);
		}
		itSkillInfo++;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			IsExistSingleSkill()
/// \brief		���� ������� ��ų�� �ܵ����θ� ��밡���� ��ų�� ������ΰ�?
/// \author		jschoi
/// \date		2004-10-04 ~ 2004-10-04
/// \warning	���� ���,�׶��� ����, ��� ��Ʈ�� ��ų�� ��¡�� Ÿ�� ��ų�� �ܵ����θ� ����Ѵ�.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsExistSingleSkill()
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if(	SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEMODE ||
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEDEFENSEMODE ||
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_AIRSIEGEMODE ||
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == BGEAR_SKILL_BASENUM_GROUNDBOMBINGMODE ||
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_GROUNDACCELERATOR ||
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == BGEAR_SKILL_BASENUM_AIRBOMBINGMODE ||
			(SKILL_KIND((*itSkillInfo)->ItemNum) == SKILL_KIND_CONTROL && (*itSkillInfo)->GetSkillState() <= SKILL_STATE_USING))
		{
			char strMessage[256];
			wsprintf(strMessage,STRERR_C_SKILL_0009,//"[%s] ��ų�� ���ÿ� ��� �� �� ���� ��ų�Դϴ�."
				g_pShuttleChild->m_pSkill->GetSkillName((*itSkillInfo)->ItemInfo->ItemNum));
			g_pD3dApp->m_pChat->CreateChatChild(strMessage,COLOR_SKILL_CANCEL);
			return TRUE;
			
		}
		itSkillInfo++;
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			IsSingleSkill(int nSkillItemNum)
/// \brief		nSkillItemNum�� ��ų�� �ܵ����� ����ϴ� ��ų�ΰ�?
/// \author		jschoi
/// \date		2004-10-04 ~ 2004-10-04
/// \warning	���� ���,�׶��� ����, ��� ��Ʈ�� ��ų�� ��¡�� Ÿ�� ��ų�� �ܵ����θ� ����Ѵ�.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsSingleSkill(int nSkillItemNum)
{
	CSkillInfo* pItemSkill = FindItemSkill(nSkillItemNum);
	if( pItemSkill == NULL )
	{
		return FALSE;
	}
	if(	
		SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEMODE ||
		SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEDEFENSEMODE ||
		SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_AIRSIEGEMODE ||
		SKILL_BASE_NUM(pItemSkill->ItemNum) == BGEAR_SKILL_BASENUM_GROUNDBOMBINGMODE ||
		SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_GROUNDACCELERATOR ||
		SKILL_BASE_NUM(pItemSkill->ItemNum) == BGEAR_SKILL_BASENUM_AIRBOMBINGMODE)
//		||SKILL_KIND(pItemSkill->ItemNum) == SKILL_KIND_CONTROL)// 2007-06-15 by dgwoo ��۽�ų����� ��Ʈ�ѽ�ų��밡��.
	{
		return TRUE;
	}
	return FALSE;
}

void CSkill::DeleteSkillFromWearItem(BYTE nItemKind)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		UINT fReAttackTime = (*itSkillInfo)->ItemInfo->ReAttacktime;
		
		// 2009. 04. 06 by ckPark ���� ��ų ���� �Ӽ� �߰�
// 		if( (*itSkillInfo)->ItemInfo->ReqItemKind != ITEMKIND_ALL_ITEM &&
// 			CAtumSJ::CheckReqItemKind((*itSkillInfo)->ItemInfo->ReqItemKind, nItemKind) &&
// 			fReAttackTime < TERM_TICK_MUST_SERVER_CHECK_SKILL_REATTACK_TIME)	// 2006-12-20 by ispark, Ư�� �ð� �̻��� ������ �ʴ´�.
		if( (*itSkillInfo)->ItemInfo->ReqItemKind != ITEMKIND_ALL_ITEM &&
			CAtumSJ::CheckReqItemKind((*itSkillInfo)->ItemInfo->ReqItemKind, nItemKind) &&
			!COMPARE_BIT_FLAG((*itSkillInfo)->ItemInfo->ItemAttribute, SKILL_ATTR_STORE_USING_TIME))	// 2006-12-20 by ispark, Ư�� �ð� �̻��� ������ �ʴ´�.
		// end 2009. 04. 06 by ckPark ���� ��ų ���� �Ӽ� �߰�
		{
			(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			g_pShuttleChild->m_pSkillEffect->DeleteSkillEffect((*itSkillInfo)->ItemInfo->ItemNum);
			// 2006-12-21 by dgwoo ���� ������ ���� ��ų�� reattacktime�� 0���� �����Ѵ�.
			(*itSkillInfo)->SetInitReattackTime();
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
			SafeDeleteSkillInfo(*itSkillInfo);
			itSkillInfo = m_vecUsingSkill.erase(itSkillInfo);
		}
		else
		{
			itSkillInfo++;
		}
	}
}

void CSkill::PutSkillFromWearItem(BYTE nItemKind)
{
	map<int, CSkillInfo*>::iterator itSkillInfo = m_mapSkill.begin();
	while(itSkillInfo != m_mapSkill.end())
	{
		if(	itSkillInfo->second->ItemInfo->SkillType == SKILLTYPE_PERMANENT &&
			(nItemKind == ITEMKIND_ALL_ITEM ||
			CAtumSJ::CheckReqItemKind(itSkillInfo->second->ItemInfo->ReqItemKind, nItemKind) ) )
		{
			UseSkill(itSkillInfo->second);
		}
		itSkillInfo++;
	}
}

BOOL CSkill::IsSatisfyReqItemKind(BYTE nReqItemKind, BYTE nItemKind)
{
	if(nReqItemKind == ITEMKIND_ALL_ITEM)
	{
		return TRUE;
	}
	
	switch(nReqItemKind)
	{
	case ITEMKIND_ALL_WEAPON:
		{
			return TRUE;
		}
		break;
	case ITEMKIND_PRIMARY_WEAPON_ALL:
		{
			if(IS_PRIMARY_WEAPON(nItemKind))
				return TRUE;
			else
				return FALSE;
		}
		break;
	case ITEMKIND_PRIMARY_WEAPON_1:
		{
			if(IS_PRIMARY_WEAPON_1(nItemKind))
				return TRUE;
			else
				return FALSE;
		}
		break;
	case ITEMKIND_PRIMARY_WEAPON_2:
		{
			if(IS_PRIMARY_WEAPON_2(nItemKind))
				return TRUE;
			else
				return FALSE;
		}
		break;
	case ITEMKIND_SECONDARY_WEAPON_ALL:
		{
			if(IS_SECONDARY_WEAPON(nItemKind))
				return TRUE;
			else
				return FALSE;
		}
		break;
	case ITEMKIND_SECONDARY_WEAPON_1:
		{
			if(IS_SECONDARY_WEAPON_1(nItemKind))
				return TRUE;
			else
				return FALSE;
		}
		break;
	case ITEMKIND_SECONDARY_WEAPON_2:
		{
			if(IS_SECONDARY_WEAPON_2(nItemKind))
				return TRUE;
			else
				return FALSE;
		}
		break;
	default:
		{
			if(nReqItemKind == nItemKind)
				return TRUE;
			else
				return FALSE;
		}
		break;
	}
	return FALSE;
}

void CSkill::CancelSkill(int nSkillItemNum, BOOL i_bEnemyUsingSkillCancel)
{
	CSkillInfo *pSkillInfo = FindUsingSkillInfo(nSkillItemNum);
	if(pSkillInfo)
	{
		if(0 < pSkillInfo->GetCheckEnableTime())
		{
			pSkillInfo->ChangeSkillState(SKILL_STATE_USING);
		}
		else
		{
			if(pSkillInfo->ItemInfo->AttackTime > 0)
			{
				g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
			}

			g_pGameMain->m_pInfSkill->DeleteSkillInfo(pSkillInfo, g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber);
			DeleteUsingSkillInfo(nSkillItemNum);
		}
	}
	
	if(i_bEnemyUsingSkillCancel)
	{
		pSkillInfo = FindEnemyUsingSkillForMeInfo(nSkillItemNum);
		if(pSkillInfo)
		{
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(pSkillInfo);
			DeleteEnemyUsingSkillInfo(nSkillItemNum);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkill::CancelSkillByBaseNum(int nSkillItemNum, BOOL i_bEnemyUsingSkillCancel)
/// \brief		��ų�� ����ϸ� vecUsing���� ������Ų��.
/// \author		dgwoo
/// \date		2007-01-05 ~ 2007-01-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::CancelSkillByBaseNum(int nSkillItemNum, BOOL i_bEnemyUsingSkillCancel)
{
	CSkillInfo *pSkillInfo = FindUsingSkillExistInfoByBaseNum(nSkillItemNum);
	if(pSkillInfo)
	{
// 2007-01-15 by dgwoo ��ų�� ����Ҷ��� �׳� ������Ų��.
//		if(0 < pSkillInfo->GetCheckEnableTime())
//		{
//			pSkillInfo->ChangeSkillState(SKILL_STATE_USING);
//		}
//		else
//		{
//			if(pSkillInfo->ItemInfo->SkillType == SKILLTYPE_CHARGING)
//			{
//				g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
//			}
//
//			g_pGameMain->m_pInfSkill->DeleteSkillInfo(pSkillInfo, g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber);
//			DeleteUsingSkillInfoByBaseNum(nSkillItemNum);
//		}
		if(pSkillInfo->ItemInfo->AttackTime > 0)
		{
			g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
		}

		g_pGameMain->m_pInfSkill->DeleteSkillInfo(pSkillInfo, g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber);
		DeleteUsingSkillInfoByBaseNum(nSkillItemNum);
		
	}
	
	if(i_bEnemyUsingSkillCancel)
	{
		pSkillInfo = FindEnemyUsingSkillForMeInfoByBaseNum(nSkillItemNum);
		if(pSkillInfo)
		{
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(pSkillInfo);
			DeleteEnemyUsingSkillInfoByBaseNum(nSkillItemNum);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkill::ReleaseAllUsingSkillWithOutPermanent()
/// \brief		������ �׾��� ��� �Ķ���� �� ��ų ����.
/// \author		dgwoo
/// \date		2007-01-10 ~ 2007-01-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::ReleaseAllUsingSkillWithOutPermanent()
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if((*itSkillInfo)->ItemInfo->SkillType != SKILLTYPE_PERMANENT)
		{
			// 2007-04-12 by dgwoo reattact������ ��� �ٽ� reattact�� �ʿ䰡 ����.
			if((*itSkillInfo)->GetSkillState() != SKILL_STATE_WAIT_REATTACK)
			{
				(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			}
			// 2007-01-24 by dgwoo ĳ���� Ÿ���� �ִ� ��� ����Ʈ�� �����Ѵ�.
			//if((*itSkillInfo)->ItemInfo->SkillType == SKILLTYPE_CHARGING)
			if((*itSkillInfo)->ItemInfo->AttackTime > 0)
			{
				g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
			}


			// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
			if((*itSkillInfo)->ItemInfo->AttackTime > 0)
			{
				g_pShuttleChild->m_pSkillEffect->DeleteCastingStateMonsterSkillEffect();
			}
			// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����

	
			// 2006-12-15 by dgwoo, 10�� �̸��ϰ�츸 ��ų ����ð��� �����.
			// 2009. 04. 06 by ckPark ���� ��ų ���� �Ӽ� �߰�
			//if((*itSkillInfo)->ItemInfo->ReAttacktime < TERM_TICK_MUST_SERVER_CHECK_SKILL_REATTACK_TIME)				//5�� ������ ���
			if(!COMPARE_BIT_FLAG((*itSkillInfo)->ItemInfo->ItemAttribute, SKILL_ATTR_STORE_USING_TIME))				//5�� ������ ���
			// end 2009. 04. 06 by ckPark ���� ��ų ���� �Ӽ� �߰�
			{
				(*itSkillInfo)->SetCheckEnableTime();
				(*itSkillInfo)->SetInitReattackTime();
				g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
				SafeDeleteSkillInfo(*itSkillInfo);
				itSkillInfo = m_vecUsingSkill.erase(itSkillInfo);
			}
			else
			{
				itSkillInfo++;
			}
		}
		else
		{
			itSkillInfo++;
		}
	}

	itSkillInfo = m_vecEnemyUsingSkillForMe.begin();
	while(itSkillInfo != m_vecEnemyUsingSkillForMe.end())
	{
		if((*itSkillInfo)->ItemInfo->SkillType != SKILLTYPE_PERMANENT)
		{
			//(*itSkillInfo)->ChangeSkillState(SKILL_STATE_RELEASE);
			(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
			SafeDeleteSkillInfo(*itSkillInfo);
			itSkillInfo = m_vecEnemyUsingSkillForMe.erase(itSkillInfo);
		}
		else
		{
			itSkillInfo++;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkill::ReleaseStatDEAD_AllUsingSkillWithOutPermanent()
/// \brief		������ �׾��� ��� �Ķ���� �� ��ų ����.(��� �� ���� ��ų ���� �ý���)
/// \author		// 2008-12-29 by bhsohn ��� �� ���� ��ų ���� �ý���
/// \date		2008-12-29 ~ 2008-12-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::ReleaseStatDEAD_AllUsingSkillWithOutPermanent()
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if(IsReleaseSkill_StatDEAD((*itSkillInfo))
			&&(*itSkillInfo)->ItemInfo->SkillType != SKILLTYPE_PERMANENT)
		{
			// reattact������ ��� �ٽ� reattact�� �ʿ䰡 ����.
			if((*itSkillInfo)->GetSkillState() != SKILL_STATE_WAIT_REATTACK)
			{
				(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			}
			// ĳ���� Ÿ���� �ִ� ��� ����Ʈ�� �����Ѵ�.
			//if((*itSkillInfo)->ItemInfo->SkillType == SKILLTYPE_CHARGING)
			if((*itSkillInfo)->ItemInfo->AttackTime > 0)
			{
				g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
			}


			// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
			if((*itSkillInfo)->ItemInfo->AttackTime > 0)
			{
				g_pShuttleChild->m_pSkillEffect->DeleteCastingStateMonsterSkillEffect();
			}
			// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����

			
			// 2006-12-15 by dgwoo, 10�� �̸��ϰ�츸 ��ų ����ð��� �����.
			// 2009. 04. 06 by ckPark ���� ��ų ���� �Ӽ� �߰�
			//if((*itSkillInfo)->ItemInfo->ReAttacktime < TERM_TICK_MUST_SERVER_CHECK_SKILL_REATTACK_TIME)				//5�� ������ ���
			if(!COMPARE_BIT_FLAG((*itSkillInfo)->ItemInfo->ItemAttribute, SKILL_ATTR_STORE_USING_TIME))				//5�� ������ ���
			// end 2009. 04. 06 by ckPark ���� ��ų ���� �Ӽ� �߰�
			{
				(*itSkillInfo)->SetCheckEnableTime();
				(*itSkillInfo)->SetInitReattackTime();
				g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
				SafeDeleteSkillInfo(*itSkillInfo);
				itSkillInfo = m_vecUsingSkill.erase(itSkillInfo);
			}
			else
			{
				itSkillInfo++;
			}
		}
		else
		{
			itSkillInfo++;
		}
	}
	
	itSkillInfo = m_vecEnemyUsingSkillForMe.begin();
	while(itSkillInfo != m_vecEnemyUsingSkillForMe.end())
	{
		if(IsReleaseSkill_StatDEAD((*itSkillInfo))
			&& (*itSkillInfo)->ItemInfo->SkillType != SKILLTYPE_PERMANENT)
		{
			//(*itSkillInfo)->ChangeSkillState(SKILL_STATE_RELEASE);
			(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
			SafeDeleteSkillInfo(*itSkillInfo);
			itSkillInfo = m_vecEnemyUsingSkillForMe.erase(itSkillInfo);
		}
		else
		{
			itSkillInfo++;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkill::IsReleaseSkill_StatDEAD()
/// \brief		������ �׾��� ��� ���� ��ų�̳�?
/// \author		// 2008-12-29 by bhsohn ��� �� ���� ��ų ���� �ý���
/// \date		2008-12-29 ~ 2008-12-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsReleaseSkill_StatDEAD(CSkillInfo* i_pSkillInfo)
{
	if(NULL == i_pSkillInfo)
	{
		return FALSE;
	}


	// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����

	// ���� ��ų �׾����� Ǯ����
	if( i_pSkillInfo->ItemInfo->Kind == ITEMKIND_FOR_MON_SKILL )
		return TRUE;

	// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����


	switch(i_pSkillInfo->ItemInfo->SkillType)
	{
	case SKILLTYPE_PERMANENT:
		{
			return FALSE;
		}
	case SKILLTYPE_TIMELIMIT:	// �ð��� ��ų�� ����� �ȵȴ�.
		{
			// ��ų ���� �ð� 1�� �̻��� ��ų�� ������ �ʴ´�.(1�й̸��� ����)
			if(i_pSkillInfo->ItemInfo->Time >= TERM_TICK_MUST_SERVER_CHECK_SKILL_DEADSTAT_REATTACK_TIME)				//5�� ������ ���
			{
				return FALSE;
			}
		}
		break;	
		
	}		
	return TRUE;
}


void CSkill::ReleaseAllUsingToggleSkill()
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if((*itSkillInfo)->ItemInfo->SkillType == SKILLTYPE_TOGGLE)
		{
			(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
			SafeDeleteSkillInfo(*itSkillInfo);
			itSkillInfo = m_vecUsingSkill.erase(itSkillInfo);
		}
		else
		{
			itSkillInfo++;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::ReleaseGroundSkill()
/// \brief		Ground ��ų ����
/// \author		dhkwon
/// \date		2004-10-04 ~ 2004-10-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::ReleaseGroundSkill()
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if(	SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_GROUNDACCELERATOR ||
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == FLASH_ACCELERATOR ||
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEMODE ||				// 2006-05-29 by ispark
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_CAMOUFLAGE ||			// 2006-11-16 by dgwoo
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEDEFENSEMODE)		// 2006-05-29 by ispark
		{
			ReleaseSkill(*itSkillInfo);
//			(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);// 2005-11-26 by ispark �ι� �ߺ�
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
			itSkillInfo = m_vecUsingSkill.erase(itSkillInfo);
			continue;
		}
		itSkillInfo++;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::ReleaseAllChargingSkill()
/// \brief		��¡��ų ����
/// \author		jschoi
/// \date		2004-10-25 ~ 2004-10-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::ReleaseAllChargingSkill()
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		//if((*itSkillInfo)->ItemInfo->SkillType == SKILLTYPE_CHARGING)
		if((*itSkillInfo)->ItemInfo->AttackTime > 0)
		{
			(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
			g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
		}
		itSkillInfo++;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::SendPrepareUseSkill()
/// \brief		������ ��ų�� �ߵ����� �˷��ش�
/// \author		jschoi
/// \date		2004-10-05 ~ 2004-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::SendPrepareUseSkill(CSkillInfo* pSkillInfo)
{
	MSG_FC_SKILL_PREPARE_USE sMsg;
	sMsg.SkillItemID.ItemNum = pSkillInfo->ItemNum;
	sMsg.SkillItemID.ItemUID = pSkillInfo->UniqueNumber;
	g_pFieldWinSocket->SendMsg(T_FC_SKILL_PREPARE_USE, (char*)&sMsg, sizeof(sMsg) );

	g_pShuttleChild->m_myShuttleInfo.CurrentSP -= pSkillInfo->ItemInfo->ReqSP;
	char *strSkillName;
	char strMessage[256];
	strSkillName = GetSkillName(pSkillInfo->ItemNum);
	wsprintf(strMessage,STRMSG_C_SKILL_0002,strSkillName);	//"��ų [%s] �� �ߵ��մϴ�."
	g_pD3dApp->m_pChat->CreateChatChild(strMessage,COLOR_SKILL_USE,CHAT_TAB_SYSTEM);	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::SendPrepareCancelSkill()
/// \brief		������ �ߵ��� ��ų�� ��ҵ��� �˷��ش�.
/// \author		jschoi
/// \date		2004-10-05 ~ 2004-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::SendPrepareCancelSkill()
{
	char* strSkillName;
	char strMessage[256];
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if((*itSkillInfo)->ItemInfo->SkillType == SKILLTYPE_CHARGING)
		//if((*itSkillInfo)->ItemInfo->AttackTime > 0)
		{
			MSG_FC_SKILL_CANCEL_PREPARE sMsg;
			sMsg.SkillItemID.ItemNum = (*itSkillInfo)->ItemNum;
			sMsg.SkillItemID.ItemUID = (*itSkillInfo)->UniqueNumber;
			g_pFieldWinSocket->SendMsg(T_FC_SKILL_CANCEL_PREPARE, (char*)&sMsg, sizeof(sMsg) );
			
			strSkillName = GetSkillName((*itSkillInfo)->ItemNum);
			wsprintf(strMessage,STRERR_C_SKILL_0010,strSkillName);//"��ų [%s] �� �ߵ��� ����մϴ�."	
			g_pD3dApp->m_pChat->CreateChatChild(strMessage,COLOR_SKILL_CANCEL);	
			(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
			SafeDeleteSkillInfo(*itSkillInfo);
			itSkillInfo = m_vecUsingSkill.erase(itSkillInfo);	
			continue;
		}
		itSkillInfo++;
	}
}

void CSkill::CheckChargingShotSkill()
{
//	UINT nSkillNum = 0;
	// ��¡��
	switch(GetChargingShotState())
	{
	case SKILL_STATE_WAITING_PREPARE:
	case SKILL_STATE_PREPARE:
	case SKILL_STATE_WAITING:
		{
			SendPrepareCancelSkill();
			SetChargingShotState(SKILL_STATE_READY);
		}
		break;
// 2007-01-10 by dgwoo 
//	case SKILL_STATE_WAITING:
//		{
//			CSkillInfo* pSkillInfo = GetPrimarySkill();
//			if(pSkillInfo)
//			{
//				pSkillInfo->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
//			}
//			else
//			{
//				SetChargingShotState(SKILL_STATE_READY);
//			}
//		}
//		break;
	case SKILL_STATE_USING:
		{
			CSkillInfo* pSkillInfo = GetPrimarySkill();
			if(pSkillInfo)
			{
				pSkillInfo->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			}
			else
			{
				SetChargingShotState(SKILL_STATE_READY);
			}
		}
		break;
	case SKILL_STATE_READY:
		break;
	case SKILL_STATE_RELEASE:
		SetChargingShotState(SKILL_STATE_READY);
	case SKILL_STATE_WAIT_REATTACK:
	default:
		SetPrimarySkill(NULL);
		break;
	}
//	return nSkillNum;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			UINT CSkill::GetChargingShotTypeSkillNum()
/// \brief		
/// \author		ispark
/// \date		2006-12-06 ~ 2006-12-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
UINT CSkill::GetChargingShotTypeSkillNum()
{
	UINT nSkillNum = 0;

	switch(GetChargingShotState())
	{
	case SKILL_STATE_WAITING_PREPARE:
	case SKILL_STATE_PREPARE:
	case SKILL_STATE_WAITING:
		{
			SendPrepareCancelSkill();
		}
		break;
	case SKILL_STATE_USING:

		{
			CSkillInfo* pSkillInfo = GetPrimarySkill();
			if(pSkillInfo)
			{
				nSkillNum = pSkillInfo->ItemNum;
			}
			else
			{
				SetChargingShotState(SKILL_STATE_READY);
			}
		}
		break;
//	case SKILL_STATE_USING:
//		{
//			CSkillInfo* pSkillInfo = GetPrimarySkill();
//			if(pSkillInfo)
//			{
//				pSkillInfo->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
//			}
//			else
//			{
//				SetChargingShotState(SKILL_STATE_READY);
//			}
//		}
//		break;
	case SKILL_STATE_READY:
		break;
	case SKILL_STATE_WAIT_REATTACK:
	case SKILL_STATE_RELEASE:
	default:
		SetPrimarySkill(NULL);
		break;
	}

	return nSkillNum;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::IsExistTargetOneSkill(CSkillInfo *pItemSkill)
/// \brief		Ÿ���� Ŭ���ؾ� �ߵ��ϴ� ��ų�� �ߵ����ΰ�?
/// \author		jschoi
/// \date		2004-10-14 ~ 2004-10-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsExistTargetOneSkill(CSkillInfo *pItemSkill)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		// 2006-10-26 by ispark, ��ų Ÿ�� Ÿ���� ��ų Ÿ������ ����
		if(((*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE ||
			(*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCLUDE_ME ||
			(*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE ||
			(*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME ||
			(*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCEPT_OURS) &&
			//((*itSkillInfo)->ItemInfo->ItemNum == pItemSkill->ItemInfo->ItemNum) &&					// 2006-12-11 by ispark
//			((*itSkillInfo)->GetAttackIndex() == g_pShuttleChild->m_myShuttleInfo.ClientIndex ||
//			GetSkillTargetState() == TRUE))
			// 2006-12-27 by dgwoo ���� ������ �� �׷��� �������; 
			((*itSkillInfo)->GetSkillState() < SKILL_STATE_USING ))
		{
			return TRUE;
		}
		itSkillInfo++;
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::FindTargetForSkill()
/// \brief		���콺 �����͸� �������� ���� ����� �� �ֺ��� Unit�� ���Ѵ�.(Ŭ��)
/// \author		jschoi
/// \date		2004-10-14 ~ 2004-10-14
/// \warning	
///
/// \param		
/// \return		TRUE : Ÿ���� ��Ҵ�. FALSE : Ÿ���� ����Ҵ�.	
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::FindTargetForSkill()
{
	float fLength = 50.0f;
	POINT pt;
	GetCursorPos(&pt);			
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);
	SetSkillTargetState(FALSE);		// ��ų ���콺 ������ ����
	
	D3DXVECTOR2 vPos1 = D3DXVECTOR2(pt.x,pt.y);
	D3DXVECTOR2 vPos2;
	//////////////////////////////////////////////////////////////////////////
	// ���� ���� Ÿ������ �˻�
	if(g_pShuttleChild->m_nObjScreenW > 0 && m_pPriSkillInfo)
	{
		vPos2 = D3DXVECTOR2(g_pShuttleChild->m_nObjScreenX, g_pShuttleChild->m_nObjScreenY);
		if(D3DXVec2Length(&(vPos1 - vPos2)) < fLength)
		{
			// 2005-12-05 by ispark, �ڽſ��� �� �� ���� ��ų
			if(m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCLUDE_ME ||
				m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME ||
				m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_WITHOUT_ME ||			// 2006-09-20 by ispark, �� �� ����
				m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_INRANGE_WITHOUT_ME ||
				m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCEPT_OURS)
			{
				// �ڱ� �ڽ� ���� �ȉ?
				char strmsg[256];
				wsprintf(strmsg, STRMSG_C_051205_0001, m_pPriSkillInfo->ItemInfo->ItemName);// "[%s] ��ų�� �ڽſ��� �� �� ���� ��ų�Դϴ�."
				g_pD3dApp->m_pChat->CreateChatChild(strmsg, COLOR_SKILL_CANCEL);
				ReleasePrepareTargetOneSkill(m_pPriSkillInfo->ItemNum);
				return FALSE;
			}
			else
			{
				SetTargetIndex(g_pShuttleChild->m_myShuttleInfo.ClientIndex);
				return TRUE;	
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 2005-11-24 by ispark
	// ��� ����Ʈ���� Ŭ��
	BOOL bFindTarget = FALSE;
	vector<PARTYENEMYINFO*>::iterator itParty = g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.begin();
	if(itParty != g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.end())
	{
		bFindTarget = FindListTargetForSkill(pt);
	}
	// Ÿ���� ��������Ƿ� ��ų�� �����ϰ�, �ʱ�ȭ �Ѵ�.
	// ���� UseSkill���µ� �ƴϹǷ� �׳� �����.
	if(bFindTarget)
	{
		// 2010. 06. 18 by jskim �ݿ��� ����� ������ SP �����ϴ� ���� ����
		if(m_pPriSkillInfo->ItemInfo->AttackTime > 0 && 
			bFindTarget &&
			(m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCLUDE_ME		||
			m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME  ||
			m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_WITHOUT_ME		||
			m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_INRANGE_WITHOUT_ME	||
			m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCEPT_OURS))
		{
			m_pPriSkillInfo->ChangeSkillState(SKILL_STATE_WAITING_PREPARE);

			// 2010. 10. 11. ������ Ÿ�� ������ ��ų ���� ����.
			m_pPriSkillInfo->SetClickTagetIndex ( g_pShuttleChild->m_pSkill->GetTargetIndex() );
			m_pPriSkillInfo->SetClickTargetUID ( g_pShuttleChild->m_pSkill->GetCharacterUID() );
			// End 2010. 10. 11. ������ Ÿ�� ������ ��ų ���� ����.

		}
		//end 2010. 06. 18 by jskim �ݿ��� ����� ������ SP �����ϴ� ���� ����
		return TRUE;
	}
	//////////////////////////////////////////////////////////////////////////
	// �ֺ��� Enemy�� Ÿ������ �˻�
	CEnemyData * pTargetEnemy = NULL;
	map<INT,CEnemyData *>::iterator itEnemy = g_pD3dApp->m_pScene->m_mapEnemyList.begin();
	while(itEnemy != g_pD3dApp->m_pScene->m_mapEnemyList.end())
	{
// 2007-02-16 by dgwoo �Ʒ��� ���� ������� �񱳹��� �� �ʿ䰡 ����.
//		if( itEnemy->second->m_nObjScreenW > 0 &&
//			(itEnemy->second->m_bySkillStateFlag == CL_SKILL_NONE ||					// 2006-12-12 by ispark, ����, �κ������� �ƴϰ�
//			(itEnemy->second->m_bySkillStateFlag != CL_SKILL_NONE &&					// 2006-12-11 by ispark, ����, �κ������̶��
//			&& (IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, itEnemy->second->m_infoCharacter.CharacterInfo.InfluenceType) || // 2006-12-12 by ispark, ���� ���� �̰ų�
//			!(itEnemy->second->IsPkEnable())))))										// 2006-12-12 by ispark, PK�� �ƴҶ�
		if(itEnemy->second->m_nObjScreenW > 0
			&& itEnemy->second->m_nAlphaValue != SKILL_OBJECT_ALPHA_OTHER_INFLUENCE)	// 2007-02-16 by dgwoo ������°� �ƴ϶��.
		{
			vPos2 = D3DXVECTOR2(itEnemy->second->m_nObjScreenX,itEnemy->second->m_nObjScreenY);
			float fLengthTemp = D3DXVec2Length(&(vPos1-vPos2));
			if(fLengthTemp < fLength)
			{

				// 2011-03-28 by hsson, �̹� ���� ������ ��ü���� ���� Ÿ�� ������ ���� ����
				BodyCond_t tEnemyBodyCondition = itEnemy->second->m_pCharacterInfo->m_nCurrentBodyCondition;
				if( COMPARE_BODYCON_BIT( tEnemyBodyCondition, BODYCON_DEAD_MASK ) )
				{
					if( m_pPriSkillInfo )
						ReleasePrepareTargetOneSkill(m_pPriSkillInfo->ItemNum);
					
					return FALSE;
				}
				// end 2011-03-28 by hsson, �̹� ���� ������ ��ü���� ���� Ÿ�� ������ ���� ����

				fLength = fLengthTemp;
				pTargetEnemy = itEnemy->second;
			}
		}
		itEnemy++;
	}

	//////////////////////////////////////////////////////////////////////////
	// Ÿ�� ��Ҵ�.
	if(pTargetEnemy && m_pPriSkillInfo)
	{ // Ÿ���� ��Ҵ�. ��ų�� ����Ѵ�.
		// 2006-11-30 by ispark, �Ʊ�(�Ϲ� ���� ����)���� �� �� ���� ��ų
		if(m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCEPT_OURS
#ifdef FREEWAR_
			&& (IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pTargetEnemy->m_infoCharacter.CharacterInfo.InfluenceType, 1, g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex)
#else
			&&(IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pTargetEnemy->m_infoCharacter.CharacterInfo.InfluenceType)
#endif
			&& !(g_pShuttleChild->IsPkEnable() && g_pShuttleChild->GetPVPClientIndex() == pTargetEnemy->m_infoCharacter.CharacterInfo.ClientIndex)
			&& !(g_pShuttleChild->IsPkState(PK_FVF) && g_pShuttleChild->m_pClientParty->IsFVFEnemy(pTargetEnemy->m_infoCharacter.CharacterInfo.CharacterUniqueNumber))
			&& !(g_pShuttleChild->IsPkState(PK_GVG) && pTargetEnemy->m_infoCharacter.CharacterInfo.GuildUniqueNumber == g_pShuttleChild->m_pGuildWarInfo->PeerGuildUID)))

		{
			// �Ʊ� ���� �ȉ?
			char strmsg[256];
			wsprintf(strmsg, STRMSG_C_061130_0000, m_pPriSkillInfo->ItemInfo->ItemName);// "[%s] ��ų�� �ڽ� ���¿��� �� �� ���� ��ų�Դϴ�."
			g_pD3dApp->m_pChat->CreateChatChild(strmsg, COLOR_SKILL_CANCEL);
			ReleasePrepareTargetOneSkill(m_pPriSkillInfo->ItemNum);
			return FALSE;
		}

		// 2005-12-05 by ispark, ��Ƽ���� �ش��ϴ� ��ų ����
		if(	m_pPriSkillInfo &&
			(m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE ||
			m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME))
		{
			// �����鿡�Ը� ���� Ÿ�� ��ų
			char strmsg[256];
			wsprintf(strmsg, STRMSG_C_051205_0002, m_pPriSkillInfo->ItemInfo->ItemName);// "[%s] ��ų�� �ڽſ��� �� �� ���� ��ų�Դϴ�."
			g_pD3dApp->m_pChat->CreateChatChild(strmsg, COLOR_SKILL_CANCEL);
			ReleasePrepareTargetOneSkill(m_pPriSkillInfo->ItemNum);
			return FALSE;
		}

		CSkillInfo* pSkillInfo = NULL;
		if(pSkillInfo = CheckConfirmSkillUse())
		{
			// ����㰡�� �ʿ��� ��ų�̴�.
			SendConfirmSkillUse(pSkillInfo, pTargetEnemy->m_infoCharacter.CharacterInfo.CharacterUniqueNumber);
			return TRUE;
		}

		SetTargetIndex(pTargetEnemy->m_infoCharacter.CharacterInfo.ClientIndex);
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	// Ÿ�� �� ��Ҵ�.
	g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0011,COLOR_SKILL_CANCEL);//"Ÿ���� �������Ͽ� ��ų�� ����մϴ�."

	// 2007-05-22 by bhsohn Ÿ�� ���� �޸� ���� ó��
	//ReleasePrepareTargetOneSkill(m_pPriSkillInfo->ItemNum);
	if(m_pPriSkillInfo)
	{
		ReleasePrepareTargetOneSkill(m_pPriSkillInfo->ItemNum);
	}
	// 2007-05-23 by bhsohn ��ų �ȳ����� ���� ó��
	else
	{
		InitTargetState();	// Ÿ���� ��ų�� �������� �ʱ�ȭ �Ѵ�.
	}
	
	return FALSE;
}

void CSkill::ReleaseAllPrepareTargetOneSkill()
{ // ���� �ߵ����� Target�� �ִ� ��ų�� ��� �����Ѵ�.
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if((*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE ||
			(*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCLUDE_ME ||
			(*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE ||
			(*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME ||
			(*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCEPT_OURS)
		{
			(*itSkillInfo)->ChangeSkillState(SKILL_STATE_RELEASE);
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
			SafeDeleteSkillInfo(*itSkillInfo);
			itSkillInfo = m_vecUsingSkill.erase(itSkillInfo);
		}
		else
		{
			itSkillInfo++;
		}
	}	
	SetTargetIndex(0);				// �ʱ�ȭ
	SetCharacterUID(0);				// �ʱ�ȭ
	SetSkillTargetState(FALSE);		// ��ų ���콺 ������ ����
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkill::ReleasePrepareTargetOneSkill(int nSkillNumber)
/// \brief		Ÿ���� ��ų�� �����Ѵ�.
/// \author		dgwoo
/// \date		2007-01-06 ~ 2007-01-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::ReleasePrepareTargetOneSkill(int nSkillNumber)
{ // ���� �ߵ����� Target�� �ִ� ��ų�� �����Ѵ�.
	CSkillInfo* pSkillInfo = FindUsingSkillInfo(nSkillNumber);
	if(pSkillInfo)
	{
		pSkillInfo->ChangeSkillState(SKILL_STATE_RELEASE);
		g_pGameMain->m_pInfSkill->DeleteSkillInfo(pSkillInfo);
		SafeDeleteSkillInfo(pSkillInfo);
		DeleteUsingSkillInfo(nSkillNumber);
	}
	
	// 2007-05-23 by bhsohn ��ų �ȳ����� ���� ó��
//	SetTargetIndex(0);				// �ʱ�ȭ
//	SetCharacterUID(0);				// �ʱ�ȭ
//	SetSkillTargetState(FALSE);		// ��ų ���콺 ������ ����
	InitTargetState();	// Ÿ���� ��ų�� �������� �ʱ�ȭ �Ѵ�.
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkill::InitPrepareTargetOneSkill()
/// \brief		Ÿ���� ��ų�� �������� �ʱ�ȭ �Ѵ�.
/// \author		// 2007-05-23 by bhsohn ��ų �ȳ����� ���� ó��
/// \date		2007-01-06 ~ 2007-01-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::InitTargetState()
{
	SetTargetIndex(0);				// �ʱ�ȭ
	SetCharacterUID(0);				// �ʱ�ȭ
	SetSkillTargetState(FALSE);		// ��ų ���콺 ������ ����
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::SafeDeleteSkillInfo(CSkillInfo* pSkillInfo)
/// \brief		���� ��밡���� ��ų����Ʈ�� �ִ� ��ų�����ΰ� ���Ͽ� ���� ��밡���� ��ų����Ʈ�� �ƴϸ� 
///				�ش� ��ų ������ ������ Clear�Ѵ�.
/// \author		jschoi
/// \date		2004-10-15 ~ 2004-10-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::SafeDeleteSkillInfo(CSkillInfo* pSkillInfo)
{// ���� ��밡���� ��ų����Ʈ�� �ִ� ��ų�����ΰ� ���Ͽ� ���� ��밡���� ��ų����Ʈ�� �ƴϸ� �ش� ��ų ������ ������ Clear�Ѵ�.

	map<int, CSkillInfo*>::iterator itSkillInfo = m_mapEnemySkillInfo.begin();
	for(; itSkillInfo != m_mapEnemySkillInfo.end(); itSkillInfo++)
	{
		if(pSkillInfo == itSkillInfo->second)
		{ // �ش� ��ų�� �ִ�.

			util::del(pSkillInfo);
			m_mapEnemySkillInfo.erase(itSkillInfo);
			return;
		}
	}	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::EraseUsingSkill(CSkillInfo* pSkillInfo)
/// \brief		��ų ������ m_vecUsingSkill����Ʈ���� ��� �����.
/// \author		jschoi
/// \date		2004-10-15 ~ 2004-10-15
/// \warning	
///
/// \param		pSkillInfo ������ ��ų�� �ּ�. bType FALSE = �ڽ� TRUE = �ٸ� ������ ������.
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::EraseUsingSkill(CSkillInfo* pSkillInfo,BOOL bEnemyDelete/* = FALSE*/)
{
	if(!bEnemyDelete)
	{
		vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
		while(itSkillInfo != m_vecUsingSkill.end())
		{
			if((*itSkillInfo)->ItemNum == pSkillInfo->ItemNum)
			{
				SafeDeleteSkillInfo(pSkillInfo);
				m_vecUsingSkill.erase(itSkillInfo);
				return;
			}
			itSkillInfo++;
		}
	}
	else
	{
		vector<CSkillInfo*>::iterator itSkillInfo = m_vecEnemyUsingSkillForMe.begin();
		while(itSkillInfo != m_vecEnemyUsingSkillForMe.end())
		{
			if((*itSkillInfo)->ItemNum == pSkillInfo->ItemNum)
			{
				SafeDeleteSkillInfo(pSkillInfo);
				m_vecEnemyUsingSkillForMe.erase(itSkillInfo);
				return;
			}
			itSkillInfo++;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�� ���� ���� ���� ������ ��ų�� ���´ٸ� �� ������ �����.
/// \author		// 2008-12-26 by bhsohn ����� ��� ���� ����
/// \date		2004-10-15 ~ 2004-10-15
/// \warning	
///
/// \param		pSkillInfo ������ ��ų�� �ּ�. bType FALSE = �ڽ� TRUE = �ٸ� ������ ������.
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::EraseLowLVUsingSkill(ClientIndex_t	i_AttackIndex, INT i_nSkillItemNum)
{
	if(i_AttackIndex == g_pShuttleChild->GetMyShuttleInfo().ClientIndex)
	{
		// ���� �� ������ ���� �ʿ䰡����.
		return;
	}
	INT nSkillItemNum = SKILL_BASE_NUM(i_nSkillItemNum);
	INT nSkillLevel = SKILL_LEVEL(i_nSkillItemNum);


	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		CSkillInfo* pSkillInfo = (*itSkillInfo);
		if(NULL == pSkillInfo)
		{			
			itSkillInfo++;
			continue;
		}
		INT nTmpSkillItemNum = SKILL_BASE_NUM(pSkillInfo->ItemNum);
		INT nTmpSkillLevel = SKILL_LEVEL(pSkillInfo->ItemNum);		
		
		if((nTmpSkillItemNum == nSkillItemNum)
			&&(nSkillLevel >= nTmpSkillLevel))
		{			
			pSkillInfo->SetSkillEnable(FALSE);
		}
		itSkillInfo++;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::FindSkillEffect(CUnitData* pUnit, int nEffectNum)
/// \brief		�ش� ��ų ����Ʈ�� �����Ѵ�.
/// \author		jschoi
/// \date		2004-12-08 ~ 2004-12-08
/// \warning	g_pD3dApp->m_pEffectList���� �ش� ����Ʈ�� ã�´�.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::DisableSkillEffect(CUnitData* pUnit, int nEffectNum)
{
	CAppEffectData* pEffect = (CAppEffectData*)g_pD3dApp->m_pEffectList->m_pChild;
	while(pEffect)
	{
		if( pEffect->m_pParent == (CAtumNode*)pUnit &&
			pEffect->m_nType == nEffectNum )		
		{
			pEffect->m_bUsing = FALSE;
			break;
		}
		pEffect = (CAppEffectData*)pEffect->m_pNext;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkill::DeleteUsingSkillInfo(int nSkillItemNum)
/// \brief		
/// \author		dgwoo
/// \date		2007-01-05 ~ 2007-01-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::DeleteUsingSkillInfo(int nSkillItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	for(; itSkillInfo != m_vecUsingSkill.end(); itSkillInfo++)
	{
		if((*itSkillInfo)->ItemNum == nSkillItemNum)
		{
			m_vecUsingSkill.erase(itSkillInfo);
			return TRUE;
		}		
	}

	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkill::DeleteUsingSkillInfoByBaseNum(int nSkillItemNum)
/// \brief		
/// \author		dgwoo
/// \date		2007-01-05 ~ 2007-01-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::DeleteUsingSkillInfoByBaseNum(int nSkillItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	for(; itSkillInfo != m_vecUsingSkill.end(); itSkillInfo++)
	{
		if(SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == SKILL_BASE_NUM(nSkillItemNum))
		{
			m_vecUsingSkill.erase(itSkillInfo);
			return TRUE;
		}		
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkill::DeleteEnemyUsingSkillInfo(int nSkillItemNum)
/// \brief		
/// \author		dgwoo
/// \date		2007-01-05 ~ 2007-01-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::DeleteEnemyUsingSkillInfo(int nSkillItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecEnemyUsingSkillForMe.begin();
	for(; itSkillInfo != m_vecEnemyUsingSkillForMe.end(); itSkillInfo++)
	{
		if((*itSkillInfo)->ItemNum == nSkillItemNum)
		{
			m_vecEnemyUsingSkillForMe.erase(itSkillInfo);
			return TRUE;
		}		
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkill::DeleteEnemyUsingSkillInfoByBaseNum(int nSkillItemNum)
/// \brief		
/// \author		dgwoo
/// \date		2007-01-05 ~ 2007-01-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::DeleteEnemyUsingSkillInfoByBaseNum(int nSkillItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecEnemyUsingSkillForMe.begin();
	for(; itSkillInfo != m_vecEnemyUsingSkillForMe.end(); itSkillInfo++)
	{
		if(SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == SKILL_BASE_NUM(nSkillItemNum))
		{
			m_vecEnemyUsingSkillForMe.erase(itSkillInfo);
			return TRUE;
		}		
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::FindListTargetForSkill(POINT pt)
/// \brief		��� ����Ʈ���� ã��(Ŭ������ ����)
/// \author		ispark
/// \date		2005-11-24 ~ 2005-11-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::FindListTargetForSkill(POINT pt)
{
	// Ŭ������ �����
	int i = 0;
	D3DXVECTOR2 vPos1 = D3DXVECTOR2(pt.x,pt.y);
	vector<PARTYENEMYINFO*>::iterator itParty = g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.begin();
	while(itParty != g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.end())
	{
		// ����Ʈ Ŭ��
		// 2008-07-10 by bhsohn Ÿ������ �ȵǴ� ���� �ذ�
//		if(pt.x > 0 && pt.x < 130 &&
//			pt.y > 170 + 34 * i &&
//			pt.y < 204 + 34 * i)
		if(pt.x > 0 && pt.x < 130 &&
			pt.y > FRAME_START_Y + PARTY_FRAME_SIZE_Y * i &&
			pt.y < (FRAME_START_Y + PARTY_FRAME_SIZE_Y) + (PARTY_FRAME_SIZE_Y * i))
		{
			// �����ʿ� ������
			if((*itParty)->m_bPartyType == _PARTYMASTER ||
				(*itParty)->m_bPartyType == _PARTYMEMBER)
			{
				CSkillInfo* pSkillInfo = NULL;
				if(pSkillInfo = CheckConfirmSkillUse())
				{
					// ����㰡�� �ʿ��� ��ų�̴�.

					// 2008. 12. 23 by ckPark ��ų ��� ���� ��� ����
					//SendConfirmSkillUse(pSkillInfo, (*itParty)->m_ImPartyMemberInfo.CharacterUniqueNumber);
					g_pGameMain->PushDelaySkill(pSkillInfo, (*itParty)->m_ImPartyMemberInfo.CharacterUniqueNumber);
					// end 2008. 12. 23 by ckPark ��ų ��� ���� ��� ����



					return TRUE;
				}

				SetTargetIndex(0);
				SetCharacterUID((*itParty)->m_ImPartyMemberInfo.CharacterUniqueNumber);

				return TRUE;
			}
			// �ٸ��ʿ� ������
			else if((*itParty)->m_bPartyType == _PARTYOTHERMAPMASTER ||
				(*itParty)->m_bPartyType == _PARTYOTHERMAPMEMBER)
			{
				CSkillInfo* pSkillInfo = NULL;
				if(pSkillInfo = CheckConfirmSkillUse())
				{
					// ����㰡�� �ʿ��� ��ų�̴�.



					// 2008. 12. 23 by ckPark ��ų ��� ���� ��� ����
					//SendConfirmSkillUse(pSkillInfo, (*itParty)->m_ImPartyMemberInfo.CharacterUniqueNumber);
					g_pGameMain->PushDelaySkill(pSkillInfo, (*itParty)->m_ImPartyMemberInfo.CharacterUniqueNumber);
					// end 2008. 12. 23 by ckPark ��ų ��� ���� ��� ����



					return TRUE;
				}
				// 2010. 06. 18 by jskim �ݿ��� ����� ������ SP �����ϴ� ���� ����
				SetTargetIndex(0);
				SetCharacterUID((*itParty)->m_ImPartyMemberInfo.CharacterUniqueNumber);
				return TRUE;
				//end 2010. 06. 18 by jskim �ݿ��� ����� ������ SP �����ϴ� ���� ����
			}
			// ����...
			else if((*itParty)->m_bPartyType == _PARTYRESTARTMEMBER)
			{

			}
		}
		else // ���� �Ϲ� Ÿ�� Ŭ��
		{
			// �����ʿ� ������
			if((*itParty)->m_bPartyType == _PARTYMASTER ||
				(*itParty)->m_bPartyType == _PARTYMEMBER)
			{
			if((*itParty)->m_pEnemyData->m_nObjScreenW > 0)
			{
				float fLength = 50.0f;
				D3DXVECTOR2 vPos2 = D3DXVECTOR2((*itParty)->m_pEnemyData->m_nObjScreenX, (*itParty)->m_pEnemyData->m_nObjScreenY);
				float fLengthTemp = D3DXVec2Length(&(vPos1-vPos2));
				if(fLengthTemp < fLength)
				{
					CSkillInfo* pSkillInfo = NULL;
					if(pSkillInfo = CheckConfirmSkillUse())
					{
						// ����㰡�� �ʿ��� ��ų�̴�.



						// 2008. 12. 23 by ckPark ��ų ��� ���� ��� ����
						//SendConfirmSkillUse(pSkillInfo, (*itParty)->m_ImPartyMemberInfo.CharacterUniqueNumber);
						g_pGameMain->PushDelaySkill(pSkillInfo, (*itParty)->m_ImPartyMemberInfo.CharacterUniqueNumber);
						// end 2008. 12. 23 by ckPark ��ų ��� ���� ��� ����



						return TRUE;
					}
				}
				}
			}
			// �ٸ��ʿ� ������
			else if((*itParty)->m_bPartyType == _PARTYOTHERMAPMASTER ||
				(*itParty)->m_bPartyType == _PARTYOTHERMAPMEMBER)
			{
				
			}
			// ����...
			else if((*itParty)->m_bPartyType == _PARTYRESTARTMEMBER)
			{

			}
		}
		
		i++;
		itParty++;
	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::ReleaseAllUsingControlSkill(int i_nExcludeSkillNum)
/// \brief		��Ʈ�� ��ų ��� ����, m_vecUsingSkill������ ������ �ʴ´�.
///				�ߺ� ���� ��ߵ� �ð��� �߰����� �����ȴ�.
/// \author		ispark
/// \date		2005-11-25 ~ 2005-11-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::ReleaseAllUsingControlSkill(int i_nExcludeSkillNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{

		if((*itSkillInfo)->ItemNum != i_nExcludeSkillNum 
			&& (*itSkillInfo)->ItemInfo->SkillType == SKILLTYPE_CLICK
			&& SKILL_KIND((*itSkillInfo)->ItemNum) == SKILL_KIND_CONTROL)
		{
			(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
			SafeDeleteSkillInfo(*itSkillInfo);

			g_pShuttleChild->DeletePattern();				
			g_pShuttleChild->m_pSkill->SetSkillPatternType(0);	// ���� �ʱ�ȭ
			// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
//			g_pShuttleChild->m_pSkill->DisableSkillEffect(g_pShuttleChild,SKILL_BASE_NUM((*itSkillInfo)->ItemInfo->SourceIndex));
			g_pShuttleChild->m_pSkill->DisableSkillEffect(g_pShuttleChild,SKILL_BASE_SOURCEINDEX((*itSkillInfo)->ItemInfo->SourceIndex));

		}

		itSkillInfo++;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::IsExistMultyNGround()
/// \brief		��ƼŸ�ٰ� ���������� ���� �� �� ����. �����ϴ��� �˻�
/// \author		ispark
/// \date		2005-11-25 ~ 2005-11-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsExistMultyNGround()
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if(SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == BGEAR_SKILL_BASENUM_GROUNDBOMBINGMODE ||
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == BGEAR_SKILL_BASENUM_AIRBOMBINGMODE ||
			(FindDestParamUseSkill((*itSkillInfo)->ItemInfo, DES_MULTITAGET_01) ||
			FindDestParamUseSkill((*itSkillInfo)->ItemInfo, DES_MULTITAGET_02)))
		{
			return TRUE;
		}

		itSkillInfo++;
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::FindDestParamUseSkill(ITEM *pItem, BYTE byDestParameter)
/// \brief		���� DestParameter���� ��ų DestParameter�� ���ؼ� �������� �����ϸ� BOOL�� ����
/// \author		ispark
/// \date		2005-11-25 ~ 2005-11-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2013-03-27 by bhsohn DestParam �ڷ��� ����
//BOOL CSkill::FindDestParamUseSkill(ITEM *pItem, BYTE byDestParameter)
BOOL CSkill::FindDestParamUseSkill(ITEM *pItem, DestParam_t byDestParameter)
{
	// 2009-04-21 by bhsohn ������ DesParam�߰�
// 	if(pItem->DestParameter1 == byDestParameter ||
// 	   pItem->DestParameter2 == byDestParameter ||
// 	   pItem->DestParameter3 == byDestParameter ||
// 	   pItem->DestParameter4 == byDestParameter)
// 	{
// 		return TRUE;
// 	}
	int nArrParamCnt = 0;
	for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
	{
		if(pItem->ArrDestParameter[nArrParamCnt] == byDestParameter)
		{
			return TRUE;
		}
	}
	// end 2009-04-21 by bhsohn ������ DesParam�߰�
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkill::IsGroundSkill(CSkillInfo *pItemSkill)
/// \brief		���󿡼� ����ϴ� ��ų�̳�?
/// \author		dgwoo
/// \date		2007-01-09 ~ 2007-01-09
/// \warning	
///
/// \param		
/// \return		TRUE ����� ��ų.
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsGroundSkill(CSkillInfo *pItemSkill)
{
	if((SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_GROUNDACCELERATOR ||
		SKILL_BASE_NUM(pItemSkill->ItemNum) == FLASH_ACCELERATOR ||
		SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEMODE ||
		SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEDEFENSEMODE||
		FindDestParamUseSkill(pItemSkill->ItemInfo, DES_SKILL_BARRIER) ||
		FindDestParamUseSkill(pItemSkill->ItemInfo, DES_SKILL_CAMOUFLAGE)))
		return TRUE;
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::IsUseSkill(CSkillInfo *pItemSkill)
/// \brief		������ ��ų�� ��� �����Ѱ�?
/// \author		ispark
/// \date		2005-11-26 ~ 2005-11-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsUseSkill(CSkillInfo *pItemSkill)
{
	// 2007-02-01 by dgwoo ���� �ʿ��� ��ų�� ����Ҽ� ����.
	if(IS_CITY_MAP_INDEX(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex)
		&& !IS_BAZAAR_SKILL(pItemSkill->ItemInfo))
	{
		return TRUE;
	}
	// 2010. 06. 29 by jskim ���Ի��� ���� �κ� ���� ���� üũ
	if(g_pGameMain->m_pInven->GetInvenFreeSize() <= 0 && pItemSkill->ItemInfo->IsExistDesParam(DES_BAZAAR_BUY))
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100629_0401,COLOR_ERROR);
		return TRUE;
	}
	//end 2010. 06. 29 by jskim ���Ի��� ���� �κ� ���� ���� üũ
	if(((g_pShuttleChild->m_dwState != _NORMAL || g_pShuttleChild->m_bUnitStop == TRUE || g_pShuttleChild->m_bLandedMove == TRUE || g_pShuttleChild->m_bPenaltyUnitStop == TRUE) || // 2005-11-25 by ispark ĳ������ ���� �������� ��ų ���� �ʱ�
		(IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) && g_pShuttleChild->m_bIsAir == FALSE)) &&
		pItemSkill->ItemInfo->SkillType == SKILLTYPE_CLICK &&
		SKILL_KIND(pItemSkill->ItemNum) == SKILL_KIND_CONTROL)	// ������ �ִ� ��ų�� m_dwState�� _NORMAL ���¿��� ��밡���ϴ�.
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0002,COLOR_SKILL_CANCEL);//"��ų�� ����� �� ���� �����Դϴ�."
		return TRUE;
	}

	// 2006-09-27 by ispark ���� ���̸鼭 ��Ʈ�� ��ų�� ��� �� �� ����.
	if(g_pShuttleChild->m_bSkillMoveIsUse == TRUE && 
		SKILL_KIND(pItemSkill->ItemNum) == SKILL_KIND_CONTROL)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0002,COLOR_SKILL_CANCEL);//"��ų�� ����� �� ���� �����Դϴ�."
		return TRUE;
	}

	if( pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ME &&
		pItemSkill->ItemInfo->ReqItemKind != ITEMKIND_ALL_ITEM &&
		g_pStoreData->IsWearItem( pItemSkill->ItemInfo->ReqItemKind ) == FALSE &&
		pItemSkill->ItemInfo->SkillType != SKILLTYPE_PERMANENT)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0003,COLOR_SKILL_CANCEL);//"��ų�� ������ ���Ⱑ �����Ǿ����� �ʽ��ϴ�."
		return TRUE;
	}

	if(	g_pShuttleChild->m_bIsAir == TRUE && IsGroundSkill(pItemSkill))
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0004,COLOR_SKILL_CANCEL);//"���󿡼��� ��밡���� ��ų�Դϴ�."
		return TRUE;
	}
	// 2006-11-16 by dgwoo Air Siege
	if(	!g_pShuttleChild->m_bIsAir&&
		(SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_AIRSIEGEMODE))
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_061206_0100,COLOR_SKILL_CANCEL);//"���߿����� ��밡���� ��ų�Դϴ�."
		return TRUE;
	}
	// 2006-11-27 by dgwoo ���� ���¿��� ����Ҽ� ���� ��ų��
	// 2009-01-12 by bhsohn A���, ���� ���� ���¿��� ���� ����� ���� �ȵǴ� ���� �ذ�
// 	if((SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_AIRSIEGEMODE) &&
// 		g_pShuttleChild->m_bUnitStop == TRUE)
// 	{
// 		char chMsg[512] = {0,};
// 		wsprintf(chMsg,STRMSG_C_061206_0101, pItemSkill->ItemInfo->ItemName);
// 		g_pD3dApp->m_pChat->CreateChatChild(chMsg,COLOR_SKILL_CANCEL);
// 		return TRUE;
// 	}
	if(SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_AIRSIEGEMODE)
	{
		if((g_pShuttleChild->m_bUnitStop == TRUE)
			||(g_pD3dApp->m_bFixUnitDirMode))
		{
			char chMsg[512] = {0,};
			wsprintf(chMsg,STRMSG_C_061206_0101, pItemSkill->ItemInfo->ItemName);
			g_pD3dApp->m_pChat->CreateChatChild(chMsg,COLOR_SKILL_CANCEL);
			return TRUE;
		}
	}
	// end 2009-01-12 by bhsohn A���, ���� ���� ���¿��� ���� ����� ���� �ȵǴ� ���� �ذ�

	// ��Ƽ �ÿ��� ��� ������ ��ų (���� �����ϰ� �ٸ� ��Ƽ���� ������ �� ��� ����)
#ifdef FREEWAR_
	if (!isFreeForAllMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
	{
		if ((pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_WITH_ME ||
			pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_WITHOUT_ME) &&
			//g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.size() == 0 )
			!g_pShuttleChild->m_pClientParty->IsParty())
		{
			g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0005, COLOR_SKILL_CANCEL);//"���ÿ��� ��밡���� ��ų�Դϴ�."
			return TRUE;
		}
	}
#endif	
	if( IsSingleSkill(pItemSkill->ItemNum) && IsExistSingleSkill())
	{
		return TRUE;
	}
	
	// 2006-10-26 by ispark, Ÿ���� ��ų�� ���� �ϳ��� �۵�
	if((pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE ||
			pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCLUDE_ME ||
			pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE ||
			pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME ||
			pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCEPT_OURS) &&
		IsExistTargetOneSkill(pItemSkill))
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0006,COLOR_SKILL_CANCEL);//"�ٸ� Ÿ���� ��ų�� �ߵ����Դϴ�."
		return TRUE;
	}
	// 2006-12-21 by dgwoo Ÿ���� ��ų�� ��¡�� ��ų�� ���ÿ� ����Ҽ�����.
	if(((GetChargingShotState() <= SKILL_STATE_USING) &&
		(GetChargingShotState() > SKILL_STATE_READY)) &&
		pItemSkill->IsMustOneTargetSkill(pItemSkill->ItemInfo->SkillTargetType))
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_061221_0100,COLOR_SKILL_CANCEL);
		return TRUE;
	}
	
	if( pItemSkill->ItemInfo->ReqSP > g_pShuttleChild->m_myShuttleInfo.CurrentSP )
	{	
		g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0007,COLOR_SKILL_CANCEL);//"��ų����Ʈ�� �����մϴ�."
		return TRUE;
	}

	// ������ �̸鼭 �����ΰ�쿡�� ���� ��ų�� ����ϸ� ���������� Ż��ȴ�.
	if( pItemSkill->ItemInfo->SkillType == SKILLTYPE_CLICK &&
		SKILL_KIND(pItemSkill->ItemNum) == SKILL_KIND_CONTROL &&
		g_pShuttleChild->m_pClientParty &&
		g_pShuttleChild->m_pClientParty->IsFormationFlightMove())
	{
		g_pShuttleChild->SendPacket(T_IC_PARTY_CANCEL_FLIGHT_POSITION);
		g_pShuttleChild->m_pClientParty->FormationFlightClear();
	}
	// 2005-12-17 by ispark
	// ������̸鼭 �������� �� ���� ��ų ���� ������ ������������ ����
	else if(pItemSkill->ItemInfo->SkillType == SKILLTYPE_CLICK &&
		SKILL_KIND(pItemSkill->ItemNum) == SKILL_KIND_CONTROL &&
		g_pShuttleChild->m_pClientParty &&
		g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER &&
		g_pShuttleChild->m_pClientParty->IsFormationFlight())
	{
		// 2008-06-17 by bhsohn ��� ���� ó��
		//g_pShuttleChild->m_pClientParty->ISendPartyChangeFlightFormation(FLIGHT_FORM_NONE);
		g_pShuttleChild->m_pClientParty->TempPartyFormation(FLIGHT_FORM_NONE);
	}

	// 2005-11-25 by ispark
	// ��ƼŸ�ٰ� ���������� ���� ��� ���ϰ� ��
	if((SKILL_BASE_NUM(pItemSkill->ItemNum) == BGEAR_SKILL_BASENUM_GROUNDBOMBINGMODE ||
		SKILL_BASE_NUM(pItemSkill->ItemNum) == BGEAR_SKILL_BASENUM_AIRBOMBINGMODE ||
		(FindDestParamUseSkill(pItemSkill->ItemInfo, DES_MULTITAGET_01) ||
		FindDestParamUseSkill(pItemSkill->ItemInfo, DES_MULTITAGET_02))) &&
		IsExistMultyNGround())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051125_0002,COLOR_SKILL_CANCEL);// "��ƼŸ�ٰ� ���������� ���� �� �� �����ϴ�."
		return TRUE;		
	}

	// 2006-07-28 by ispark
	// ���λ��� ����� ������ ��������
// 2007-06-01 by dgwoo ���ڸ����������� ��밡�� �ϸ� ������ ���ѵ� Ǯ���� ����.
	if(!g_pD3dApp->m_bCharacter && 
		IS_BAZAAR_SKILL(pItemSkill->ItemInfo))
//		&& !g_pCharacterChild->GetbBazaarEvent())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_060728_0002,COLOR_SKILL_CANCEL);// "�̰������� ������ ���� �� �� �����ϴ�."
		return TRUE;
	}

	// 2006-08-01 by ispark
	// ��� �������̸� ���λ��������� �ڵ� Ż��
	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType != _NOPARTY && 
		g_pD3dApp->m_bCharacter && 
		IS_BAZAAR_SKILL(pItemSkill->ItemInfo) &&
		g_pCharacterChild->GetbBazaarEvent())
	{
		g_pShuttleChild->m_pClientParty->ISendPartyLeave();
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_PARTY_0007, COLOR_SKILL_CANCEL);//"��뿡�� Ż�� �Ͽ����ϴ�."
	}

	// 2006-11-17 by ispark
	// �̺�Ʈ�ʿ����� ��� �� �� ���� ��ų
	if(!IsUseEventMap(pItemSkill))
	{
		char chMsg[512] = {0,};
		wsprintf(chMsg, STRMSG_C_061117_0001, pItemSkill->ItemInfo->ItemName);	// "�̺�Ʈ�ʿ����� %s ��ų�� ����� �� �����ϴ�."
		g_pD3dApp->m_pChat->CreateChatChild(chMsg, COLOR_SKILL_CANCEL);
		return TRUE;
	}
	if(!IsSkillPossibility())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_061206_0102, COLOR_SKILL_CANCEL);	// "[�κ�����]��ų ����߿��� ��ų �ߵ��� �Ҽ� �����ϴ�."
		return TRUE;
	}
// 2011-10110 by jhahn EP4 ħ�� ��ų �߰�
	if(!IsSkillPossibilitySlience())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_061206_0102, COLOR_SKILL_CANCEL);	// "ħ�� ��ų ����߿��� ��ų �ߵ��� �Ҽ� �����ϴ�."
		return TRUE;
	}									 
//end 2011-10110 by jhahn EP4 ħ�� ��ų �߰�
// 2007-02-01 by dgwoo ��������ų�� ���ֽ�ų �ߺ������ �����ϴ�.
//	if(	FindUsingSkillInfoByBaseNum(IGEAR_SKILL_BASENUM_BERSERKER) && 
//		SKILL_BASE_NUM(pItemSkill->ItemNum) == IGEAR_SKILL_BASENUM_FRENZY)
//	{
//		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_061212_0100, COLOR_SKILL_CANCEL);	//"�ش罺ų�� ������� �ߺ������ �Ұ����մϴ�."
//		return TRUE;
//	}

	// 2009. 08. 20 by ckPark ���� ���¿��� ������� ��ų ������ ����
	if( g_pShuttleChild->m_dwState == _FALLING || g_pShuttleChild->m_dwState == _FALLEN)
	{
		g_pD3dApp->m_pChat->CreateChatChild( STRERR_C_SKILL_0002, COLOR_SKILL_CANCEL );	//"��ų�� ����� �� ���� �����Դϴ�."
		return TRUE;
	}
	// end 2009. 08. 20 by ckPark ���� ���¿��� ������� ��ų ������ ����

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::IsSkillPossibility()
/// \brief		��ų�� ����Ҽ� �ִ°�? ��ҵ鿡 ���� ��ų ����� ������ �Ǵ�.
/// \author		dgwoo
/// \date		2006-11-28 ~ 2006-11-28
/// \warning	�߰� �Ǵ� ������ ��� ����.
///
/// \param		
/// \return		TRUE: ��ų��밡�� FALSE : ��ų ����� ����.
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsSkillPossibility()
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if(BGEAR_SKILL_BASENUM_INVISIBLE == SKILL_BASE_NUM((*itSkillInfo)->ItemNum))
		{
			return FALSE;
		}
		itSkillInfo++;
	}	
	return TRUE;
}
// 2011-10110 by jhahn EP4 ħ�� ��ų �߰�
///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::IsSkillPossibilitySlience()
/// \brief		
/// \author		jhahn
/// \date		2011-11-28 ~ 2011-11-28
/// \warning	
///
/// \param		
/// \return		TRUE: ��ų��밡�� FALSE : ��ų ����� ����.
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsSkillPossibilitySlience()
{
	if(g_pShuttleChild->GetSkillMissileWarning() == TRUE)
	{
		 return FALSE;
	}
	 return TRUE;
}										 
//end 2011-10110 by jhahn EP4 ħ�� ��ų �߰�
///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::IsSkillPossibility()
/// \brief		
/// \author		dgwoo
/// \date		2008-01-08 ~ 2008-01-08
/// \warning	
///
/// \param		
/// \return		TRUE: ��ų��밡�� FALSE : ��ų ����� ����.
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsAttackPossibility()
{
	// �κ����� ���´� ������ �Ұ����� ���´�
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if(BGEAR_SKILL_BASENUM_INVISIBLE == SKILL_BASE_NUM((*itSkillInfo)->ItemNum))
		{
			return FALSE;
		}
		itSkillInfo++;
	}	
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::IsSkillOverBooster()
/// \brief		�����ν��͸� ������ ã�´�. ������ Ư���� ����Ʈ�� ȿ���� ���� ���ؼ�
/// \author		ispark
/// \date		2005-11-29 ~ 2005-11-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsSkillOverBooster()
{
	for (auto pSkillInfo : m_vecUsingSkill)
	{
		if(
			SKILL_KIND(pSkillInfo->ItemNum) == SKILL_KIND_CONTROL
			&& (pSkillInfo->ItemNum / 10) == 783203						  // �����ν���

// 2010-12-28 by hsson �����ν��� ��ų ����
			&& pSkillInfo->GetSkillState() == SKILL_STATE_USING       // ��ų�� ������̸�
// end 2010-12-28 by hsson �����ν��� ��ų ����

			)
		{
			return TRUE;
		}
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::UseSkillConfirm(int i)
/// \brief		��� �㰡 Ȯ��
/// \author		ispark
/// \date		2005-12-03 ~ 2005-12-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::UseSkillConfirm(int i)
{
	MSG_FC_SKILL_CONFIRM_USE_ACK sMsg;
	if(i == 0)
	{
		sMsg.bYesOrNo = TRUE;				// ����
		sMsg.AttackCharacterUID = m_stSkillConfirm.AttackCharacterUID;		// ��ų ����� CharacterUID
		sMsg.TargetCharacterUID = m_stSkillConfirm.TargetCharacterUID;		// ��ų Ÿ�� CharacterUID
		sMsg.UsingSkillItemNum = m_stSkillConfirm.UsingSkillItemNum;		// ��� ��ų ItemNum
	}
	else
	{
		sMsg.bYesOrNo = FALSE;				// �ź�
		sMsg.AttackCharacterUID = m_stSkillConfirm.AttackCharacterUID;		// ��ų ����� CharacterUID
		sMsg.TargetCharacterUID = m_stSkillConfirm.TargetCharacterUID;		// ��ų Ÿ�� CharacterUID
		sMsg.UsingSkillItemNum = m_stSkillConfirm.UsingSkillItemNum;		// ��� ��ų ItemNum
	}

	// 2009. 04. 06 by ckPark �ݿ�������ν� �ٸ� Ÿ���� ��ų ������ ����
	sMsg.SkillConfirmUseUID	= m_stSkillConfirm.SkillConfirmUseUID;
	// end 2009. 04. 06 by ckPark �ݿ�������ν� �ٸ� Ÿ���� ��ų ������ ����

	g_pFieldWinSocket->SendMsg(T_FC_SKILL_CONFIRM_USE_ACK, (char*)&sMsg, sizeof(sMsg));
	memset(&m_stSkillConfirm, 0, sizeof(MSG_FC_SKILL_CONFIRM_USE));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::SetSkillConfirmData(MSG_FC_SKILL_CONFIRM_USE *pMsg)
/// \brief		��ų ��� ���� ����Ÿ �ӽ� ����
/// \author		ispark
/// \date		2005-12-03 ~ 2005-12-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::SetSkillConfirmData(MSG_FC_SKILL_CONFIRM_USE *pMsg)
{
	memcpy(&m_stSkillConfirm, pMsg, sizeof(MSG_FC_SKILL_CONFIRM_USE));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::CheckConfirmSkillUse()
/// \brief		����㰡�� �ʿ��� ��ų���� üũ, �׷��ٸ� ���� ó��
/// \author		ispark
/// \date		2005-12-03 ~ 2005-12-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CSkillInfo* CSkill::CheckConfirmSkillUse()
{
	vector<CSkillInfo*>::iterator itvecSkillInfo = m_vecUsingSkill.begin();
	while(itvecSkillInfo != m_vecUsingSkill.end())
	{
		// 2006-10-26 by ispark
		// �ݿ��������
//		if(FindDestParamUseSkill((*itvecSkillInfo)->ItemInfo, DES_SKILL_SUMMON_FORMATION_MEMBER) &&
//			(*itvecSkillInfo)->GetCheckAckWaitingTime() == 0)
		// 2007-12-11 by dgwoo �ݿ��� ����� ��ų�� ��� �Ҽ� ������ �ٸ� Ÿ���� ��ų�� 
		if(FindDestParamUseSkill((*itvecSkillInfo)->ItemInfo, DES_SKILL_SUMMON_FORMATION_MEMBER) &&
			(*itvecSkillInfo)->GetSkillState() == SKILL_STATE_PREPARE)
		{
			//DbgOut("CheckConfirmSkillUse() %d\n",(*itvecSkillInfo)->GetSkillState());
			return (*itvecSkillInfo);
		}
		itvecSkillInfo++;
	}

	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkillInfo* CSkill::FindItemEnemySkillInfo(int nSkillItemNumber)
/// \brief		
/// \author		dgwoo
/// \date		2007-01-05 ~ 2007-01-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CSkillInfo* CSkill::FindItemEnemySkillInfo(int nSkillItemNumber)
{
	FLOG( "CSkill::FindItemSkill(int nSkillItemNumber)" );
	map<int, CSkillInfo*>::iterator itSkillInfo = m_mapEnemySkillInfo.find(nSkillItemNumber);
	if(itSkillInfo != m_mapEnemySkillInfo.end())
	{	
		if(nSkillItemNumber == itSkillInfo->second->ItemNum)
		{
			return itSkillInfo->second;
		}
	}
	return NULL;
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::SendConfirmSkillUse(CSkillInfo* pSkillInfo, UID32_t TargetCharacterUniqueNumber)
/// \brief		����㰡 ��ų �޼��� ������
/// \author		ispark
/// \date		2005-12-06 ~ 2005-12-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::SendConfirmSkillUse(CSkillInfo* pSkillInfo, UID32_t TargetCharacterUniqueNumber)
{
	MSG_FC_SKILL_CONFIRM_USE sMsg;
	wsprintf(sMsg.szAttackCharacterName, "%s", g_pShuttleChild->m_myShuttleInfo.CharacterName);		// ��ų ������� CharacterName;
	sMsg.AttackCharacterUID = g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;				// ��ų ����� CharacterUID
	sMsg.TargetCharacterUID = TargetCharacterUniqueNumber;											// ��ų Ÿ�� CharacterUID
	sMsg.UsingSkillItemNum = pSkillInfo->ItemInfo->ItemNum;											// ��� ��ų ItemNum
	sMsg.MapChannelIndex = g_pShuttleChild->m_myShuttleInfo.MapChannelIndex;						// ��ų ������� MapChannelIndex
	g_pFieldWinSocket->SendMsg(T_FC_SKILL_CONFIRM_USE, (char*)&sMsg, sizeof(sMsg));
	pSkillInfo->SetCheckAckWaitingTime(12);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkill::IsUseEventMap(CSkillInfo* pItemSkill)
/// \brief		�̺�Ʈ �ʿ��� ����� �� �� �ִ� ��ų�ΰ�?
/// \author		ispark
/// \date		2006-11-17 ~ 2006-11-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsUseEventMap(CSkillInfo* pItemSkill)
{
	MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
	
	// 2006-11-17 by ispark, �̺�Ʈ �ʿ����� ����
	if(IS_MAP_INFLUENCE_EVENT_AREA(pMapInfo->MapInfluenceType) &&
		FindDestParamUseSkill(pItemSkill->ItemInfo, DES_SKILL_SUMMON_FORMATION_MEMBER))
	{
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkill::IsCancelSkill(CSkillInfo* pSkillInfo)
/// \brief		��ȭ ��ų�߿� ��ҵ� ��ų�ΰ�.
/// \author		dgwoo
/// \date		2007-01-29 ~ 2007-01-29
/// \warning	
///
/// \param		
/// \return		��ȭ ��ų�� ���� ��ҵ� ��ų���� �˾ƿ´�.
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsCancelSkill(CSkillInfo* pSkillInfo)
{
	// 2007-01-29 by dgwoo 10�� �̸��� ��ų�� ��ߵ��ð����� �ʱ�ȭ�Ѵ�.
	// 2009. 04. 06 by ckPark ���� ��ų ���� �Ӽ� �߰�
// 	if(pSkillInfo->GetSkillState() == SKILL_STATE_USING
// 		&& pSkillInfo->ItemInfo->ReAttacktime < TERM_TICK_MUST_SERVER_CHECK_SKILL_REATTACK_TIME)
	if(pSkillInfo->GetSkillState() == SKILL_STATE_USING
		&& !COMPARE_BIT_FLAG(pSkillInfo->ItemInfo->ItemAttribute, SKILL_ATTR_STORE_USING_TIME))
	// end 2009. 04. 06 by ckPark ���� ��ų ���� �Ӽ� �߰�
	{
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	CSkill::GetPreSkillItemNum()
/// \brief		������ ��û�ϴ� ���� ���
/// \author		// 2007-04-05 by bhsohn ��� ���� ó��
/// \date		2007-04-05 ~ 2007-04-05
/// \warning	
///
/// \param		
/// \return		��ȭ ��ų�� ���� ��ҵ� ��ų���� �˾ƿ´�.
///////////////////////////////////////////////////////////////////////////////
INT	CSkill::GetPreSkillItemNum()
{	
	return m_nPreSkillItemNum;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkill::InitPreSkillItemNum()
/// \brief		������� �ʱ�ȭ
/// \author		// 2007-04-05 by bhsohn ��� ���� ó��
/// \date		2007-04-05 ~ 2007-04-05
/// \warning	
///
/// \param		
/// \return		��ȭ ��ų�� ���� ��ҵ� ��ų���� �˾ƿ´�.
///////////////////////////////////////////////////////////////////////////////
void CSkill::InitPreSkillItemNum()
{
	m_nPreSkillItemNum = 0;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkill::SetScanSkill(int nSkillIdx, BOOL bScan)
/// \brief		������� �ʱ�ȭ
/// \author		// 2007-04-19 by bhsohn ��ġ���� ������ �߰�
/// \date		2007-04-19 ~ 2007-04-19
/// \warning	
///
/// \param		
/// \return		��ȭ ��ų�� ���� ��ҵ� ��ų���� �˾ƿ´�.
///////////////////////////////////////////////////////////////////////////////
void CSkill::SetScanSkill(int nIdx, BOOL bScan)
{
	if(nIdx >= MAX_SCAN )
	{
		return;
	}
	if(bScan)
	{
		m_nScanState |= (1<< nIdx);		
	}
	else
	{
		m_nScanState &= ~(1<< nIdx);
	}

	// ���� ��ĵ�� ������ �ֳ�?
	if(m_nScanState)
	{
		m_bScan	= TRUE;
	}
	else
	{
		m_bScan	= FALSE;
	}
}

void CSkill::SetScanPosition(int nIdx, D3DXVECTOR3	vScanPosition)
{
	if(nIdx >= MAX_SCAN )
	{
		return;
	}
	m_vScanPosition[nIdx] = vScanPosition;
}

float CSkill::GetScanRange(int nIdx)								
{
	if(nIdx >= MAX_SCAN )
	{
		return 0;
	}
	return m_fScanRange[nIdx];
}
void CSkill::SetScanRange(int nIdx, float ScanRange)				
{
	if(nIdx >= MAX_SCAN )
	{
		return;
	}
	m_fScanRange[nIdx] = ScanRange;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkillInfo::PrepareSkillFromServer()
/// \brief		
/// \author		// 2007-04-19 by bhsohn ��ġ���� ������ �߰�
/// \date		2007-04-20 ~ 2007-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsCanSeeInvisible(D3DXVECTOR3 vPos)
{
	BOOL bCanSeeInvisible = FALSE;
	if(FALSE == ISScanSkill())
	{
		// ��ĵ ���°� �ƴϸ� �� ����. 
		return bCanSeeInvisible;
	}
	FLOAT fLength = 0;
	float fScanRange =0;
	for(int nIdx = 0;nIdx < MAX_SCAN;nIdx++)
	{
		fScanRange = GetScanRange(nIdx);
// 2012-11-29 by mspark, ��ġ���� ��ġ�� �̵��ϴ� ĳ������ ��ġ�� �����ϵ��� ����
#ifdef C_SEARCHEYE_SHUTTLE_POSITION_MSPARK
		if(SCAN_ITEM == nIdx || SCAN_ITEM2 == nIdx)
		{
			fLength = D3DXVec3Length(&(g_pShuttleChild->m_vPos - vPos));
		}
		else
		{
		fLength = D3DXVec3Length(&(m_vScanPosition[nIdx] - vPos));
		}		
#else
		fLength = D3DXVec3Length(&(m_vScanPosition[nIdx] - vPos));
#endif
// end 2012-11-29 by mspark, ��ġ���� ��ġ�� �̵��ϴ� ĳ������ ��ġ�� �����ϵ��� ����
		if(fLength <= fScanRange)
		{
			// ��ĵ �Ÿ� �ȿ� ���Դ�
			bCanSeeInvisible= TRUE;
			return bCanSeeInvisible;
		}
	}
	return bCanSeeInvisible;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkill::ReleasePrePareSkill()
/// \brief		
/// \author		// 2007-10-22 by bhsohn Ÿ���� ��ų ������ ���� ó��
/// \date		2007-10-22 ~ 2007-10-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::ReleasePrePareSkill()
{
	if(NULL == m_pPriSkillInfo)
	{
		return;
	}
	ReleasePrepareTargetOneSkill(m_pPriSkillInfo->ItemNum);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ���� �������� üũ
/// \author		// 2008-08-27 by bhsohn HP/DP���ÿ� ���� ������ �߰�
/// \date		2008-08-27 ~ 2008-08-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsShieldParalyze()
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		CSkillInfo *pSkillInfo = (*itSkillInfo);		
		// 2009-04-21 by bhsohn ������ DesParam�߰�
		//if(IS_EXIST_DES_PARAM(pSkillInfo->ItemInfo,DES_SKILL_SHIELD_PARALYZE))
		if(pSkillInfo->ItemInfo->IsExistDesParam(DES_SKILL_SHIELD_PARALYZE))
		{
			return TRUE;
		}
		itSkillInfo++;
	}	
	return FALSE;

}


// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
// �ش� ���� �Ķ��� �����ϴ� ��ų�� �ɷ� �ִ°�
BOOL	CSkill::IsExistDesParamSKill( BYTE nDesParam )
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		CSkillInfo *pSkillInfo = (*itSkillInfo);		
		if(pSkillInfo->ItemInfo->IsExistDesParam( nDesParam ) )
			return TRUE;
		itSkillInfo++;
	}

	itSkillInfo = m_vecEnemyUsingSkillForMe.begin();
	while(itSkillInfo != m_vecEnemyUsingSkillForMe.end())
	{
		CSkillInfo *pSkillInfo = (*itSkillInfo);		
		if(pSkillInfo->ItemInfo->IsExistDesParam( nDesParam ) )
			return TRUE;
		itSkillInfo++;
	}

	return FALSE;
}
// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�ڱ� �ڽſ��� ���� �ִ� ��ų�̳�?
/// \author		// 2008-10-23 by bhsohn �ڱ� �ڽ����� �� ����Ű �߰�
/// \date		2008-10-23 ~ 2008-10-23
/// \warning	
///
/// \param		BYTE		SkillTargetType;				// ��ų Ÿ�� Ÿ��, SKILLTARGETTYPE_XXX
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsTargetToMyShuttle(BYTE	i_bySkillTargetType)
{
	// �ڽſ��� �� �� ���� ��ų
	if(i_bySkillTargetType == SKILLTARGETTYPE_ONE_EXCLUDE_ME ||
		i_bySkillTargetType == SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME ||
		i_bySkillTargetType == SKILLTARGETTYPE_PARTY_WITHOUT_ME ||			// �� �� ����
		i_bySkillTargetType == SKILLTARGETTYPE_INRANGE_WITHOUT_ME ||
		i_bySkillTargetType == SKILLTARGETTYPE_ONE_EXCEPT_OURS)
	{
		return FALSE;
	}
	return TRUE;		
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�ڱ� �ڽſ��� ���� �ִ� ��ų�̳�?
/// \author		// 2008-10-23 by bhsohn �ڱ� �ڽ����� �� ����Ű �߰�
/// \date		2008-10-23 ~ 2008-10-23
/// \warning	
///
/// \param		BYTE		SkillTargetType;				// ��ų Ÿ�� Ÿ��, SKILLTARGETTYPE_XXX
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::FindTargetForSkill_ToMe()
{
	SetSkillTargetState(FALSE);		// ��ų ���콺 ������ ����

	if(NULL == m_pPriSkillInfo)
	{
		return FALSE;
	}
	if(!IsTargetToMyShuttle(m_pPriSkillInfo->ItemInfo->SkillTargetType))
	{
		// �ڱ� �ڽ� ���� �ȉ?
		char strmsg[256];
		wsprintf(strmsg, STRMSG_C_051205_0001, m_pPriSkillInfo->ItemInfo->ItemName);// "[%s] ��ų�� �ڽſ��� �� �� ���� ��ų�Դϴ�."
		g_pD3dApp->m_pChat->CreateChatChild(strmsg, COLOR_SKILL_CANCEL);
		ReleasePrepareTargetOneSkill(m_pPriSkillInfo->ItemNum);
		return FALSE;		
	}
	else
	{
		SetTargetIndex(g_pShuttleChild->m_myShuttleInfo.ClientIndex);
		return TRUE;	
	}
	return TRUE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkill::IsSkillUse(INT	ItemNum)
/// \brief		
/// \author		// 2009-03-30 by bhsohn ��¡�� ����Ʈ ���� ����
/// \date		2009-03-30 ~ 2009-03-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsSkillUse(INT	ItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if(ItemNum == SKILL_BASE_NUM((*itSkillInfo)->ItemNum))
		{
			return TRUE;
		}
		itSkillInfo++;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-05-07 by bhsohn ��������Ʈ ������ �ý���
/// \date		2013-05-07 ~ 2013-05-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::DisableTimeLimitSkill(INT ItemNum)
{
	vector<CSkillInfo*>::iterator itvecSkillInfo = m_vecEnemyUsingSkillForMe.begin();
	while(itvecSkillInfo != m_vecEnemyUsingSkillForMe.end())
	{
		CSkillInfo* pSkillInfo = (*itvecSkillInfo);
		if(pSkillInfo && ItemNum == pSkillInfo->ItemNum)
		{
			pSkillInfo->DisableTimelimitSkill();
		}
		itvecSkillInfo++;
	}
}