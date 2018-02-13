// INFOpMain.cpp: implementation of the CINFOpMain class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "AtumSound.h"
#include "QuestData.h"
#include "StoreData.h"
#include "ItemInfo.h"
#include "Interface.h"
#include "AtumDatabase.h"


#include "INFOpMain.h"

// ������ ���� 10������ �˷���
#define WAR_MONSTER_WARNING_TIME					600.0f // 60.0f*10 =  1��*60��

#define		FIRST_SHOW_DELAY_TIME			(5.0f)

#define		NARRATION_DELAY			1.0f
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFOpMain::CINFOpMain()
{
	m_pResGameData = NULL;
	m_pDataHeader = NULL;

	m_pINFOpWnd = NULL;
	m_pINFOpUserHelper = NULL;
	m_nShowWnd = 0;
	//m_nShowWnd |= (1<<SHOW_OP_USER_HELPER);
	//m_nShowWnd |= (1<<SHOW_OP_WND);
	//m_pINFOpNPC = NULL;
		
	m_fWarMonsterCreated = -1;
	m_vecPreWarMonsterInfo.clear();
	m_vecWarMonsterInfo.clear();

	ZERO_MEMORY(m_chNarration);
	// 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
	ZERO_MEMORY(m_chErNarration);

	m_fNarrationTime = -1;
	
	// 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����
	m_pINFOpInfo = NULL;
	// end 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����
}

CINFOpMain::~CINFOpMain()
{	
	m_vecPreWarMonsterInfo.clear();
	m_vecWarMonsterInfo.clear();

	CINFOpMain::DeleteDeviceObjects();	
	util::del(m_pResGameData);
}

HRESULT CINFOpMain::InitDeviceObjects()
{
	DeleteDeviceObjects();

	char strPath[256];
	g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_TEXTURE, "operator.tex");
	GetResourceFile(strPath);
	
	{
		if(NULL == m_pINFOpWnd)
		{
			m_pINFOpWnd = new CINFOpWnd(this);
		}
		m_pINFOpWnd->InitDeviceObjects();
	}
	// 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����
	if(m_pINFOpInfo == NULL)
	{
		m_pINFOpInfo = new CINFOpInfo(this);
	}
	m_pINFOpInfo->InitDeviceObjects();
	// end 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����
	
	{
		if(NULL == m_pINFOpUserHelper)
		{
			m_pINFOpUserHelper = new CINFOpUserHelper(this);
		}
		m_pINFOpUserHelper->InitDeviceObjects();
	}
//	{
//		if(NULL == m_pINFOpNPC)
//		{
//			m_pINFOpNPC = new CINFOpNPC(this);
//		}
//		m_pINFOpNPC->InitDeviceObjects();
//	}
	return S_OK ;
}

HRESULT CINFOpMain::RestoreDeviceObjects()
{
	m_pINFOpWnd->RestoreDeviceObjects();
	m_pINFOpUserHelper->RestoreDeviceObjects();
	//m_pINFOpNPC->RestoreDeviceObjects();
	// 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����
	m_pINFOpInfo->RestoreDeviceObjects();
	// end 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����
	return S_OK ;
}

HRESULT CINFOpMain::DeleteDeviceObjects()
{	
	if(m_pINFOpWnd)
	{
		m_pINFOpWnd->DeleteDeviceObjects();
		util::del(m_pINFOpWnd);
	}
	if(m_pINFOpUserHelper)
	{
		m_pINFOpUserHelper->DeleteDeviceObjects();
		util::del(m_pINFOpUserHelper);
	}
//	if(m_pINFOpNPC)
//	{
//		m_pINFOpNPC->DeleteDeviceObjects();
//		util::del(m_pINFOpNPC);
//	}
	// 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����
	if(m_pINFOpInfo)
	{
		m_pINFOpInfo->DeleteDeviceObjects();
		util::del(m_pINFOpInfo);
	}
	// end 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����
	util::del(m_pDataHeader);

	return S_OK ;
}

HRESULT CINFOpMain::InvalidateDeviceObjects()
{
	m_pINFOpWnd->InvalidateDeviceObjects();
	m_pINFOpUserHelper->InvalidateDeviceObjects();
	//m_pINFOpNPC->InvalidateDeviceObjects();
	// 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����
	m_pINFOpInfo->InvalidateDeviceObjects();
	// end 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����
	return S_OK ;
}

