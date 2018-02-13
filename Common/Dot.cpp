// Dot.cpp: implementation of the CDot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dot.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDot::CDot()
{
	this->InitDot();	// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
}

CDot::~CDot()
{

}

void CDot::InitDot() {
	mt_auto_lock mta(&m_mtvectDotInfo);
	m_mtvectDotInfo.clear();
}


/*******************************************************************************************************
**
**	��Ʈ ���� ���.
**
**	Create Info : ??. ??. ??
**
**	Update Info : 2010. 06. 08 by hsLee. - �뷱�� ���� �߰�. (i_fIncreasePowerRatio)
**
********************************************************************************************************/
void CDot::SetDot ( ITEM * i_pSkill , INT i_nDesParamIdx , ClientIndex_t i_nClientIndex , UID32_t i_nTimerUID , float i_fIncreasePowerRatio /*= 1.0f*/ ) 
{
	if ( NULL == i_pSkill ) 
	{	
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return;
	}

	DOTINFO	DotInfo;
		util::zero(&DotInfo, sizeof(DOTINFO));

	DotInfo.FirstTimerUID	= i_nTimerUID;
	DotInfo.ClientIndex		= i_nClientIndex;
	DotInfo.ItemNum			= i_pSkill->ItemNum;
	DotInfo.Kind			= i_pSkill->Kind;
	DotInfo.DesParam		= i_pSkill->ArrDestParameter[i_nDesParamIdx];

	// 2010. 06. 08 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ����.) - ��ų �뷱�� ����.
	//DotInfo.DesParamValue	= i_pSkill->ArrParameterValue[i_nDesParamIdx];
	DotInfo.DesParamValue	= i_pSkill->ArrParameterValue[i_nDesParamIdx] * i_fIncreasePowerRatio;

	DotInfo.Time			= i_pSkill->Time;

	m_mtvectDotInfo.pushBackLock(DotInfo);			// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
}

BOOL CDot::CheckApplyingDot(UID32_t i_nTimerUID, DestParam_t i_byDesParam)		// 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����)
{
	mt_auto_lock mta(&m_mtvectDotInfo);

	mtvectorDotInfo::iterator	itr = m_mtvectDotInfo.begin();

	while(itr != m_mtvectDotInfo.end()) 
	{
		if ( i_nTimerUID == itr->FirstTimerUID && i_byDesParam == itr->DesParam ) 
		{
			return TRUE;
		}
		itr++;
	}
	
	return FALSE;
}

BOOL CDot::DecreaseApplyingDotTime(UID32_t i_nTimerUID, DestParam_t i_byDesParam)		// 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����)
{
	mt_auto_lock mta(&m_mtvectDotInfo);

	mtvectorDotInfo::iterator	itr = m_mtvectDotInfo.begin();

	while ( itr != m_mtvectDotInfo.end() ) 
	{
		if ( i_nTimerUID == itr->FirstTimerUID && i_byDesParam == itr->DesParam ) 
		{
			itr->Time -= TIMER_DOT_INTERVAL;

			if ( itr->Time > 0 ) 
			{
				return TRUE;
			}
			else 
			{
				return FALSE;
			}
		}

		itr++;
	}
	
	return FALSE;
}

BOOL CDot::ReleaseDot(UID32_t i_nTimerUID) {
	mt_auto_lock mta(&m_mtvectDotInfo);

	mtvectorDotInfo::iterator	itr = m_mtvectDotInfo.begin();
	while(itr != m_mtvectDotInfo.end()) {
		if(i_nTimerUID == itr->FirstTimerUID) {
			itr = m_mtvectDotInfo.erase(itr);
			return TRUE;
		}
		itr++;
	}
	
	return FALSE;
}

BOOL CDot::ReleaseDotByCancelSkill(ITEM * i_pSkill, DestParam_t i_byDestParam) {		// 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����)
	if(NULL == i_pSkill) {
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return FALSE;
	}

	mt_auto_lock mta(&m_mtvectDotInfo);
	
	mtvectorDotInfo::iterator	itr = m_mtvectDotInfo.begin();
	while(itr != m_mtvectDotInfo.end()) {
		if(i_pSkill->ItemNum == itr->ItemNum		// 2009-09-09 ~ 2010-01-20 by dhjin, ���Ǵ�Ƽ - ���� ���� ��ų ��� �ȵǴ� ���� ����
			&&i_byDestParam  == itr->DesParam) {
			itr = m_mtvectDotInfo.erase(itr);
			return TRUE;
		}
		itr++;
	}
	
	return FALSE;
}

ClientIndex_t CDot::GetClientIdx(UID32_t i_nTimerUID) {
	mt_auto_lock mta(&m_mtvectDotInfo);
	mtvectorDotInfo::iterator	itr = m_mtvectDotInfo.begin();
	while(itr != m_mtvectDotInfo.end()) {
		if(i_nTimerUID == itr->FirstTimerUID) {
			return itr->ClientIndex;
		}
		itr++;
	}
	
	return FALSE;
}

void CDot::CopyDotInfo(UID32_t i_nTimerUID, DOTINFO * o_pDotInfo) {
	if(NULL == o_pDotInfo) {
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return;
	}

	mt_auto_lock mta(&m_mtvectDotInfo);
	mtvectorDotInfo::iterator	itr = m_mtvectDotInfo.begin();
	while(itr != m_mtvectDotInfo.end()) {
		if(i_nTimerUID == itr->FirstTimerUID) {
			o_pDotInfo->FirstTimerUID	= itr->FirstTimerUID;
			o_pDotInfo->ClientIndex		= itr->ClientIndex;
			o_pDotInfo->ItemNum			= itr->ItemNum;
			o_pDotInfo->DesParam		= itr->DesParam;
			o_pDotInfo->DesParamValue	= itr->DesParamValue;
			o_pDotInfo->Kind			= itr->Kind;
			o_pDotInfo->Time			= itr->Time;
			return;
		}
		itr++;
	}
}

INT CDot::MSG_FC_CHARACTER_DEBUFF_DOT_INFO_OK(MSG_DOT_INFO *o_pDotInfo) {
	if(NULL == o_pDotInfo) {
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return FALSE;
	}

	int	DotInfoCount = 0;
	
	mt_auto_lock mta(&m_mtvectDotInfo);
	mtvectorDotInfo::iterator itr = m_mtvectDotInfo.begin();
	for(; itr != m_mtvectDotInfo.end(); itr++) {
		o_pDotInfo[DotInfoCount].ItemNum			= itr->ItemNum; 
		DotInfoCount++;
	}
	
	return DotInfoCount;
}