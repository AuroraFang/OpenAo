// SkillInfo.cpp: implementation of the CSkillInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SkillInfo.h"
#include "AtumApplication.h"
#include "AtumDatabase.h"
#include "ShuttleChild.h"
#include "Skill.h"
#include "Chat.h"
#include "WeaponItemInfo.h"
#include "FieldWinSocket.h"
#include "INFGameMain.h"
#include "INFSkill.h"
#include "StoreData.h"
#include "Interface.h"
#include "UnitData.h"	
#include "SceneData.h"
// 2009-03-31 by bhsohn 1��Ī ���� ���� �����, ���� ����
#include "Camera.h"
// end 2009-03-31 by bhsohn 1��Ī ���� ���� �����, ���� ����
#include "SkillEffect.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define WAITING_TIME -2000.0f

CSkillInfo::CSkillInfo(ITEM_SKILL* pItemSkill)
{
	// 2008-12-26 by bhsohn ����� ��� ���� ����
	m_bSkillEnable = TRUE;
	// end 2008-12-26 by bhsohn ����� ��� ���� ����

	Clone(pItemSkill);
	Init();
	m_bSetParamFactor = FALSE;
	m_AttackIndex = 0;
	m_TargetIndex = 0;
}

CSkillInfo::~CSkillInfo()
{

}

void CSkillInfo::Init()
{
	m_fCheckReattackTime = 0;
	m_fCheckEnableTime = 0;
	m_fCheckPrepareTime = 0;
	m_fCheckWaitingTime = 0;
	m_fCheckWaitingPrepareTime = 0;
	m_fCheckAckWaitingTime = 0; // 2005-12-05 by ispark
	m_dwState = SKILL_STATE_READY;
	m_bApprovalEnd = FALSE;
	// 2009-02-17 by bhsohn ��ĵ ���� ����
	SetSkillEnable(TRUE);	// �ٽ� ������·�
	// end 2009-02-17 by bhsohn ��ĵ ���� ����

	// 2010. 10. 11. ������ Ÿ�� ������ ��ų ���� ����.
	ResetClickTargetInfo();

}

void CSkillInfo::Tick(float fElapsedTime)
{
	switch(ItemInfo->SkillType)
	{
	case SKILLTYPE_PERMANENT:
		{
			TickPermanent(fElapsedTime);
		}
		break;
	case SKILLTYPE_CLICK:
		{
			TickClick(fElapsedTime);
		}
		break;
	case SKILLTYPE_TIMELIMIT:
		{
			TickTimelimit(fElapsedTime);
		}
		break;
	case SKILLTYPE_TOGGLE:
		{
			TickToggle(fElapsedTime);
		}
		break;
	case SKILLTYPE_CHARGING:
		{
			TickCharging(fElapsedTime);
		}
		break;
	}
	if(	m_dwState == SKILL_STATE_WAITING &&
		ItemInfo->ReqItemKind != ITEMKIND_ALL_ITEM &&
		g_pStoreData->IsWearItem( ItemInfo->ReqItemKind ) == FALSE &&
		ItemInfo->SkillType != SKILLTYPE_PERMANENT)
	{ // ��ų�� ������ ���Ⱑ �����Ǿ����� �����Ƿ� ��ų�� �����Ѵ�.
		ChangeSkillState(SKILL_STATE_RELEASE);
	}
}

void CSkillInfo::TickPermanent(float fElapsedTime)	// ������
{// �ƹ��� �۾��� ���Ѵ�.
	switch(m_dwState)
	{
	case SKILL_STATE_READY:
		{
			ChangeSkillState(SKILL_STATE_WAITING_PREPARE);
		}
		break;
	case SKILL_STATE_WAITING_PREPARE:
		{
			m_fCheckWaitingPrepareTime -= fElapsedTime*1000.0f;
			if(m_fCheckWaitingPrepareTime < WAITING_TIME)
			{
				ChangeSkillState(SKILL_STATE_RELEASE);
			}		
		}
		break;
	case SKILL_STATE_PREPARE:
		{
			m_fCheckPrepareTime -= fElapsedTime*1000.0f;
			if(m_fCheckPrepareTime <= 0)
			{
				ChangeSkillState(SKILL_STATE_WAITING);
				return;
			}
		}
		break;
	case SKILL_STATE_WAITING:
		{
		}
		break;
	case SKILL_STATE_USING:
		{
		}
		break;
	case SKILL_STATE_WAIT_REATTACK:
		{
		}
		break;
	case SKILL_STATE_RELEASE:
		{
		}
		break;
	}
}

void CSkillInfo::TickClick(float fElapsedTime)		// Ŭ����
{ 
	switch(m_dwState)
	{
	case SKILL_STATE_READY:
		{
			// 2010. 06. 18 by jskim �ݿ��� ����� ������ SP �����ϴ� ���� ����
			//ChangeSkillState(SKILL_STATE_WAITING_PREPARE);
			if(ItemInfo->AttackTime == 0 ||
				(ItemInfo->SkillTargetType	!=	SKILLTARGETTYPE_ONE_EXCLUDE_ME			&&
				ItemInfo->SkillTargetType	!=	SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME	&&
				ItemInfo->SkillTargetType	!=	SKILLTARGETTYPE_PARTY_WITHOUT_ME		&&
				ItemInfo->SkillTargetType	!=	SKILLTARGETTYPE_INRANGE_WITHOUT_ME		&&
				ItemInfo->SkillTargetType	!=	SKILLTARGETTYPE_ONE_EXCEPT_OURS))
			{
				ChangeSkillState(SKILL_STATE_WAITING_PREPARE);
			}
			//end 2010. 06. 18 by jskim �ݿ��� ����� ������ SP �����ϴ� ���� ����
		}
		break;
	case SKILL_STATE_WAITING_PREPARE:
		{
			m_fCheckWaitingPrepareTime -= fElapsedTime*1000.0f;
			if(m_fCheckWaitingPrepareTime < WAITING_TIME)
			{
				ChangeSkillState(SKILL_STATE_RELEASE);
			}		
		}
		break;
	case SKILL_STATE_PREPARE:
		{
			// 2005-12-05 by ispark, Ŭ�������� ���ѵ� �ð� üũ

			if ( m_fCheckPrepareTime > .0f )
				m_fCheckPrepareTime -= fElapsedTime*1000.0f;

			if( m_fCheckAckWaitingTime != 0)
			{
				m_fCheckAckWaitingTime -= fElapsedTime;
//				DBGOUT("2�ð�%f\n", m_fCheckAckWaitingTime);
			}

			if(m_fCheckPrepareTime <= 0 && m_fCheckAckWaitingTime == 0)
			{
				//				DBGOUT("1�ð�%f\n", m_fCheckPrepareTime);
				if( FALSE == this->IsMustOneTargetSkill(ItemInfo->SkillTargetType) ||
					g_pShuttleChild->m_pSkill->GetTargetIndex() != 0 ||
					g_pShuttleChild->m_pSkill->GetCharacterUID() != 0 )
				{
					ChangeSkillState(SKILL_STATE_WAITING);
					g_pShuttleChild->m_pSkill->SetTargetIndex(0);
					g_pShuttleChild->m_pSkill->SetCharacterUID(0);
				}
				// 2010. 10. 11. ������ Ÿ�� ������ ��ų ���� ����.
				else if ( FALSE == this->IsMustOneTargetSkill(ItemInfo->SkillTargetType) ||
						GetClickTargetIndex() != 0 || GetClickTargetUID() != 0 )
				{
					g_pShuttleChild->m_pSkill->SetTargetIndex( this->GetClickTargetIndex() );
					g_pShuttleChild->m_pSkill->SetCharacterUID ( this->GetClickTargetUID() );
					
					ChangeSkillState(SKILL_STATE_WAITING);

					g_pShuttleChild->m_pSkill->SetTargetIndex(0);
					g_pShuttleChild->m_pSkill->SetCharacterUID(0);
					ResetClickTargetInfo();
				}
				// End 2010. 10. 11. ������ Ÿ�� ������ ��ų ���� ����.
			}
			else if(m_fCheckAckWaitingTime < 0)
			{
				// ���ѵ� �ð��� ������ �ϸ� �ڵ� ���
				char strMsg[256];
				wsprintf(strMsg,STRMSG_C_051205_0003,ItemInfo->ItemName);// "�ð��ʰ��� [%s] ��ų�� ��� �Ǿ����ϴ�."
				g_pD3dApp->m_pChat->CreateChatChild(strMsg,COLOR_SKILL_CANCEL);
				ChangeSkillState(SKILL_STATE_RELEASE);

				return;
			}
		}
		break;
	case SKILL_STATE_WAITING:
		{
			m_fCheckWaitingTime -= fElapsedTime*1000.0f;
			if(m_fCheckWaitingTime < WAITING_TIME)
			{
				ChangeSkillState(SKILL_STATE_USING);
			}
		}
		break;
	case SKILL_STATE_USING:
		{
			// Ŭ�����̹Ƿ� m_fCheckEnableTime�� ����.
			//m_fCheckReattackTime -= fElapsedTime*1000.0f;
			if(SKILL_KIND(ItemNum) != SKILL_KIND_CONTROL)
			{
				ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
				return;
			}
		}
		break;
	case SKILL_STATE_WAIT_REATTACK:
		{
			m_fCheckReattackTime -= fElapsedTime*1000.0f;
			if(m_fCheckReattackTime <= 0)
			{
				ChangeSkillState(SKILL_STATE_RELEASE);
				return;
			}
		}
		break;
	case SKILL_STATE_RELEASE:
		{
		}
		break;
	}
}