void CINFOpMain::RenderOpHelper(DWORD dwGameState)
{
	BOOL bShowShop = g_pInterface->IsBuildingShow();
	if(bShowShop)
	{
		return;
	}	
	
	switch(dwGameState)
	{	
	case _GAME:
		{
			if(IsShowWnd(SHOW_OP_USER_HELPER))
			{
				m_pINFOpUserHelper->Render();
			}			
		}
		break;
	}
}
void CINFOpMain::RenderOpWnd()
{
	BOOL bShowShop = g_pInterface->IsBuildingShow();
	if(bShowShop)
	{
		return;
	}
	if(IsShowWnd(SHOW_OP_WND))
	{
		m_pINFOpWnd->Render();
	}			
// 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����	
	if(m_pINFOpInfo)
    {
	    m_pINFOpInfo->Render();								
    }
// end 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����	
}

void CINFOpMain::Tick(DWORD dwGameState)
{
	BOOL bShowShop = g_pInterface->IsBuildingShow();
	if(bShowShop)
	{
		return;
	}
	if(m_fNarrationTime >= 0)
	{
		m_fNarrationTime -= g_pD3dApp->GetCheckElapsedTime();
		if(m_fNarrationTime < 0)
		{
			if(strlen(m_chNarration) >1)
			{
				// 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
				//g_pD3dApp->StartNarrationSound(m_chNarration);
				g_pD3dApp->StartNarrationSound(m_chNarration, m_chErNarration);
				ZERO_MEMORY(m_chErNarration);
				// end 2009-01-23 by bhsohn �����̼� ������ ���� ó��				
				ZERO_MEMORY(m_chNarration);
			}
		}
	}

	if(IsShowWnd(SHOW_OP_WND))
	{
		m_pINFOpWnd->Tick();
	}
	switch(dwGameState)
	{
	case _GAME:
		{
			if(IsShowWnd(SHOW_OP_USER_HELPER))
			{
				m_pINFOpUserHelper->Tick();
			}
		}
		break;
	}
	RefreshWarMonsterInfo();

	TickWarMonsterInfo();
// 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����	
	if(m_pINFOpInfo)
    {
	    m_pINFOpInfo->Tick();
    }													
// end 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����	
			
}

void CINFOpMain::RefreshWarMonsterInfo()
{
	if(m_vecPreWarMonsterInfo.empty())
	{
		return;
	}		
	
	ATUM_DATE_TIME curServerTime = GetServerDateTime();
	
	if( (0 == curServerTime.Year)
		&&( 0 == curServerTime.Month)
		&&( 0 == curServerTime.Day)
		&&( 0 == curServerTime.Minute))
	{
		return;
	}
	
	vector<structPreWarMonsterInfo>::iterator it = 	m_vecPreWarMonsterInfo.begin();
	while(it != m_vecPreWarMonsterInfo.end())
	{
		structPreWarMonsterInfo struPreWarMonsterInfo = (*it);
		AddStrategyPointInfo(struPreWarMonsterInfo.MapIndex, 
								struPreWarMonsterInfo.MapInfluenceType, 
								struPreWarMonsterInfo.chStringTxt,
								&struPreWarMonsterInfo.timeSummonMonsterTime);
		it++;
	}
	m_vecPreWarMonsterInfo.clear();	
}

void CINFOpMain::TickWarMonsterInfo()
{
	if(m_vecWarMonsterInfo.empty())
	{
		return;
	}
	float fCheckTime = g_pD3dApp->GetCheckElapsedTime();
	vector<structWarMonsterInfo>::iterator it = 	m_vecWarMonsterInfo.begin();

	// 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
	BOOL bRqRefrehshWarMonter = FALSE;

	while(it != m_vecWarMonsterInfo.end())
	{
		// 2008-03-07 by bhsohn ��������, �Ʒ��� �̿� ���ϰ� ����
//		if((*it).fSummonLiveTime > WAR_MONSTER_WARNING_TIME)
//		{
//			(*it).fSummonLiveTime -= fCheckTime;
//		}
//		else
//		{
//			PopupWarSummonLiveTime10Minute((*it).MapIndex);
//
//			it = m_vecWarMonsterInfo.erase(it);
//			continue;		
//		}
		(*it).fSummonLiveTime -= fCheckTime;
		if((*it).fSummonLiveTime < 0)
		{
			it = m_vecWarMonsterInfo.erase(it);
			// 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
			bRqRefrehshWarMonter = TRUE;
			continue;
		}
		else if((*it).fSummonLiveTime < WAR_MONSTER_WARNING_TIME)
		{
			if(FALSE == (*it).bPopupOp)
			{
				PopupWarSummonLiveTime10Minute((*it).MapIndex);
				(*it).bPopupOp = TRUE;
			}			
		}
		// end 2008-03-05 by bhsohn ��������, �Ʒ��� �̿� ���ϰ� ����
		
		it++;
	}
	// 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
	if(bRqRefrehshWarMonter)
	{
		g_pInterface->RefreshWarSPWarInfo();	
	}
	// end 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
}
void CINFOpMain::DetroySummonMonster(MapIndex_t	MapIndex)
{
	vector<structWarMonsterInfo>::iterator it = 	m_vecWarMonsterInfo.begin();
	while(it != m_vecWarMonsterInfo.end())
	{
		if(MapIndex == (*it).MapIndex)
		{
			it = m_vecWarMonsterInfo.erase(it);
			continue;
		}

		it++;
	}

}
// ���� ���� 10����
void CINFOpMain::PopupWarSummonLiveTime10Minute(MapIndex_t	MapIndex)
{
	m_pINFOpWnd->ResetOpWndText();
	
	char chBufTmp[256]; 
	memset(chBufTmp, 0x00, 256);	

	char chTime[128];
	memset(chTime, 0x00, 128);
	int nSecond = WAR_MONSTER_WARNING_TIME;
	int nMinute = nSecond/60;
	int nHour = 0;

	
	// 2009. 02. 03 by ckPark �������� 10�� ���۷����ͽ� ���̸� ����
	//wsprintf(chBufTmp, STRMSG_C_070718_0231);
	wsprintf( chBufTmp, STRMSG_C_090204_0301, g_pGameMain->GetMapNameByIndex( MapIndex ) );
	// end 2009. 02. 03 by ckPark �������� 10�� ���۷����ͽ� ���̸� ����


	wsprintf(chTime, STRMSG_C_070206_0202, nHour, nMinute);

	m_pINFOpWnd->ResetOpWndText();
	m_pINFOpWnd->SetOpWndText(chBufTmp);
	m_pINFOpWnd->SetOpWndTimeText(chTime);	

	// �˾� ����
	m_pINFOpWnd->ShowOpWnd(OPWND_BK_IMAGE_INFLWAR);

	// ��� ������ �� ������ Show
	ShowOpWnd(TRUE);

	
	// 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
	
	m_fNarrationTime = NARRATION_DELAY;
	ZERO_MEMORY(m_chNarration);
	
	// �Ϲ� ������ ���
	sprintf(m_chNarration, "op_war_point_1_2");
	if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_NORMAL ) )
		sprintf(m_chNarration, "op_war_point_1_2");
	else if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_VCN ) )
		sprintf(m_chNarration, "op_war_point_1_2");// ����������(�Ϲ� ���°� ����)
	else if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI ) )
		sprintf(m_chNarration, "op_war_point_1_2_ANI");// �˸���	
	// end 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
	
	// 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
	ZERO_MEMORY(m_chErNarration);
	sprintf(m_chErNarration, "op_war_point_1_2");// �˸���	
	// end 2009-01-23 by bhsohn �����̼� ������ ���� ó��
}

int CINFOpMain::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bShowShop = g_pInterface->IsBuildingShow();
	if(bShowShop)
	{
		return INF_MSGPROC_NORMAL;
	}
	if(IsShowWnd(SHOW_OP_USER_HELPER))
	{
		if( INF_MSGPROC_BREAK == m_pINFOpUserHelper->WndProc(uMsg, wParam, lParam))
		{
			return INF_MSGPROC_BREAK;
		}
	}
	if(IsShowWnd(SHOW_OP_WND))
	{
		if( INF_MSGPROC_BREAK == m_pINFOpWnd->WndProc(uMsg, wParam, lParam))
		{
			return INF_MSGPROC_BREAK;
		}		
	}
	return INF_MSGPROC_NORMAL;		
}


HRESULT CINFOpMain::GetResourceFile(char* szFileName)
{
	FLOG( "CINFGameMain::SetResourceFile(char* szFileName)" );
	util::del(m_pResGameData);
	m_pResGameData = new CGameData;
	m_pResGameData->SetFile(szFileName, FALSE, NULL, 0);

	return S_OK;
}

DataHeader * CINFOpMain::FindResource(char* szRcName)
{
	FLOG( "CINFGameMain::FindResource(char* szRcName)" );
	DataHeader* pHeader = NULL;
	if(m_pResGameData)
	{
		pHeader = m_pResGameData->Find(szRcName);
	}
	return pHeader;
}

BOOL CINFOpMain::IsShowWnd(int nIdx)
{
	if(m_nShowWnd & (1<<nIdx))
	{
		return TRUE;
	}
	return FALSE;
}