void CSkillInfo::TickTimelimit(float fElapsedTime)	// �ð���
{
	switch(m_dwState)
	{
	case SKILL_STATE_READY:
		{
			ChangeSkillState(SKILL_STATE_WAITING_PREPARE);
		}
		break;
	case SKILL_STATE_WAITING_PREPARE:
		{
			m_fCheckWaitingPrepareTime -= fElapsedTime*1000.0f;
			if(m_fCheckWaitingPrepareTime < WAITING_TIME)
			{
				ChangeSkillState(SKILL_STATE_RELEASE);
			}		
		}
		break;
	case SKILL_STATE_PREPARE:
		{
			m_fCheckPrepareTime -= fElapsedTime*1000.0f;
			if(m_fCheckPrepareTime <= 0)
			{
				if(FALSE ==	IsMustOneTargetSkill(ItemInfo->SkillTargetType))
				{ // Ÿ���� �ʿ� ���� ��ų�̶��.
					ChangeSkillState(SKILL_STATE_WAITING);
					return;
				}
				
				// ������ �ƴ� Ÿ���ϰ��� Ŭ���Ҷ� ���� ����Ѵ�.
				if(g_pShuttleChild->m_pSkill->GetTargetIndex() != 0)
				{ // ������ ���.
					ChangeSkillState(SKILL_STATE_WAITING);
					return;
				}
			}
		}
		break;
	case SKILL_STATE_WAITING:
		{
			m_fCheckWaitingTime -= fElapsedTime*1000.0f;
			if(m_fCheckWaitingTime < WAITING_TIME)
			{
				ChangeSkillState(SKILL_STATE_USING);
			}
		}
		break;
	case SKILL_STATE_USING:
		{
			m_fCheckEnableTime -= fElapsedTime*1000.0f;
			m_fCheckReattackTime -= fElapsedTime*1000.0f;
			if(m_fCheckEnableTime <= 0)
			{
				ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
				return;
			}
		}
		break;
	case SKILL_STATE_WAIT_REATTACK:
		{
			m_fCheckReattackTime -= fElapsedTime*1000.0f;
			if(m_fCheckReattackTime <= 0)
			{
				ChangeSkillState(SKILL_STATE_RELEASE);
				return;
			}
		}
		break;
	case SKILL_STATE_RELEASE:
		{
			SetInitReattackTime();
		}
		break;
	}
}

void CSkillInfo::TickToggle(float fElapsedTime)		// �����
{
	switch(m_dwState)
	{
	case SKILL_STATE_READY:
		{
			ChangeSkillState(SKILL_STATE_WAITING_PREPARE);
		}
		break;
	case SKILL_STATE_WAITING_PREPARE:
		{
			m_fCheckWaitingPrepareTime -= fElapsedTime*1000.0f;
			if(m_fCheckWaitingPrepareTime < WAITING_TIME)
			{
				ChangeSkillState(SKILL_STATE_RELEASE);
			}		
		}
		break;
	case SKILL_STATE_PREPARE:
		{
			m_fCheckPrepareTime -= fElapsedTime*1000.0f;
			if(m_fCheckPrepareTime <= 0)
			{
				ChangeSkillState(SKILL_STATE_WAITING);
				return;
			}
		}
		break;
	case SKILL_STATE_WAITING:
		{
		}
		break;
	case SKILL_STATE_USING:
		{
		}
		break;
	case SKILL_STATE_WAIT_REATTACK:
		{
			m_fCheckReattackTime -= fElapsedTime*1000.0f;
			if(m_fCheckReattackTime <= 0)
			{
				ChangeSkillState(SKILL_STATE_RELEASE);
				return;
			}
		}
		break;
	case SKILL_STATE_RELEASE:
		{
		}
		break;
	}
}

void CSkillInfo::TickCharging(float fElapsedTime)		// ��¡��
{
	switch(m_dwState)
	{
	case SKILL_STATE_READY:
		{
			ChangeSkillState(SKILL_STATE_WAITING_PREPARE);
		}
		break;
	case SKILL_STATE_WAITING_PREPARE:
		{
			m_fCheckWaitingPrepareTime -= fElapsedTime*1000.0f;
			if(m_fCheckWaitingPrepareTime < WAITING_TIME)
			{
				ChangeSkillState(SKILL_STATE_RELEASE);
			}		
		}
		break;
	case SKILL_STATE_PREPARE:
		{
			m_fCheckPrepareTime -= fElapsedTime*1000.0f;
			if(	m_fCheckPrepareTime <= 0 )
			{
				ChangeSkillState(SKILL_STATE_WAITING);
				return;
			}
		}
		break;
	case SKILL_STATE_WAITING:
		{
		}
		break;
	case SKILL_STATE_USING:
		{
			m_fCheckReattackTime -= fElapsedTime*1000.0f;
		}
		break;
	case SKILL_STATE_WAIT_REATTACK:
		{
			m_fCheckReattackTime -= fElapsedTime*1000.0f;
			if(m_fCheckReattackTime <= 0)
			{
				ChangeSkillState(SKILL_STATE_RELEASE);
				g_pShuttleChild->m_pSkill->SetChargingShotState(SKILL_STATE_RELEASE);
				return;
			}
		}
		break;
	case SKILL_STATE_RELEASE:
		{
		}
		break;
	}
}

void CSkillInfo::Clone(ITEM_SKILL *pItemSkill)
{
	UniqueNumber = pItemSkill->UniqueNumber;
	AccountUniqueNumber = pItemSkill->AccountUniqueNumber;
	ItemInfo = g_pDatabase->GetServerItemInfo( pItemSkill->ItemNum );
	Possess = pItemSkill->Possess;		// ��ų������ ����. 
	ItemNum = pItemSkill->ItemNum;
	ItemWindowIndex = pItemSkill->ItemWindowIndex;
	Kind = pItemSkill->Kind;
}