void CINFOpMain::ShowOpUserHelper(BOOL bShow, int nHelpMode, char* pTxt/*=NULL*/)
{	
	if(bShow)
	{
		m_nShowWnd |= (1<<SHOW_OP_USER_HELPER);
		switch(nHelpMode)
		{
		case OPERATOR_USER_HELPER_FREESKA:
		{
			m_pINFOpUserHelper->StartFirstUserExplain();
		}
			break;
		case OPERATOR_USER_TUTORIAL_COMPLETE:
		{
			m_pINFOpUserHelper->TutorialComplete();			
		}
			break;
		case OPERATOR_USER_TUTORIAL_FAIL:
		{
			m_pINFOpUserHelper->TutorialFail();			
		}
			break;
		case OPERATOR_USER_TUTORIAL_HELPER:
		{
			m_pINFOpUserHelper->TutorialHelper(pTxt);
		}
			break;
		case OPERATOR_USER_TUTORIAL_ALL_COMPLETE:
			{
				m_pINFOpUserHelper->TutorialAllComplete();
			}
			break;
		case OPERATOR_USER_SEL_VCU:
		case OPERATOR_USER_SEL_ANI:
			{
				m_pINFOpUserHelper->SelInfluence(nHelpMode);
			}
			break;
		case OPERATOR_USER_TUTORIAL_NOT_L10:
			{
				m_pINFOpUserHelper->TutorialNOTL10();
			}
			break;
		}		
	}
	else
	{
		m_nShowWnd &= ~(1<<SHOW_OP_USER_HELPER);		
	}

}

void CINFOpMain::CloseFirstUserExplain()
{
	m_pINFOpUserHelper->CloseFirstUserExplain();
	// ������ī ����̷� ������ �� �̼��� �˷��ش�.
	g_pGameMain->UpdateMissionPopWnd(g_pShuttleChild->GetMyShuttleInfo().Level, -1);

}
// ���� ���򸻿��� Close��ư Ŭ��
void CINFOpMain::OnUserHelperClickCloseBtn(int nHelperMode)
{
	switch(nHelperMode)
	{
	case OPERATOR_USER_SEL_VCU:
	case OPERATOR_USER_SEL_ANI:
		{
			ShowOpUserHelper(FALSE, FALSE);
			g_pGameMain->OnClickFirstUserCloseBtn(); //ó�� ������ ������ ������ �ݱ� ��ư Ŭ��
		}
		break;
	}
	
}

void CINFOpMain::ShowOpWnd(BOOL bShow)
{
	if(FALSE == g_pInterface->GetOperatorMode())
	{
		m_nShowWnd &= ~(1<<SHOW_OP_WND);
		return;
	}
	if(bShow)
	{
		m_nShowWnd |= (1<<SHOW_OP_WND);
	}
	else
	{
		m_nShowWnd &= ~(1<<SHOW_OP_WND);
	}
}


// 2009. 01. 12 by ckPark ���� ���� �ý���
void CINFOpMain::ShowOpWarDeclare(BYTE Influence, ATUM_DATE_TIME MSWarStartTime, BOOL GiveUp, BYTE SelectCount)
{
	if(FALSE == g_pInterface->GetOperatorMode())
	{
		return;
	}

	// �Ϲ� ������ ��� �ƹ��͵� ����
	if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_NORMAL ) )
		return;

	m_pINFOpWnd->ResetOpWndText();	
	char	chBufTmp[MAX_PATH]; 
	memset(chBufTmp, 0x00, MAX_PATH);

	// ���� ���� �̸� ����
	char	*szOurInflName, *szEnemyInflName;
	if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI ) )
	{
		szOurInflName	= STRMSG_C_060210_0001;
		szEnemyInflName	= STRMSG_C_060210_0000;
	}
	
	else if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_VCN ) )
	{
		szOurInflName	= STRMSG_C_060210_0000;
		szEnemyInflName	= STRMSG_C_060210_0001;
	}
	else 
	{
		return;
	}


	// ���⿩�ο� ���� ��Ʈ�� ����
	if(GiveUp)	// "\\y����(%s)���� ����(%s)���� �������� �����Ͽ����ϴ�. (��������%d�� ����)\\y"
		sprintf(chBufTmp, STRMSG_C_090113_0326, szOurInflName, szEnemyInflName, SelectCount);
	else		// "\\y����(%s)���� ����(%s)���� %s %02d:%02d�� �������� �Ͽ����ϴ�.(��������%d�� ����)\\y"
	{
		std::string strYYYYMMDD;
		MSWarStartTime.GetLocalString_YYYYMMDD(MSWarStartTime.Year, MSWarStartTime.Month, MSWarStartTime.Day, strYYYYMMDD);
		sprintf(chBufTmp, STRMSG_C_090113_0319, szOurInflName, szEnemyInflName, strYYYYMMDD.c_str(), MSWarStartTime.Hour, MSWarStartTime.Minute, SelectCount); // "\\y����(%s)���� ����(%s)���� %d�� %d�� %d�� %d:%d�� �������� �Ͽ����ϴ�.(��������%d�� ����)\\y"
	}

	m_pINFOpWnd->SetOpWndText(chBufTmp);
	
	// �˾� ����
	m_pINFOpWnd->ShowOpWnd(OPWND_BK_IMAGE_INFLWAR);
	
	// ��� ������ �� ������ Show
	ShowOpWnd(TRUE);
}
// end 2009. 01. 12 by ckPark ���� ���� �ý���



void CINFOpMain::ShowOpMissionComplete(INT i_nExpOfCompensation, BYTE i_byBonusStatOfCompensation, vector<QUEST_PAY_ITEM_INFO> i_vecQuestPay)
{	
	if(FALSE == g_pInterface->GetOperatorMode())
	{
		return;
	}
	m_pINFOpWnd->ResetOpWndText();	
	char chBufTmp[256]; 
	memset(chBufTmp, 0x00, 256);
		
	// ������ �ϳ��� ���� ����Ʈ
	if((0 == i_nExpOfCompensation )
		&&(0 == i_vecQuestPay.size())
		&&(0 == i_byBonusStatOfCompensation))
	{
		wsprintf(chBufTmp, STRMSG_C_070712_0210);	//"�̼� �����Դϴ�."
		m_pINFOpWnd->SetOpWndText(chBufTmp);

		// �˾� ����
		m_pINFOpWnd->ShowOpWnd(OPWND_BK_IMAGE_QUEST);

		// ��� ������ �� ������ Show
		ShowOpWnd(TRUE);
		
		// 2007-08-13 by bhsohn �̼� ���� �����̼� ���� ó��		
		m_fNarrationTime = NARRATION_DELAY;
		ZERO_MEMORY(m_chNarration);


		// 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
		wsprintf(m_chNarration, "op_mission_end2");		

		// �Ϲ� ������ ���
		if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_NORMAL ) )
			sprintf(m_chNarration, "op_mission_end2");
		else if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_VCN ) )
			sprintf(m_chNarration, "op_mission_end2");	// ����������(�Ϲ� ���°� ����)
		else if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI ) )
			sprintf(m_chNarration, "op_mission_end2_ANI");// �˸���

		// end 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
		
		// 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
		ZERO_MEMORY(m_chErNarration);
		sprintf(m_chErNarration, "op_mission_end2");// �˸���	
		// end 2009-01-23 by bhsohn �����̼� ������ ���� ó��

		// end 2007-08-13 by bhsohn �̼� ���� �����̼� ���� ó��
		return;

	}
	
	wsprintf(chBufTmp, STRMSG_C_070712_0203);	//"�����մϴ�. ���ηκ��� ������ ���޵Ǿ����."
	m_pINFOpWnd->SetOpWndText(chBufTmp);

	if(i_nExpOfCompensation > 0)
	{
		wsprintf(chBufTmp, STRMSG_C_070712_0204, i_nExpOfCompensation); //"����ġ %d ����"
		m_pINFOpWnd->SetOpWndText(chBufTmp);
	}	

	if(i_byBonusStatOfCompensation > 0)
	{
		wsprintf(chBufTmp, STRMSG_C_070712_0211, i_byBonusStatOfCompensation); //"���ʽ� ���� %d ��"
		m_pINFOpWnd->SetOpWndText(chBufTmp);
	}	

	vector<QUEST_PAY_ITEM_INFO>::iterator it = i_vecQuestPay.begin();
	while(it != i_vecQuestPay.end())
	{
		QUEST_PAY_ITEM_INFO info = (*it);

		ITEM *pItem = g_pGameMain->GetServerItemInfo(info.ItemNum);
		if(pItem)
		{
			wsprintf(chBufTmp, STRMSG_C_070712_0205, pItem->ItemName, info.ItemCount); // "%s %d ����"
			m_pINFOpWnd->SetOpWndText(chBufTmp);
		}
		it++;
	}
	// �˾� ����
	m_pINFOpWnd->ShowOpWnd(OPWND_BK_IMAGE_QUEST);

	// ��� ������ �� ������ Show
	ShowOpWnd(TRUE);

	// 2007-08-13 by bhsohn �̼� ���� �����̼� ���� ó��
	m_fNarrationTime = NARRATION_DELAY;
	ZERO_MEMORY(m_chNarration);
	
	// 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
	wsprintf(m_chNarration, "op_mission_end");
	
	// �Ϲ� ������ ���
	if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_NORMAL ) )
		sprintf(m_chNarration, "op_mission_end");
	else if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_VCN ) )
		sprintf(m_chNarration, "op_mission_end");// ����������(�Ϲ� ���°� ����)
	else if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI ) )
		sprintf(m_chNarration, "op_mission_end_ANI");// �˸���
	
	// end 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�

	// 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
	ZERO_MEMORY(m_chErNarration);
	sprintf(m_chErNarration, "op_mission_end");// �˸���	
	// end 2009-01-23 by bhsohn �����̼� ������ ���� ó��


	// end 2007-08-13 by bhsohn �̼� ���� �����̼� ���� ó��
	
}