void CSkillInfo::SetParamFactor()
{
	FLOG( "CSkill::SetParamFactor(ITEM* pItem)" );
	if(m_bSetParamFactor == FALSE)
	{
		// 2009-04-21 by bhsohn ������ DesParam�߰�
// 		g_pShuttleChild->SetParamFactorDesParam( g_pShuttleChild->m_paramFactor , ItemInfo->DestParameter1, ItemInfo->ParameterValue1);	
// 		g_pShuttleChild->SetParamFactorDesParam( g_pShuttleChild->m_paramFactor , ItemInfo->DestParameter2, ItemInfo->ParameterValue2);
// 		g_pShuttleChild->SetParamFactorDesParam( g_pShuttleChild->m_paramFactor , ItemInfo->DestParameter3, ItemInfo->ParameterValue3);
// 		g_pShuttleChild->SetParamFactorDesParam( g_pShuttleChild->m_paramFactor , ItemInfo->DestParameter4, ItemInfo->ParameterValue4);
// 		if( ItemInfo->DestParameter1 == DES_SHOTNUM_01 || 
// 			ItemInfo->DestParameter1 == DES_MULTINUM_01 ||
// 			ItemInfo->DestParameter1 == DES_REATTACKTIME_01 ||
// 			ItemInfo->DestParameter2 == DES_SHOTNUM_01 || 
// 			ItemInfo->DestParameter2 == DES_MULTINUM_01 || 
// 			ItemInfo->DestParameter2 == DES_REATTACKTIME_01 ||
// 			ItemInfo->DestParameter3 == DES_SHOTNUM_01 || 
// 			ItemInfo->DestParameter3 == DES_MULTINUM_01 || 
// 			ItemInfo->DestParameter3 == DES_REATTACKTIME_01 ||
// 			ItemInfo->DestParameter4 == DES_SHOTNUM_01 || 
// 			ItemInfo->DestParameter4 == DES_MULTINUM_01 || 
// 			ItemInfo->DestParameter4 == DES_REATTACKTIME_01)
// 		{
// 			g_pShuttleChild->FieldSocketCharacterSendParamfactorInRange( POS_PROWOUT );
// 		}
// 		else if( ItemInfo->DestParameter1 == DES_SHOTNUM_02 || 
// 			ItemInfo->DestParameter1 == DES_MULTINUM_02 ||
// 			ItemInfo->DestParameter1 == DES_REATTACKTIME_02 ||
// 			ItemInfo->DestParameter2 == DES_SHOTNUM_02 || 
// 			ItemInfo->DestParameter2 == DES_MULTINUM_02 || 
// 			ItemInfo->DestParameter2 == DES_REATTACKTIME_02 ||
// 			ItemInfo->DestParameter3 == DES_SHOTNUM_02 || 
// 			ItemInfo->DestParameter3 == DES_MULTINUM_02 || 
// 			ItemInfo->DestParameter3 == DES_REATTACKTIME_02 ||
// 			ItemInfo->DestParameter4 == DES_SHOTNUM_02 || 
// 			ItemInfo->DestParameter4 == DES_MULTINUM_02 || 
// 			ItemInfo->DestParameter4 == DES_REATTACKTIME_02)
// 		{
// 			g_pShuttleChild->FieldSocketCharacterSendParamfactorInRange( POS_WINGOUT );
// 		}
		
		// 2013-05-07 by bhsohn M��� ������ ������ �۾�
// 		int nArrParamCnt = 0;
// 		for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
// 		{
// 			g_pShuttleChild->SetParamFactorDesParam( g_pShuttleChild->m_paramFactor, 
// 				ItemInfo->ArrDestParameter[nArrParamCnt], 
// 				ItemInfo->ArrParameterValue[nArrParamCnt]);	
// 			
// 		}		
#ifdef SC_BUFF_PENALTY_JHSEOL_BCKIM
		float fPanelty = 1.0f;
		if(g_pGameMain && g_pGameMain->GetINFItemInfo())
		{			
			fPanelty = g_pGameMain->GetINFItemInfo()->GetBuffPanaltyInfo(SIZE_MAX_DESPARAM_COUNT_IN_ITEM, 
														ItemInfo->ArrDestParameter, ItemInfo->ArrParameterValue, 
														g_pShuttleChild->m_myShuttleInfo.UnitKind);
		}
		int nArrParamCnt = 0;
		for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			g_pShuttleChild->SetParamFactorDesParam( g_pShuttleChild->m_paramFactor, 
				ItemInfo->ArrDestParameter[nArrParamCnt], 
				fPanelty*ItemInfo->ArrParameterValue[nArrParamCnt]);	
			
		}
#elif defined NEMERIAN_NATION_BALANCE_BUFF_PENALITY
		int nArrParamCnt = 0;
		for (nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			if (ItemInfo->ArrDestParameter[nArrParamCnt] == DES_DEFENSE_01
				|| ItemInfo->ArrDestParameter[nArrParamCnt] == DES_DEFENSE_02
				|| ItemInfo->ArrDestParameter[nArrParamCnt] == DES_DEFENSEPROBABILITY_01
				|| ItemInfo->ArrDestParameter[nArrParamCnt] == DES_DEFENSEPROBABILITY_02
				|| ItemInfo->ArrDestParameter[nArrParamCnt] == DES_MINATTACK_01
				|| ItemInfo->ArrDestParameter[nArrParamCnt] == DES_MAXATTACK_01
				|| ItemInfo->ArrDestParameter[nArrParamCnt] == DES_MINATTACK_02
				|| ItemInfo->ArrDestParameter[nArrParamCnt] == DES_MAXATTACK_02
				|| ItemInfo->ArrDestParameter[nArrParamCnt] == DES_ATTACKPROBABILITY_01
				|| ItemInfo->ArrDestParameter[nArrParamCnt] == DES_ATTACKPROBABILITY_02) {
				g_pShuttleChild->SetParamFactorDesParam(g_pShuttleChild->m_paramFactor,
					ItemInfo->ArrDestParameter[nArrParamCnt],
					(g_buffPenality*ItemInfo->ArrParameterValue[nArrParamCnt]));
			}
			else {
				g_pShuttleChild->SetParamFactorDesParam(g_pShuttleChild->m_paramFactor,
					ItemInfo->ArrDestParameter[nArrParamCnt],
					ItemInfo->ArrParameterValue[nArrParamCnt]);
			}

		}
#else
		int nArrParamCnt = 0;
		for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			g_pShuttleChild->SetParamFactorDesParam( g_pShuttleChild->m_paramFactor, 
				ItemInfo->ArrDestParameter[nArrParamCnt], 
				ItemInfo->ArrParameterValue[nArrParamCnt]);	
			
		}		
#endif
		// END 2013-05-07 by bhsohn M��� ������ ������ �۾�

		
		BOOL bBreak = FALSE;
		for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			if( ItemInfo->ArrDestParameter[nArrParamCnt] == DES_SHOTNUM_01 
				||ItemInfo->ArrDestParameter[nArrParamCnt] == DES_MULTINUM_01 
				||ItemInfo->ArrDestParameter[nArrParamCnt] == DES_REATTACKTIME_01)
			{
				bBreak = TRUE;
				g_pShuttleChild->FieldSocketCharacterSendParamfactorInRange( POS_PROWOUT );
				break;
			}

		}
		for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			if(bBreak)
			{
				break;
			}
			if( ItemInfo->ArrDestParameter[nArrParamCnt] == DES_SHOTNUM_02 || 
				ItemInfo->ArrDestParameter[nArrParamCnt] == DES_MULTINUM_02 ||
				ItemInfo->ArrDestParameter[nArrParamCnt] == DES_REATTACKTIME_02 )
			{
				g_pShuttleChild->FieldSocketCharacterSendParamfactorInRange( POS_WINGOUT );
				break;
			}
		}
		// end 2009-04-21 by bhsohn ������ DesParam�߰�

		m_bSetParamFactor = TRUE;
	}
}

void CSkillInfo::ReleaseParamFactor()
{
	FLOG( "CSkill::ReleaseParamFactor(ITEM* pItem)" );
	if(m_bSetParamFactor == TRUE)
	{
		// 2009-04-21 by bhsohn ������ DesParam�߰�
// 		g_pShuttleChild->ReleaseParamFactorDesParam( g_pShuttleChild->m_paramFactor , ItemInfo->DestParameter1, ItemInfo->ParameterValue1);	
// 		g_pShuttleChild->ReleaseParamFactorDesParam( g_pShuttleChild->m_paramFactor , ItemInfo->DestParameter2, ItemInfo->ParameterValue2);
// 		g_pShuttleChild->ReleaseParamFactorDesParam( g_pShuttleChild->m_paramFactor , ItemInfo->DestParameter3, ItemInfo->ParameterValue3);
// 		g_pShuttleChild->ReleaseParamFactorDesParam( g_pShuttleChild->m_paramFactor , ItemInfo->DestParameter4, ItemInfo->ParameterValue4);
// 		if( ItemInfo->DestParameter1 == DES_SHOTNUM_01 || 
// 			ItemInfo->DestParameter1 == DES_MULTINUM_01 ||
// 			ItemInfo->DestParameter1 == DES_REATTACKTIME_01 ||
// 			ItemInfo->DestParameter2 == DES_SHOTNUM_01 || 
// 			ItemInfo->DestParameter2 == DES_MULTINUM_01 || 
// 			ItemInfo->DestParameter2 == DES_REATTACKTIME_01 ||
// 			ItemInfo->DestParameter3 == DES_SHOTNUM_01 || 
// 			ItemInfo->DestParameter3 == DES_MULTINUM_01 || 
// 			ItemInfo->DestParameter3 == DES_REATTACKTIME_01 ||
// 			ItemInfo->DestParameter4 == DES_SHOTNUM_01 || 
// 			ItemInfo->DestParameter4 == DES_MULTINUM_01 || 
// 			ItemInfo->DestParameter4 == DES_REATTACKTIME_01)
// 		{
// 			g_pShuttleChild->FieldSocketCharacterSendParamfactorInRange( POS_PROWOUT );
// 		}
// 		else if( ItemInfo->DestParameter1 == DES_SHOTNUM_02 || 
// 			ItemInfo->DestParameter1 == DES_MULTINUM_02 ||
// 			ItemInfo->DestParameter1 == DES_REATTACKTIME_02 ||
// 			ItemInfo->DestParameter2 == DES_SHOTNUM_02 || 
// 			ItemInfo->DestParameter2 == DES_MULTINUM_02 || 
// 			ItemInfo->DestParameter2 == DES_REATTACKTIME_02 ||
// 			ItemInfo->DestParameter3 == DES_SHOTNUM_02 || 
// 			ItemInfo->DestParameter3 == DES_MULTINUM_02 || 
// 			ItemInfo->DestParameter3 == DES_REATTACKTIME_02 ||
// 			ItemInfo->DestParameter4 == DES_SHOTNUM_02 || 
// 			ItemInfo->DestParameter4 == DES_MULTINUM_02 || 
// 			ItemInfo->DestParameter4 == DES_REATTACKTIME_02)
// 		{
// 			g_pShuttleChild->FieldSocketCharacterSendParamfactorInRange( POS_WINGOUT );
// 		}
		// 2013-05-07 by bhsohn M��� ������ ������ �۾�
// 		int nArrParamCnt = 0;
// 		for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
// 		{
// 			g_pShuttleChild->ReleaseParamFactorDesParam( g_pShuttleChild->m_paramFactor, 
// 				ItemInfo->ArrDestParameter[nArrParamCnt], 
// 				ItemInfo->ArrParameterValue[nArrParamCnt]);	
// 		}				
#ifdef SC_BUFF_PENALTY_JHSEOL_BCKIM
		float fPanelty = 1.0f;
		if(g_pGameMain && g_pGameMain->GetINFItemInfo())
		{			
			fPanelty = g_pGameMain->GetINFItemInfo()->GetBuffPanaltyInfo(SIZE_MAX_DESPARAM_COUNT_IN_ITEM, 
				ItemInfo->ArrDestParameter, ItemInfo->ArrParameterValue, 
				g_pShuttleChild->m_myShuttleInfo.UnitKind);
		}
		int nArrParamCnt = 0;
		for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			g_pShuttleChild->ReleaseParamFactorDesParam( g_pShuttleChild->m_paramFactor, 
				ItemInfo->ArrDestParameter[nArrParamCnt], 
				fPanelty * ItemInfo->ArrParameterValue[nArrParamCnt]);	
		}
#elif defined NEMERIAN_NATION_BALANCE_BUFF_PENALITY
		int nArrParamCnt = 0;
		for (nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			if (ItemInfo->ArrDestParameter[nArrParamCnt] == DES_DEFENSE_01
				|| ItemInfo->ArrDestParameter[nArrParamCnt] == DES_DEFENSE_02
				|| ItemInfo->ArrDestParameter[nArrParamCnt] == DES_DEFENSEPROBABILITY_01
				|| ItemInfo->ArrDestParameter[nArrParamCnt] == DES_DEFENSEPROBABILITY_02
				|| ItemInfo->ArrDestParameter[nArrParamCnt] == DES_MINATTACK_01
				|| ItemInfo->ArrDestParameter[nArrParamCnt] == DES_MAXATTACK_01
				|| ItemInfo->ArrDestParameter[nArrParamCnt] == DES_MINATTACK_02
				|| ItemInfo->ArrDestParameter[nArrParamCnt] == DES_MAXATTACK_02
				|| ItemInfo->ArrDestParameter[nArrParamCnt] == DES_ATTACKPROBABILITY_01
				|| ItemInfo->ArrDestParameter[nArrParamCnt] == DES_ATTACKPROBABILITY_02) {
				g_pShuttleChild->ReleaseParamFactorDesParam(g_pShuttleChild->m_paramFactor,
					ItemInfo->ArrDestParameter[nArrParamCnt],
					(g_buffPenality*ItemInfo->ArrParameterValue[nArrParamCnt]));
			}
			else {
				g_pShuttleChild->ReleaseParamFactorDesParam(g_pShuttleChild->m_paramFactor,
					ItemInfo->ArrDestParameter[nArrParamCnt],
					ItemInfo->ArrParameterValue[nArrParamCnt]);
			}
		}
#else
		int nArrParamCnt = 0;
		for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			g_pShuttleChild->ReleaseParamFactorDesParam( g_pShuttleChild->m_paramFactor, 
			ItemInfo->ArrDestParameter[nArrParamCnt], 
			ItemInfo->ArrParameterValue[nArrParamCnt]);	
		}				
#endif
		// END 2013-05-07 by bhsohn M��� ������ ������ �۾�

		
		BOOL bBreak = FALSE;
		for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			if( ItemInfo->ArrDestParameter[nArrParamCnt] == DES_SHOTNUM_01 || 
				ItemInfo->ArrDestParameter[nArrParamCnt] == DES_MULTINUM_01 ||
				ItemInfo->ArrDestParameter[nArrParamCnt] == DES_REATTACKTIME_01)				
			{
				bBreak = TRUE;
				g_pShuttleChild->FieldSocketCharacterSendParamfactorInRange( POS_PROWOUT );
				break;
			}
		}
		for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			if(bBreak)
			{
				break;
			}
			if( ItemInfo->ArrDestParameter[nArrParamCnt] == DES_SHOTNUM_02 || 
				ItemInfo->ArrDestParameter[nArrParamCnt] == DES_MULTINUM_02 ||
				ItemInfo->ArrDestParameter[nArrParamCnt] == DES_REATTACKTIME_02)
			{
				g_pShuttleChild->FieldSocketCharacterSendParamfactorInRange( POS_WINGOUT );
				break;
			}
		}
		// end 2009-04-21 by bhsohn ������ DesParam�߰�

		m_bSetParamFactor = FALSE;
	}
}