void CINFOpMain::AddStrategyPoint(char *pTxt, MapIndex_t	MapIndex, SHORT	MapInfluenceType, ATUM_DATE_TIME		CreateTime)
{	
	ATUM_DATE_TIME curServerTime = GetServerDateTime();
	BOOL bAddPre = FALSE;
	if( (0 == curServerTime.Year)
		&&( 0 == curServerTime.Month)
		&&( 0 == curServerTime.Day)
		&&( 0 == curServerTime.Minute))
	{
		bAddPre = TRUE;
	}
	if(!bAddPre)
	{
		AddStrategyPointInfo(MapIndex, MapInfluenceType, pTxt, &CreateTime);		
	}
	else
	{
		AddPreStrategyPointInfo(MapIndex, MapInfluenceType, pTxt, &CreateTime);		
	}
	
}

// ���� ����Ʈ ����
void CINFOpMain::AddStrategyPointInfo(MapIndex_t	MapIndex, short MapInfluenceType, char* pTxt, ATUM_DATE_TIME *pSummonMonsterTime)
{
	// 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�	
	
// 	char chNarration[64];
// 	memset(chNarration, 0x00, 64);
// 
//
// 	if(IS_MAP_INFLUENCE_VCN(MapInfluenceType)
// 		|| (IS_MAP_INFLUENCE_ANI(MapInfluenceType)))
// 	{
// 		wsprintf(chNarration, "op_war_point_1_1");
// 	}	
// 	else
// 	{		
// 		return;
// 	}
//
// 	// �����̼� 
// 	g_pD3dApp->StartNarrationSound(chNarration);

	// �� ���� ������ ����������, �˸��� �Ѵ� �ƴ� ��� �׳� ����
	if( !(IS_MAP_INFLUENCE_VCN(MapInfluenceType) || IS_MAP_INFLUENCE_ANI(MapInfluenceType)) )
		return;

	m_fNarrationTime = NARRATION_DELAY;
	ZERO_MEMORY(m_chNarration);

	sprintf(m_chNarration, "op_war_point_1_1");	
	// �Ϲ� ������ ���
	if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_NORMAL ) )
		sprintf(m_chNarration, "op_war_point_1_1");
	else if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_VCN ) )
		sprintf(m_chNarration, "op_war_point_1_1");// ����������(�Ϲ� ���°� ����)
	else if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI ) )
		sprintf(m_chNarration, "op_war_point_1_1_ANI");// �˸���
	
	// end 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�

	// 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
	ZERO_MEMORY(m_chErNarration);
	sprintf(m_chErNarration, "op_war_point_1_1");// �˸���	
	// end 2009-01-23 by bhsohn �����̼� ������ ���� ó��


	char chTime[128];
	memset(chTime, 0x00, 128);
	ATUM_DATE_TIME curServerTime = GetServerDateTime();
	// 2008-09-23 by bhsohn ������ ���� �ð��� 1�ð� �ʰ� �����ߴٰ� �α� ������ ���� ó��
	ATUM_DATE_TIME curEndTime = (*pSummonMonsterTime);	
	curEndTime.AddDateTime(0, 0, 0, WAR_MONSTER_LIVETIME_HOUR, 0, 0);	

	int nSecondRemainMonster = curEndTime.GetTimeInSeconds() - curServerTime.GetTimeInSeconds();
	if(nSecondRemainMonster < 0)
	{
		return;
	}
	// end 2008-09-23 by bhsohn ������ ���� �ð��� 1�ð� �ʰ� �����ߴٰ� �α� ������ ���� ó��
	int nWarMonsterLive = (3600*WAR_MONSTER_LIVETIME_HOUR);
	if(nSecondRemainMonster > nWarMonsterLive)
	{
		nSecondRemainMonster = nWarMonsterLive;
	}
	
	structWarMonsterInfo struWarMonsterInfo;
	memset(&struWarMonsterInfo, 0x00, sizeof(structWarMonsterInfo));

	// 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
	struWarMonsterInfo.MapInfluenceType = MapInfluenceType;
	struWarMonsterInfo.timeSummon = (*pSummonMonsterTime);
	// end 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
	
	struWarMonsterInfo.MapIndex = MapIndex;
	struWarMonsterInfo.fSummonLiveTime = nSecondRemainMonster;	// ������ �ð� 
	// 2008-03-07 by bhsohn ��������, �Ʒ��� �̿� ���ϰ� ����
	struWarMonsterInfo.bPopupOp		= FALSE;

	m_vecWarMonsterInfo.push_back(struWarMonsterInfo);
	
	if(nWarMonsterLive != nSecondRemainMonster )
	{		
		// ó�� ���ӽÿ� ���������� �ణ�� ������
		m_pINFOpWnd->SetFirstLoadTime(FIRST_SHOW_DELAY_TIME);
	}

	int nHour, nMinute;
	int nSecondRemainMotherShipTmp = nSecondRemainMonster;	
	nHour = (nSecondRemainMotherShipTmp/3600);
	nSecondRemainMotherShipTmp -= (nHour*3600);
	nMinute = (nSecondRemainMotherShipTmp/60);

	wsprintf(chTime, STRMSG_C_070206_0202, nHour, nMinute);

	m_pINFOpWnd->ResetOpWndText();
	m_pINFOpWnd->SetOpWndText(pTxt);
	m_pINFOpWnd->SetOpWndTimeText(chTime);

	// �˾� ����
	m_pINFOpWnd->ShowOpWnd(OPWND_BK_IMAGE_INFLWAR);

	// ��� ������ �� ������ Show
	ShowOpWnd(TRUE);	
}