void CSkillInfo::ChangeSkillState(int dwState, int nTempSkillItemNum) 
{ 
	switch(dwState)
	{
	case SKILL_STATE_WAITING_PREPARE:
		{
// 2006-12-20 by ispark, OK�� ������ ó���Ѵ�.
//*--------------------------------------------------------------------------*//
//			m_dwState = SKILL_STATE_PREPARE;
//			m_fCheckPrepareTime = ItemInfo->AttackTime;
//*--------------------------------------------------------------------------*//
			m_dwState = SKILL_STATE_WAITING_PREPARE;			
// 2006-11-29 by dgwoo ĳ����Ÿ���� �ִ°͵��� SendPrepareUseSkill()���.
//			if(ItemInfo->SkillType == SKILLTYPE_CHARGING)		
			if(ItemInfo->AttackTime > 0)
			{
				if( ItemInfo->ReqSP > g_pShuttleChild->m_myShuttleInfo.CurrentSP )
				{	
					g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0007,COLOR_SKILL_CANCEL);
					ChangeSkillState(SKILL_STATE_RELEASE);
				}
				else
				{
					g_pShuttleChild->m_pSkill->SendPrepareUseSkill(this);
				}
			}
			else
			{
				// ĳ���� Ÿ���� ���� ��ų�� Prepare�� �ٷ� �Ѿ��.
				ChangeSkillState(SKILL_STATE_PREPARE);
			}
			// 2008-12-26 by bhsohn ����� ��� ���� ����
			SetSkillEnable(TRUE);	// �ٽ� ������·�
			// end 2008-12-26 by bhsohn ����� ��� ���� ����
		}
		break;
	case SKILL_STATE_PREPARE:
		{
			m_dwState = SKILL_STATE_PREPARE;
			m_fCheckPrepareTime = ItemInfo->AttackTime;
		}
		break;
	case SKILL_STATE_WAITING:
		{
			m_dwState = SKILL_STATE_WAITING;
			if(ItemInfo->SkillType != SKILLTYPE_PERMANENT)
			{
				if( ItemInfo->AttackTime <= 0 &&
					ItemInfo->ReqSP > g_pShuttleChild->m_myShuttleInfo.CurrentSP )
				{	
					g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0007,COLOR_SKILL_CANCEL);
					ChangeSkillState(SKILL_STATE_RELEASE);
				}
				else
				{
					// 2010. 01. 27 by jskim ���� �� ������� ���� �����Ͽ��� ���� ���·� ���� �Ǵ� ���� ����
					if(SKILL_BASE_NUM(ItemNum) == AGEAR_SKILL_BASENUM_AIRSIEGEMODE 
						&& g_pShuttleChild->m_dwState == _LANDING)
					{
						g_pShuttleChild->InitUnitStop();						
						g_pShuttleChild->m_dwState = _NORMAL;
					}
					//end 2010. 01. 27 by jskim ���� �� ������� ���� �����Ͽ��� ���� ���·� ���� �Ǵ� ���� ����
					// ������ ��ų ����� ����
					SendUseSkill();
				}
			}
			else
			{
				// permanent type�� �ٷ� �����Ѵ�.
				UseSkillFromServer();
			}
		}
		break;
	case SKILL_STATE_USING:
		{
			m_dwState = SKILL_STATE_USING;
			m_fCheckEnableTime = ItemInfo->Time;
			m_fCheckReattackTime = ItemInfo->ReAttacktime;
		}
		break;
	case SKILL_STATE_WAIT_REATTACK:
		{
			m_dwState = SKILL_STATE_WAIT_REATTACK;
			// 2008-12-26 by bhsohn ����� ��� ���� ����			
			if(!IsSkillEnable())
			{
				// 2009-02-17 by bhsohn ��ĵ ���� ����
				SetSkillEnable(TRUE);	// �ٽ� ������·�
				// end 2009-02-17 by bhsohn ��ĵ ���� ����
				// ������ų�̴�.
				return;				
			}
			// end 2008-12-26 by bhsohn ����� ��� ���� ����
			SendCancelSkill(nTempSkillItemNum);
			char strMessage[256];
			char* strSkillName;
			strSkillName = g_pShuttleChild->m_pSkill->GetSkillName(ItemInfo->ItemNum);
			if(	ItemInfo->SkillType == SKILLTYPE_TIMELIMIT ||
				ItemInfo->SkillType == SKILLTYPE_TOGGLE)
			{
				g_pGameMain->m_pInfSkill->DeleteSkillInfo(this,Possess);
				wsprintf(strMessage,STRMSG_C_SKILL_0003,strSkillName);//"��ų [%s] �� �����մϴ�."
				g_pD3dApp->m_pChat->CreateChatChild(strMessage,COLOR_SKILL_CANCEL,CHAT_TAB_SYSTEM);
				// 2009-03-31 by bhsohn 1��Ī ���� ���� �����, ���� ����
				if(SKILL_BASE_NUM(ItemInfo->ItemNum) == AGEAR_SKILL_BASENUM_AIRSIEGEMODE)
				{
					// �������� 3��Ī���� ��� ����
					if(CAMERA_TYPE_FPS == g_pSOption->sEyePt)
					{
						g_pD3dApp->m_pCamera->SetCamType( g_pSOption->sEyePt);
					}					
				}
				// end 2009-03-31 by bhsohn 1��Ī ���� ���� �����, ���� ����

			}
			ReleaseParamFactor();

			switch(SKILL_BASE_NUM(ItemNum))
			{
			
			case AGEAR_SKILL_BASENUM_AIRSIEGEMODE:
				g_pShuttleChild->ChangeUnitState(_NORMAL);
				g_pD3dApp->m_bFixUnitDirMode = FALSE;
				
			case AGEAR_SKILL_BASENUM_SIEGEMODE:
			case AGEAR_SKILL_BASENUM_SIEGEDEFENSEMODE:
				g_pShuttleChild->ChangeSingleBodyCondition( BODYCON_SIEGE_OFF_MASK );
				if(COMPARE_BODYCON_BIT( g_pShuttleChild->GetCurrentBodyCondition(), BODYCON_SIEGE_ON_MASK) )
				{
					g_pShuttleChild->SendFieldSocketChangeBodyCondition( g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_SIEGE_ON_MASK );
				}
				g_pShuttleChild->SendFieldSocketChangeBodyCondition( g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_SIEGE_OFF_MASK | BODYCON_SET_OR_CLEAR_MASK );
				g_pShuttleChild->m_bAttackMode = _AIR;
				
				if(g_pShuttleChild->m_pPrimaryWeapon)
				{
					g_pShuttleChild->m_pPrimaryWeapon->SetAttackMode( ATT_TYPE_GENERAL_PRI );//(BYTE)fValue;
				}
				if(g_pShuttleChild->m_pSecondaryWeapon)
				{
					g_pShuttleChild->m_pSecondaryWeapon->SetAttackMode( ATT_TYPE_GENERAL_SEC );//(BYTE)fValue;
				}
				break;
			case BGEAR_SKILL_BASENUM_GROUNDBOMBINGMODE:
				g_pShuttleChild->m_bAttackMode = _AIR;
				if(g_pShuttleChild->m_pSecondaryWeapon)
				{
					g_pShuttleChild->m_pSecondaryWeapon->SetAttackMode( ATT_TYPE_GENERAL_SEC );//(BYTE)fValue;
				}
				break;
			case BGEAR_SKILL_BASENUM_AIRBOMBINGMODE:
				g_pShuttleChild->m_bAttackMode = _AIR;
				if(g_pShuttleChild->m_pSecondaryWeapon)
				{
					g_pShuttleChild->m_pSecondaryWeapon->SetAttackMode( ATT_TYPE_GENERAL_SEC );//(BYTE)fValue;
				}
				break;
			// 2006-11-30 by dgwoo ��� ������ �������� Ǯ���ش�.
			case BGEAR_SKILL_BASENUM_BIG_BOOM:
				g_pShuttleChild->m_bUnitStop			= FALSE;
				g_pShuttleChild->m_bPenaltyUnitStop		= TRUE;
				g_pShuttleChild->m_bMouseMoveLock		= FALSE;
				g_pShuttleChild->m_bSkillGoMove			= TRUE;
				g_pShuttleChild->m_nBooster = BOOSTER_TYPE_MIN;
				break;
//			case SMART_SPEED:	// 2005-11-26 by ispark
			case MGEAR_SKILL_BASENUM_REVERSEENGINE:
				g_pShuttleChild->m_pSkill->SetSmartSpeedState(FALSE);
				g_pShuttleChild->SetShuttleSpeed();
				break;
			case MGEAR_SKILL_BASENUM_INVICIBLE:
				g_pShuttleChild->m_bAttack = TRUE;
				break;
			case AGEAR_SKILL_BASENUM_GROUNDACCELERATOR:
				g_pShuttleChild->SetShuttleSpeed();
				break;
			case AGEAR_SKILL_BASENUM_CAMOUFLAGE:
				g_pShuttleChild->SetUnitAlpha(CL_SKILL_NONE, SKILL_OBJECT_ALPHA_NONE);
				break;
			case BGEAR_SKILL_BASENUM_INVISIBLE:
				g_pShuttleChild->SetUnitAlpha(CL_SKILL_NONE, SKILL_OBJECT_ALPHA_NONE);
				g_pShuttleChild->m_bAttack = TRUE;
				break;
			case IGEAR_SKILL_BASENUM_SILENCE:
				g_pShuttleChild->SetSkillMissileWarning(FALSE);
				if(g_pShuttleChild->m_pSkill)
				{
					//2012-02-23 by jhahn ħ���� ��� ����Ʈ ���� ����
					g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
					//end 2012-02-23 by jhahn ħ���� ��� ����Ʈ ���� ����
					//g_pShuttleChild->m_pSkill->ReleaseAllUsingSkillWithOutPermanent();
					// 2007-04-05 by bhsohn ��� ���� ó��
					// ���� ��ų�ʱ�ȭ
				//	g_pShuttleChild->m_pSkill->InitPreSkillItemNum();
				}
				break;
			case MGEAR_SKILL_BASENUM_SCANNING:
				// 2007-04-19 by bhsohn ��ġ���� ������ �߰�
				//g_pShuttleChild->m_pSkill->SetScanSkill(FALSE);
				g_pShuttleChild->m_pSkill->SetScanSkill(SCAN_SKILL, FALSE);
				break;
			default:
				break;
			}
			
			if( ItemInfo->SkillType == SKILLTYPE_CLICK &&
				SKILL_KIND(ItemNum) == SKILL_KIND_CONTROL )	// ������ �ִ� ��ų�̶�� ������ ����
			{
				g_pShuttleChild->DeletePattern();				
				g_pShuttleChild->m_pSkill->SetSkillPatternType(0);	// ���� �ʱ�ȭ
//				g_pShuttleChild->m_pSkill->DisableSkillEffect(g_pShuttleChild,SKILL_BASE_NUM(ItemNum));
				// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
//				g_pShuttleChild->m_pSkill->DisableSkillEffect(g_pShuttleChild,SKILL_BASE_NUM(ItemInfo->SourceIndex));
				g_pShuttleChild->m_pSkill->DisableSkillEffect(g_pShuttleChild,SKILL_BASE_SOURCEINDEX(ItemInfo->SourceIndex));
				// END 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
			}

			// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
			if( ItemInfo->SkillType == SKILLTYPE_TIMELIMIT )
			{
				// Ȧ�彺ų ������
				if( ItemInfo->IsExistDesParam( DES_SKILL_MON_HOLD ) )
				{
					int nCnt = 0;
					
					// Ȧ�� ���� �Ķ��� �ִ� ��ų�� 1������ ������ Ȧ�� ���¸� �����Ѵ�
					// �ֳĸ� ���� ��ų 1�� �ٷ� ���� �Ǳ� ����
					for( std::vector<CSkillInfo*>::iterator it = g_pShuttleChild->m_pSkill->m_vecEnemyUsingSkillForMe.begin();
						 it != g_pShuttleChild->m_pSkill->m_vecEnemyUsingSkillForMe.end();
						 ++it )
					{
						if( (*it)->ItemInfo->IsExistDesParam( DES_SKILL_MON_HOLD ) )
							++nCnt;
					}

					if( nCnt <= 1 )
					{
						g_pShuttleChild->m_pSkill->SetMonsterHoldState( FALSE );
						g_pShuttleChild->SetShuttleSpeed();
					}
				}
			}
			// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
		}
		break;
	case SKILL_STATE_RELEASE:
		{
			m_dwState = SKILL_STATE_RELEASE;
			// 2008-12-26 by bhsohn ����� ��� ���� ����
			SetSkillEnable(TRUE);	// �ٽ� ������·�
			// end 2008-12-26 by bhsohn ����� ��� ���� ����
		}
		break;
	}

	// ��¡�� ���� ����
	if(ItemInfo->SkillType == SKILLTYPE_CHARGING)
	{
		g_pShuttleChild->m_pSkill->SetChargingShotState(m_dwState);	
	}
}
// 2010. 06. 18 by jskim ���� ��� ��ų ��Ʈ�� ���� 
//void CSkillInfo::UseSkillFromServer()
void CSkillInfo::UseSkillFromServer(char* Name /* = NULL */, BOOL isUser /* = TRUE */)
//end 2010. 06. 18 by jskim ���� ��� ��ų ��Ʈ�� ����
{
	if( (ItemInfo->SkillTargetType != SKILLTARGETTYPE_ONE ||
		ItemInfo->SkillTargetType != SKILLTARGETTYPE_ONE_EXCEPT_OURS) &&
		(m_dwState == SKILL_STATE_WAIT_REATTACK ||
		m_dwState == SKILL_STATE_RELEASE) )
	{
		return;
	}
	Init();
	ChangeSkillState(SKILL_STATE_USING);
	SetParamFactor();
	if(ItemInfo->SkillType != SKILLTYPE_PERMANENT)
	{
		char strMessage[256];
		g_pGameMain->m_pInfSkill->InsertSkillInfo(this);
		// 2010. 06. 18 by jskim ���� ��� ��ų ��Ʈ�� ����
		//wsprintf(strMessage,STRMSG_C_SKILL_0004,ItemInfo->ItemName);	//"��ų [%s] �� ����մϴ�."
		if(isUser)
		{
			wsprintf(strMessage,STRMSG_C_SKILL_0004,ItemInfo->ItemName);	//"��ų [%s] �� ����մϴ�."
		}
		else
		{
			if(Name == NULL)
			{
				wsprintf(strMessage,STRMSG_C_100618_0401, ItemInfo->ItemName);	//"���Ͱ� ��ų [%s] �� ����մϴ�."
			}
			else
			{
				wsprintf(strMessage,STRMSG_C_100618_0402, Name, ItemInfo->ItemName);	//"%s(��)�� ��ų [%s] �� ����մϴ�."
			}
			
		}		
		//end 2010. 06. 18 by jskim ���� ��� ��ų ��Ʈ�� ����
		g_pD3dApp->m_pChat->CreateChatChild(strMessage,COLOR_SKILL_USE,CHAT_TAB_SYSTEM);
	}

	switch(SKILL_BASE_NUM(ItemNum))
	{
		case AGEAR_SKILL_BASENUM_AIRSIEGEMODE:
			g_pD3dApp->m_bFixUnitDirMode = TRUE;
		case AGEAR_SKILL_BASENUM_SIEGEMODE:
		case AGEAR_SKILL_BASENUM_SIEGEDEFENSEMODE:
			g_pShuttleChild->ChangeSingleBodyCondition( BODYCON_SIEGE_ON_MASK );
			if(COMPARE_BODYCON_BIT( g_pShuttleChild->GetCurrentBodyCondition(), BODYCON_SIEGE_OFF_MASK) )
			{
				g_pShuttleChild->SendFieldSocketChangeBodyCondition( g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_SIEGE_OFF_MASK );
			}
			g_pShuttleChild->SendFieldSocketChangeBodyCondition( g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_SIEGE_ON_MASK | BODYCON_SET_OR_CLEAR_MASK );
			g_pShuttleChild->m_bAttackMode = _SIEGE;
			if(g_pShuttleChild->m_pPrimaryWeapon)
			{
				g_pShuttleChild->m_pPrimaryWeapon->SetAttackMode( ATT_TYPE_SEIGE_PRI );//(BYTE)fValue;
			}
			if(g_pShuttleChild->m_pSecondaryWeapon)
			{
				g_pShuttleChild->m_pSecondaryWeapon->SetAttackMode( ATT_TYPE_SIEGE_SEC );//(BYTE)fValue;
			}
			break;
		case AGEAR_SKILL_BASENUM_GROUNDACCELERATOR:
			g_pShuttleChild->SetShuttleSpeed();
			break;
		case AGEAR_SKILL_BASENUM_CAMOUFLAGE:
			g_pShuttleChild->SetUnitAlpha(CL_SKILL_CAMOUFLAGE, SKILL_OBJECT_ALPHA_DEFAULT);
			break;
		case BGEAR_SKILL_BASENUM_GROUNDBOMBINGMODE:
			g_pShuttleChild->m_bAttackMode = _GROUND;
			if(g_pShuttleChild->m_pSecondaryWeapon)
			{
				g_pShuttleChild->m_pSecondaryWeapon->SetAttackMode( ATT_TYPE_GROUND_BOMBING_SEC );//(BYTE)fValue;
				g_pShuttleChild->m_pSkill->SetSecondarySkill(this);
			}
			break;
		case BGEAR_SKILL_BASENUM_AIRBOMBINGMODE:
			// 2007-06-25 by dgwoo ������ ������ �������� ����Ǵ� ���׼���.
			//g_pShuttleChild->m_bAttackMode = _GROUND;
			g_pShuttleChild->m_bAttackMode = _AIRBOMBING;
			if(g_pShuttleChild->m_pSecondaryWeapon)
			{
				g_pShuttleChild->m_pSecondaryWeapon->SetAttackMode( ATT_TYPE_AIR_BOMBING_SEC );//(BYTE)fValue;
				g_pShuttleChild->m_pSkill->SetSecondarySkill(this);
			}
			break;
		case BGEAR_SKILL_BASENUM_INVISIBLE:
			g_pShuttleChild->SetUnitAlpha(CL_SKILL_INVISIBLE, SKILL_OBJECT_ALPHA_DEFAULT);
			g_pShuttleChild->SetUnLockOn();
			g_pShuttleChild->m_pTarget = NULL;
			if(g_pShuttleChild->m_pPrimaryWeapon)
			{
				// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
				//g_pShuttleChild->m_pPrimaryWeapon->m_vecTargetIndex.clear();
				g_pShuttleChild->m_pPrimaryWeapon->m_vecTargetIndexData.clear();
				// end 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
			}
			if(g_pShuttleChild->m_pSecondaryWeapon)
			{
				// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
				//g_pShuttleChild->m_pSecondaryWeapon->m_vecTargetIndex.clear();
				g_pShuttleChild->m_pSecondaryWeapon->m_vecTargetIndexData.clear();
				// end 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
			}
			g_pShuttleChild->m_bAttack = FALSE;
			break;
		// 2006-11-30 by dgwoo
		case BGEAR_SKILL_BASENUM_BIG_BOOM:
			break;
	//	case SMART_SPEED:	// 2005-11-26 by ispark
		case MGEAR_SKILL_BASENUM_REVERSEENGINE:
			g_pShuttleChild->m_pSkill->SetSmartSpeedState(TRUE);
			g_pShuttleChild->SetShuttleSpeed();
			break;
		case MGEAR_SKILL_BASENUM_INVICIBLE:
			g_pShuttleChild->m_bAttack = FALSE;
			break;
		case IGEAR_SKILL_BASENUM_SILENCE:
			g_pShuttleChild->SetSkillMissileWarning(TRUE);
			if(g_pShuttleChild->m_pSkill)
			{
				// 2012-02-23 by jhahn ħ���� ��� ����Ʈ ���� ����
				g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
				//end 2012-02-23 by jhahn ħ���� ��� ����Ʈ ���� ����
				//	g_pShuttleChild->m_pSkill->ReleaseAllUsingSkillWithOutPermanent();
				// 2007-04-05 by bhsohn ��� ���� ó��
				// ���� ��ų�ʱ�ȭ
			//	g_pShuttleChild->m_pSkill->InitPreSkillItemNum();
			}
			break;
		case IGEAR_SKILL_BASENUM_BERSERKER:
			// 2007-02-01 by dgwoo ����Ŀ ��ƿ�� ���� ��������ų�� ��ҽ�Ű�� �ʴ´�.
			//g_pShuttleChild->m_pSkill->ReleseSkill(IGEAR_SKILL_BASENUM_BERSERKER);
			break;
		case MGEAR_SKILL_BASENUM_SCANNING:
			{
				// 2007-04-19 by bhsohn ��ġ���� ������ �߰�
				//g_pShuttleChild->m_pSkill->SetScanSkill(TRUE);
				//g_pShuttleChild->m_pSkill->SetScanRange(ItemInfo->Range);
				g_pShuttleChild->m_pSkill->SetScanSkill(SCAN_SKILL, TRUE);
				g_pShuttleChild->m_pSkill->SetScanRange(SCAN_SKILL, ItemInfo->Range);
				
				CUnitData *pUnit = g_pScene->FindUnitDataByClientIndex(m_AttackIndex);
				if(pUnit != NULL)
				{
					if(m_AttackIndex == m_TargetIndex)
					{// �ڽ��� �� ��ĵ�� ���� ���� �����ϰ� �ƴ϶�� Enemy->UseSkillFromServer���� �����Ѵ�.
						// 2007-04-19 by bhsohn ��ġ���� ������ �߰�
						D3DXVECTOR3	vVel, vUp;
						vVel = D3DXVECTOR3(0,0,1);
						vUp = D3DXVECTOR3(0,1,0);
						g_pScene->AddFieldItemScanObject(pUnit,m_fCheckEnableTime, RC_EFF_SCAN_SKILL, _LAY, vVel, vUp);
					}
					// 2007-04-19 by bhsohn ��ġ���� ������ �߰�
					//g_pShuttleChild->m_pSkill->m_vScanPosition = pUnit->m_vPos;
					g_pShuttleChild->m_pSkill->SetScanPosition(SCAN_SKILL, pUnit->m_vPos);
				}
			}
			break;
		default:
			break;
	}
	
	if(ItemInfo->SkillType == SKILLTYPE_CLICK)
	{//	Ŭ����
		if(SKILL_KIND(ItemNum) == SKILL_KIND_CONTROL)	// ������ �ִ� ��ų�̶�� ������ ����
		{
			// 2005-08-23 by ispark
			// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
//			if(g_pShuttleChild->InitCinemaSkill(SKILL_BASE_NUM(ItemInfo->SourceIndex)) == TRUE)
			if(g_pShuttleChild->InitCinemaSkill(SKILL_BASE_SOURCEINDEX(ItemInfo->SourceIndex)) == TRUE)
			{
				g_pShuttleChild->InitCinemaCamera(ItemInfo->CameraPattern);
			}
// 2007-01-18 by dgwoo �ҽ� �ε����� �ƴ� ItemNum����
//			g_pShuttleChild->m_pSkill->SetSkillPatternType(SKILL_BASE_NUM(ItemInfo->SourceIndex)); // ���� ����
			g_pShuttleChild->m_pSkill->SetSkillPatternType(SKILL_BASE_NUM(ItemInfo->ItemNum)); // ���� ����
		}
		if(SKILL_KIND(ItemNum) == SKILL_KIND_ATTRIBUTE)
		{
			// ��ȭ
			// 2009-04-21 by bhsohn ������ DesParam�߰�
			//if(IS_EXIST_DES_PARAM(ItemInfo, DES_SKILL_CANCELALL))
			if(ItemInfo->IsExistDesParam(DES_SKILL_CANCELALL))
			{
//				if(!IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType,m_infoCharacter.CharacterInfo.InfluenceType) ||	// ����� �̰ų�
//					IsPkEnable())																											// PK���
//				{
					g_pShuttleChild->m_pSkill->ReleseSkill(DES_SKILL_CANCELALL);
//				}
			}


			// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����

			if( ItemInfo->IsExistDesParam( DES_SKILL_RELEASE ) )
				g_pShuttleChild->m_pSkill->ReleseSkill( DES_SKILL_RELEASE );

			// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����

		}
	}


	// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
	if( ItemInfo->SkillType == SKILLTYPE_TIMELIMIT )
	{
		if( ItemInfo->IsExistDesParam( DES_SKILL_MON_HOLD ) )
		{
			g_pShuttleChild->m_pSkill->SetMonsterHoldState( TRUE );
			g_pShuttleChild->SetShuttleSpeed();
		}
	}
	// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
}


void CSkillInfo::ReUsingSkill()
{
	ReleaseParamFactor();
	if(	m_dwState == SKILL_STATE_USING )
	{
		g_pGameMain->m_pInfSkill->DeleteSkillInfo(this, Possess);
		SendCancelSkill();
	}
	if(	m_dwState == SKILL_STATE_USING ||
		m_dwState == SKILL_STATE_WAIT_REATTACK ||
		m_dwState == SKILL_STATE_RELEASE )
	{
		ChangeSkillState(SKILL_STATE_WAITING);
	}
}

BOOL CSkillInfo::IsEnableStateForReUsing()
{
	if(	ItemInfo->SkillType == SKILLTYPE_TIMELIMIT &&
		m_fCheckReattackTime <= 0)
		return TRUE;
	return FALSE;
}

void CSkillInfo::SendUseSkill()
{
	MSG_FC_SKILL_USE_SKILL sMsg;
	sMsg.AttackIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
	sMsg.SkillItemID.ItemNum = ItemNum;
	map<int, CSkillInfo*>::iterator itSkillInfo = g_pShuttleChild->m_pSkill->m_mapSkill.find(ItemNum);
	if(itSkillInfo != g_pShuttleChild->m_pSkill->m_mapSkill.end())
	{
		sMsg.SkillItemID.ItemUID = itSkillInfo->second->UniqueNumber;
	}
	else
	{
		// ���ܻ�Ȳ �߻����� ����..
		DBGOUT("Send Skill : Can't Find Skill Information .\n");
		return;
	}
	if(ItemInfo->SkillTargetType == SKILLTARGETTYPE_ME)
	{
		sMsg.TargetIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex; // ������ ����� ��ų�̴�.
	}
	else if(ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE ||
			ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCLUDE_ME ||
			ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE ||
			ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME ||
			ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCEPT_OURS)
	{
		sMsg.TargetIndex = g_pShuttleChild->m_pSkill->GetTargetIndex();				// Ÿ�Ͽ��� ����� ��ų�̴�.
		sMsg.TargetCharUID = g_pShuttleChild->m_pSkill->GetCharacterUID();			// ����ũ�ѹ��� ����� ��ų
	}
	else
	{																				// ��Ƽ���� ����� ��ų, �ֺ����� ����� ��ų ���...
		sMsg.TargetIndex = 0;														// �������� �������� ó���Ѵ�.
		sMsg.TargetCharUID = 0;
	}
	SetClientIndex(sMsg.AttackIndex, sMsg.TargetIndex);								// 2006-09-14 by ispark
	g_pFieldWinSocket->SendMsg(T_FC_SKILL_USE_SKILL, (char*)&sMsg, sizeof(sMsg) );
	if(ItemInfo->AttackTime <= 0)
	{
		g_pShuttleChild->m_myShuttleInfo.CurrentSP -= ItemInfo->ReqSP;
	}
	else if(ItemInfo->SkillType == SKILLTYPE_CHARGING)
	{
		// ��¡�� ����Ʈ�� ���� ��ų �ѹ� ����
		g_pShuttleChild->m_pSkill->SetPrimarySkill(this);
		g_pShuttleChild->m_PrimaryAttack.nAttackCount = 0;
		g_pShuttleChild->m_PrimaryAttack.fCheckAttackTime = 0;
		g_pShuttleChild->m_PrimaryAttack.bShootingWeapon = FALSE;
	}

	// 2005-12-30 by ispark, �̹� Ÿ���� ������ ������ �������Ƿ� �ʿ䰡 ����. Ÿ���� ���� �ִٸ� �ʱ�ȭ
	if(g_pShuttleChild->m_pSkill->GetTargetIndex() != 0)
	{
		g_pShuttleChild->m_pSkill->SetTargetIndex(0);
		g_pShuttleChild->m_pSkill->SetCharacterUID(0);
	}
}