void CINFOpMain::AddPreStrategyPointInfo(MapIndex_t	MapIndex, short MapInfluenceType, char* pTxt, ATUM_DATE_TIME *pSummonMonsterTime)
{
	structPreWarMonsterInfo struWarMonsterInfo;
	memset(&struWarMonsterInfo, 0x00, sizeof(structPreWarMonsterInfo));

	struWarMonsterInfo.MapIndex = MapIndex;
	struWarMonsterInfo.MapInfluenceType =  MapInfluenceType;
	strncpy(struWarMonsterInfo.chStringTxt, pTxt, strlen(pTxt)+1);
	memcpy(&struWarMonsterInfo.timeSummonMonsterTime, pSummonMonsterTime, sizeof(ATUM_DATE_TIME));

	m_vecPreWarMonsterInfo.push_back(struWarMonsterInfo);	
	
}

void CINFOpMain::ShowOpBossMonsterSummon(BYTE byBelligerence, int nHour, int nMinute)
{
	if(FALSE == g_pInterface->GetOperatorMode())
	{
		return;
	}
	BOOL bFirstLoad = FALSE;
	if(( MOTHERSHIP_LIVETIME_HOUR == nHour)&&(0 == nMinute))
	{		
		bFirstLoad = TRUE;		
	}
	if(FALSE == bFirstLoad)
	{
		m_pINFOpWnd->SetFirstLoadTime(FIRST_SHOW_DELAY_TIME);
	}
	
	m_pINFOpWnd->ResetOpWndText();

	char buffInfluence[128];
	char szArea[128];
	char buff[256];
	char buffTime[256];
	ZERO_MEMORY(buff);
	ZERO_MEMORY(szArea);
	ZERO_MEMORY(buffInfluence);
	ZERO_MEMORY(buffTime);

	char chNarration[64];	
	ZERO_MEMORY(chNarration);	

	if(byBelligerence == BELL_INFLUENCE_VCN)
	{			
		wsprintf(buffInfluence, STRMSG_C_060210_0000);		
		wsprintf(szArea, STRMSG_C_070712_0207);

		// 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
		wsprintf(m_chNarration, "op_war_bcu");

		m_fNarrationTime = NARRATION_DELAY;
		ZERO_MEMORY(m_chNarration);
		
		// �Ϲ� ������ ���
		if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_NORMAL ) )
			sprintf(m_chNarration, "op_war_bcu");
		else if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_VCN ) )
			sprintf(m_chNarration, "op_war_bcu");// ����������(�Ϲ� ���°� ����)
		else if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI ) )
			sprintf(m_chNarration, "op_war_bcu_ANI");// �˸���
		
		// end 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
		// 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
		ZERO_MEMORY(m_chErNarration);
		sprintf(m_chErNarration, "op_war_bcu");// �˸���	
		// end 2009-01-23 by bhsohn �����̼� ������ ���� ó��

	}
	else
	{		
		wsprintf(buffInfluence, STRMSG_C_060210_0001);		
		wsprintf(szArea, STRMSG_C_070712_0208);

		// 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
		wsprintf(m_chNarration, "op_war_ani");

		m_fNarrationTime = NARRATION_DELAY;
		ZERO_MEMORY(m_chNarration);
		
		// �Ϲ� ������ ���
		if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_NORMAL ) )
			sprintf(m_chNarration, "op_war_ani");
		else if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_VCN ) )
			sprintf(m_chNarration, "op_war_ani");// ����������(�Ϲ� ���°� ����)
		else if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI ) )
			sprintf(m_chNarration, "op_war_ani_ANI");// �˸���
		
		// end 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�

		// 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
		ZERO_MEMORY(m_chErNarration);
		sprintf(m_chErNarration, "op_war_ani");// �˸���	
		// end 2009-01-23 by bhsohn �����̼� ������ ���� ó��
	}


	// 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
	// �����̼�
	//g_pD3dApp->StartNarrationSound(chNarration);
	// end 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�

	wsprintf(buff, STRMSG_C_070712_0206, buffInfluence, szArea); // "%s���� ������ %s�� �����Ͽ����ϴ�."
	m_pINFOpWnd->SetOpWndText(buff);
	

	wsprintf(buffTime, STRMSG_C_070206_0202, nHour, nMinute);
	m_pINFOpWnd->SetOpWndTimeText(buffTime);


	// �˾� ����
	m_pINFOpWnd->ShowOpWnd(OPWND_BK_IMAGE_INFLWAR);
	
	// ��� ������ �� ������ Show
	ShowOpWnd(TRUE);
}