void CSkillInfo::SendCancelSkill(int nAttackSkillItemNum)
{
	if(	ItemInfo->SkillType == SKILLTYPE_TOGGLE ||
		ItemInfo->SkillType == SKILLTYPE_TIMELIMIT ||
		(ItemInfo->SkillType == SKILLTYPE_CLICK && SKILL_KIND(ItemNum) == SKILL_KIND_CONTROL) ||
		ItemInfo->SkillType == SKILLTYPE_CHARGING)
	{
		MSG_FC_SKILL_CANCEL_SKILL sMsg;
		memset(&sMsg,0x00,sizeof(sMsg));
		char buffer[SIZE_MAX_PACKET];
		int nType = T_FC_SKILL_CANCEL_SKILL;
		sMsg.SkillItemID.ItemNum = ItemNum;
		sMsg.SkillItemID.ItemUID = UniqueNumber;
		sMsg.AttackSkillItemNum0 = nAttackSkillItemNum;
		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
		g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));


		// ���λ��� Ư�� ó��
		if(IS_BAZAAR_SKILL(ItemInfo))
		{
			// ���λ��� ����
			g_pInterface->CloseBazaarShop();
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkillInfo::CheckUsingSkillToMeAtPrepare()
/// \brief		
/// \author		jschoi
/// \date		2004-10-15 ~ 2004-10-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkillInfo::CheckUsingSkillToMeAtPrepare()
{
	if(	ItemInfo->SkillTargetType == SKILLTARGETTYPE_ME ||				// ������ ����� ��ų�̰ų�.
		(ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE &&			// Ÿ��ONE ��ų�� ������ ����߰ų�.
		g_pShuttleChild->m_pSkill->GetTargetIndex() == g_pShuttleChild->m_myShuttleInfo.ClientIndex) ||			
		ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_WITH_ME ||	// ���� ������ ��Ƽ������ ����� ��ų�̰ų�.
		ItemInfo->SkillTargetType == SKILLTARGETTYPE_INRANGE_WITH_ME || // ���� ������ �� �ֺ������� ����� ��ų�̰ų�.
		ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE ||			// 2005-12-05 by ispark �ƹ��� �ϳ��� ��Ƽ��.
		ItemInfo->SkillTargetType == SKILLTARGETTYPE_ALL_ENEMY)			// 2006-11-23 by dgwoo Ÿ�� �߰�.
			{
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkillInfo::SetClientIndex(ClientIndex_t AttackIndex, ClientIndex_t TargetIndex)
/// \brief		
/// \author		ispark
/// \date		2006-09-14 ~ 2006-09-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkillInfo::SetClientIndex(ClientIndex_t AttackIndex, ClientIndex_t TargetIndex)
{
	m_AttackIndex = AttackIndex;
	m_TargetIndex = TargetIndex;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkillInfo::IsMustOneTargetSkill(BYTE i_bySkillTargetType)
/// \brief		�ϳ��� Ÿ���� ��ų.
/// \author		dgwoo
/// \date		2006-12-20 ~ 2006-12-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkillInfo::IsMustOneTargetSkill(BYTE i_bySkillTargetType)
{
	switch(i_bySkillTargetType)	
	{
	case SKILLTARGETTYPE_ONE:
	case SKILLTARGETTYPE_ONE_EXCLUDE_ME:
	case SKILLTARGETTYPE_PARTY_ONE:
	case SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME:
	case SKILLTARGETTYPE_ONE_EXCEPT_OURS:
		return TRUE;
	}					
	return FALSE;
}		

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkillInfo::PrepareSkillFromServer()
/// \brief		
/// \author		ispark
/// \date		2006-12-20 ~ 2006-12-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkillInfo::PrepareSkillFromServer()
{
	ChangeSkillState(SKILL_STATE_PREPARE);

	// 2009-04-21 by bhsohn ������ DesParam�߰�
// 	if(IS_EXIST_DES_PARAM(ItemInfo,DES_SKILL_HYPERSHOT))
// 	{//�����ۼ�.
// 	}
// 	else if(IS_EXIST_DES_PARAM(ItemInfo,DES_SKILL_BIG_BOOM))
// 	{// ��ս� �������� ���´�.
// 		g_pShuttleChild->m_bUnitStop			= TRUE;
// 		g_pShuttleChild->m_bPenaltyUnitStop		= TRUE;
// 		g_pShuttleChild->m_bMouseMoveLock		= TRUE;
// 		g_pShuttleChild->m_bSkillGoMove			= FALSE;
// 		SetReattackTime(ItemInfo->ReAttacktime);
// 	}else
// 	{// ��¡��.
// 	}
	if(ItemInfo->IsExistDesParam(DES_SKILL_HYPERSHOT))
	{//�����ۼ�.
	}
	else if(ItemInfo->IsExistDesParam(DES_SKILL_BIG_BOOM))
	{// ��ս� �������� ���´�.
		g_pShuttleChild->m_bUnitStop			= TRUE;
		g_pShuttleChild->m_bPenaltyUnitStop		= TRUE;
		g_pShuttleChild->m_bMouseMoveLock		= TRUE;
		g_pShuttleChild->m_bSkillGoMove			= FALSE;
		SetReattackTime(ItemInfo->ReAttacktime);
	}else
	{// ��¡��.
	}
	// end 2009-04-21 by bhsohn ������ DesParam�߰�
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkillInfo::IsSkillEnable()
/// \brief		
/// \author		// 2008-12-26 by bhsohn ����� ��� ���� ����
/// \date		2008-12-26 ~ 2008-12-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkillInfo::IsSkillEnable()
{
	return m_bSkillEnable;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkillInfo::IsSkillEnable()
/// \brief		
/// \author		// 2008-12-26 by bhsohn ����� ��� ���� ����
/// \date		2008-12-26 ~ 2008-12-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkillInfo::SetSkillEnable(BOOL i_bSkillEnable)
{
	m_bSkillEnable = i_bSkillEnable;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-05-07 by bhsohn ��������Ʈ ������ �ý���
/// \date		2013-05-16 ~ 2013-05-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkillInfo::DisableTimelimitSkill()
{
	if(!ItemInfo)
	{
		return;
	}
	if(SKILLTYPE_TIMELIMIT != ItemInfo->SkillType)
	{
		return;
	}
	m_fCheckEnableTime = 0.0f;
}