VOID CINFOpMain::ShowOpWndTxt(char* pTxt)
{
	if(FALSE == g_pInterface->GetOperatorMode())
	{
		return;
	}

	m_pINFOpWnd->ResetOpWndText();
	m_pINFOpWnd->SetOpWndText(pTxt);
	m_pINFOpWnd->ShowOpWnd(OPWND_BK_IMAGE_QUEST);
	// ��� ������ �� ������ Show
	ShowOpWnd(TRUE);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL IsRevivalUseItemWarMonster(MapIndex_t i_MapIndex)
/// \brief		�Է¹��� �ʿ��� ��Ȱ �������� ����Ҽ� �ִ°��ΰ�(��������)
/// \author		dgwoo
/// \date		2007-10-06 ~ 2007-10-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFOpMain::IsRevivalUseItemWarMonster(MapIndex_t i_MapIndex)
{
	vector<structWarMonsterInfo>::iterator it = m_vecWarMonsterInfo.begin();
	while(it != m_vecWarMonsterInfo.end())
	{
		MAP_INFO * pMapInfo = g_pDatabase->GetMapInfo(it->MapIndex);
		if(pMapInfo->MapIndex == i_MapIndex)
		{
			return TRUE;
		}
		if(pMapInfo->BeforeMapIndex == i_MapIndex
			|| (pMapInfo->BeforeMapIndex2 == i_MapIndex))	// 2008-06-20 by bhsohn ���?�ý��� �߰�
		{
			return TRUE;
		}
		it++;
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn   VOID CINFOpMain::ShowOpWndMultiTxt(char* pTxt)
/// \brief  
/// \author  // 2007-11-19 by bhsohn ���۷����� ������ �ؽ�Ʈ ó��
/// \date  2007-11-19 ~ 2007-11-19
/// \warning 
///
/// \param  
/// \return  
///////////////////////////////////////////////////////////////////////////////
VOID CINFOpMain::ShowOpWndMultiTxt(vector<string>* pvecEditText)
{
	if(FALSE == g_pInterface->GetOperatorMode())
	{
		return;
	}
	
	m_pINFOpWnd->ResetOpWndText();
	vector<string>::iterator it =  pvecEditText->begin();
	while(it != pvecEditText->end())
	{
		char* pTxt = (char*)(*it).c_str();
		if(pTxt)
		{
			m_pINFOpWnd->SetOpWndText(pTxt);
		}  
		it++;
	}
	
	m_pINFOpWnd->ShowOpWnd(OPWND_BK_IMAGE_QUEST);
	// ��� ������ �� ������ Show
	ShowOpWnd(TRUE);
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL IsDoingStrategyWar()
/// \brief		������ ���̳�?
/// \author		// 2008-03-07 by bhsohn ��������, �Ʒ��� �̿� ���ϰ� ����
/// \date		2008-03-07 ~ 2008-03-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFOpMain::IsDoingStrategyWar()
{
	int nStratePoint = m_vecWarMonsterInfo.size();
	if(0 == nStratePoint)
	{
		// �������� �ƴϴ�.
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
/// \date		2008-04-22 ~ 2008-04-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFOpMain::GetStrategyWarInfo(int nIdx, MapIndex_t	*o_MapIndex, ATUM_DATE_TIME *o_timeSummon, short *o_MapInfluenceType)
{
	if(nIdx< 0 || nIdx >= m_vecWarMonsterInfo.size())
	{
		return;
	}
	(*o_MapIndex) = m_vecWarMonsterInfo[nIdx].MapIndex;
	(*o_timeSummon) = m_vecWarMonsterInfo[nIdx].timeSummon;
	(*o_MapInfluenceType) = m_vecWarMonsterInfo[nIdx].MapInfluenceType;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
/// \date		2008-04-22 ~ 2008-04-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int  CINFOpMain::GetStrategyWarCnt()
{
	return m_vecWarMonsterInfo.size();
}