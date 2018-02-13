// OutPost.cpp: implementation of the COutPost class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OutPost.h"
#include "FieldIOCP.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutPost::COutPost(SOUTPOST_INFO * i_pSOutPostInfo, CFieldIOCP * i_pFIOCP)
{
	m_pFieldIOCP19	= i_pFIOCP;

	m_OutPostState = 0x00;
	m_OutPostMapIndex = i_pSOutPostInfo->OutPostMapIndex;
	m_OutPostCityMapIndex = i_pSOutPostInfo->OutPostCityMapIndex;
	m_OutPostGetTime = i_pSOutPostInfo->OutPostGetTime;
	m_OutPostNextWarTime = i_pSOutPostInfo->OutPostNextWarTime;
	m_OutPostNextWarTimeSet = i_pSOutPostInfo->OutPostNextWarTimeSet;
	m_OutPostInfluence = i_pSOutPostInfo->OutPostInfluence;
	m_OutPostGuildUID = i_pSOutPostInfo->OutPostGuildUID;
	util::strncpy(m_OutPostGuildName, i_pSOutPostInfo->OutPostGuildName, SIZE_MAX_GUILD_NAME);
	InitResetPossibleValues();
}

COutPost::~COutPost()
{

}

void COutPost::InitResetPossibleValues()
{
	m_OutPostResetStartTime.Reset();
	m_OutPostResetEndTime.Reset();
	m_OutPostResetPossibleGuildUID = 0;
	m_OutPostResetPossibleInfluence = 0;
	util::zero(m_OutPostResetPossibleGuildName, SIZE_MAX_GUILD_NAME);	
}

void COutPost::InitProtectorDamage()
{
	mt_auto_lock mta(&m_mtvectProtectorDamageInfo);
	m_mtvectProtectorDamageInfo.clear();
}

BYTE COutPost::GetOutPostState()
{
	return m_OutPostState;
}

void COutPost::SetOutPostState(BYTE i_byOutPostState)
{
	if(OUTPOST_NORMAL == i_byOutPostState)
	{
		m_OutPostState &= 0x00;
		return;
	}
	else if(!IS_OUTPOST(m_OutPostState))
	{
		m_OutPostState = i_byOutPostState;
	}
	else if(IS_OUTPOST_WAR(i_byOutPostState))
	{
		m_OutPostState &= 0x0F;
		m_OutPostState |= i_byOutPostState;
	}
	else if(IS_OUTPOST_RESET(i_byOutPostState))
	{
		m_OutPostState &= 0xF0;
		m_OutPostState |= i_byOutPostState;
	}

	// 2012-11-26 by jhseol, ���� �ý��� ������ - ����� Ʈ���� ��������(�Ͻ�����), ������ ���������� Ȯ���ϴ� �Լ�.(���������� üũ�� ����)
	if ( NULL != m_pFieldIOCP19 )
	{
		m_pFieldIOCP19->m_InflWarManager.SetDoingInfluenceWar();
	}
	// end 2012-11-26 by jhseol, ���� �ý��� ������ - ����� Ʈ���� ��������(�Ͻ�����), ������ ���������� Ȯ���ϴ� �Լ�.(���������� üũ�� ����)
}

INT COutPost::GetOutPostMapIndex()
{
	return m_OutPostMapIndex;
}

INT	COutPost::GetOutPostCityMapIndex()
{
	return m_OutPostCityMapIndex;
}

ATUM_DATE_TIME * COutPost::GetOutPostNextWarTime()
{
	return &m_OutPostNextWarTime;
}

BOOL COutPost::GetOutPostNextWarTimeSet()
{
	return m_OutPostNextWarTimeSet;
}

void COutPost::SetOutPostNextWarTimeSet(BOOL i_bNextWarTimeSet)
{
	m_OutPostNextWarTimeSet = i_bNextWarTimeSet;
}

BYTE COutPost::GetOutPostInfluence()
{
	return m_OutPostInfluence;
}

UID32_t COutPost::GetOutPostGuildUID()
{
	return m_OutPostGuildUID;
}

CHAR * COutPost::GetOutPostGuildName()
{
	return m_OutPostGuildName;
}

ATUM_DATE_TIME * COutPost::GetOutPostResetStartTime()
{
	return &m_OutPostResetStartTime;
}

ATUM_DATE_TIME * COutPost::GetOutPostResetEndTime()
{
	return &m_OutPostResetEndTime;
}

UID32_t COutPost::GetOutPostResetPossibleGuildUID()
{
	return m_OutPostResetPossibleGuildUID;
}

void COutPost::SetOutPostResetPossibleGuildUID(UID32_t i_nGuildUID)
{
	m_OutPostResetPossibleGuildUID = i_nGuildUID;
}

BYTE COutPost::GetOutPostResetPossibleInfluence()
{
	return m_OutPostResetPossibleInfluence;
}

void COutPost::SetOutPostResetPossibleInfluence(BYTE i_byInfluence)
{
	m_OutPostResetPossibleInfluence = i_byInfluence;
}

CHAR * COutPost::GetOutPostResetPossibleGuildName()
{
	return m_OutPostResetPossibleGuildName;
}
void COutPost::SetOutPostResetPossibleGuildName(CHAR * i_szGuildName)
{
	util::strncpy(m_OutPostResetPossibleGuildName, i_szGuildName, SIZE_MAX_GUILD_NAME);
}

ATUM_DATE_TIME * COutPost::GetPotPostGetTime()
{
	return &m_OutPostGetTime;
}

void COutPost::SetOutPostGetTime(ATUM_DATE_TIME * i_pCurrentTime)
{
	m_OutPostGetTime = * i_pCurrentTime;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	COutPost::CheckOutPostWarTime(ATUM_DATE_TIME * i_pCurrentTime)
/// \brief		���� �ð��� ���������� ���� �ð��� üũ�Ͽ� ���� �ð��� ��ȯ�Ѵ�.
/// \author		dhjin
/// \date		2007-08-17 ~ 2007-08-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	COutPost::CheckOutPostWarTime(ATUM_DATE_TIME * i_pCurrentTime)
{
	return m_OutPostNextWarTime.GetTimeDiffTimeInMinutes(*i_pCurrentTime);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL COutPost::CheckSetOutPostWarDay(ATUM_DATE_TIME * i_pCurrentTime)
/// \brief		���� �ð��� ���������� ���� ���ڸ� üũ�Ͽ� ���� ���ڰ� �������� �������� ���� ����Ʈ ���� ���ڷ� �����Ѵ�.
/// \author		dhjin
/// \date		2007-09-03 ~ 2007-09-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL COutPost::CheckSetOutPostWarDay(ATUM_DATE_TIME * i_pCurrentTime)
{
	if(m_OutPostNextWarTimeSet)
	{// 2007-09-03 by dhjin, ���� �������� ���� �ð� ������ �Ұ����ϸ� FALSE
		return FALSE;
	}

	if(m_OutPostNextWarTime.Year == i_pCurrentTime->Year
		&& m_OutPostNextWarTime.Month == i_pCurrentTime->Month
		&& m_OutPostNextWarTime.Day == i_pCurrentTime->Day)
	{// 2007-09-03 by dhjin, ���� �������� ���� �ð� ������ ������ ���ڸ� �������Ƿ� ���� ����.
		this->SetOutPostNextWarTimeSet(TRUE);
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPost::AddDamageOfProtector(BYTE i_byInfluence, UID32_t i_nGuildUID, CHAR * i_szGuildName, float i_fDamage)
/// \brief		�������� ��ȣ���� �������� �� ���ܰ� ������ ���� �����Ѵ�.
/// \author		dhjin
/// \date		2007-08-17 ~ 2007-08-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPost::AddDamageOfProtector(BYTE i_byInfluence, UID32_t i_nGuildUID, CHAR * i_szGuildName, float i_fDamage)
{
	if(!IS_OUTPOST_WAR(m_OutPostState))
	{// 2007-08-17 by dhjin, �������� ���� ������ üũ
		return;
	}

	mt_auto_lock mta(&m_mtvectProtectorDamageInfo);
	mtvectSOutPostProtectorDamage::iterator	itr = m_mtvectProtectorDamageInfo.begin();
	while(itr != m_mtvectProtectorDamageInfo.end())
	{
		if(i_nGuildUID == itr->GuildUID)
		{// 2007-08-17 by dhjin, ��ȣ���� �������� �� ���� �ִ� �����̸� �������� �ջ��Ѵ�.
			itr->Damage += i_fDamage;
			return;
		}
		itr++;
	}

	// 2007-08-17 by dhjin, �������� �� ���� ���� �����̹Ƿ� �߰��Ѵ�.
	SOutPostProtectorDamage	tmProtectorDamage;
	tmProtectorDamage.Influence	= i_byInfluence;
	tmProtectorDamage.GuildUID	= i_nGuildUID;
	tmProtectorDamage.Damage	= i_fDamage;
	util::strncpy(tmProtectorDamage.GuildName, i_szGuildName, SIZE_MAX_GUILD_NAME);
	m_mtvectProtectorDamageInfo.push_back(tmProtectorDamage);

}

struct sort_SOutPostProtectorDamage_By_SumOfDamage
{
	bool operator()(SOutPostProtectorDamage op1, SOutPostProtectorDamage op2)
	{
		return op1.Damage > op2.Damage;				// �������� ����
	}
};

///////////////////////////////////////////////////////////////////////////////
/// \fn			UID32_t COutPost::GetTopGuildOfProtectorDamage(mtvectSOutPostProtectorDamage *o_pvectDamageList)
/// \brief		// 2007-10-16 by cmkwon, �α� �߰� - �����߰�(mtvectSOutPostProtectorDamage *o_pvectDamageList), �������� ��ȣ���� �������� ���� ���� �� ������ ��ȯ�Ѵ�.
/// \author		dhjin
/// \date		2007-08-17 ~ 2007-08-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
UID32_t COutPost::GetTopGuildOfProtectorDamage(mtvectSOutPostProtectorDamage *o_pvectDamageList)
{
	if (0 == m_mtvectProtectorDamageInfo.size())
	{// 2007-08-17 by dhjin, �������� ���� ������ ���ٸ�
		return 0;
	}
	
	mt_auto_lock mta(&m_mtvectProtectorDamageInfo);
	sort(m_mtvectProtectorDamageInfo.begin(), m_mtvectProtectorDamageInfo.end(), sort_SOutPostProtectorDamage_By_SumOfDamage());

	// 2007-10-16 by cmkwon, �α� �߰� - �����߰�(mtvectSOutPostProtectorDamage *o_pvectDamageList)
	for(int i=0; i < m_mtvectProtectorDamageInfo.size(); i++)
	{
		o_pvectDamageList->push_back(m_mtvectProtectorDamageInfo[i]);
	}

	mtvectSOutPostProtectorDamage::iterator	itr = m_mtvectProtectorDamageInfo.begin();
	for(; itr != m_mtvectProtectorDamageInfo.end(); itr++)
	{
		if(m_pFieldIOCP19->m_GuildWarManager.CheckGuildStateDisMemberReady(itr->GuildUID)
			|| m_pFieldIOCP19->m_OutPostManager.CheckOutPostPossessByGuildUID(itr->GuildUID)	// 2007-12-13 by dhjin, ���������� �����ϰ� �ִ� ��������
			|| m_pFieldIOCP19->m_OutPostManager.CheckOutPostResetByGuildUID(itr->GuildUID)		// 2007-12-13 by dhjin, ���������� �����ϰ� �ִ� �������� 
			)
		{// 2007-11-09 by dhjin, ��ü ��û���� �����̸� ���� ������
			continue;
		}

		// 2007-08-24 by dhjin, ���� ������ ���� ����.
		this->SetOutPostResetPossibleInfluence(itr->Influence);
		this->SetOutPostResetPossibleGuildUID(itr->GuildUID);
		this->SetOutPostResetPossibleGuildName(itr->GuildName);

		return itr->GuildUID;
	}
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPost::SetOutPostResetStartTime(ATUM_DATE_TIME * i_pCurrentTime)
/// \brief		�������� ���� �ð� ���� �ð� ����	
/// \author		dhjin
/// \date		2007-08-20 ~ 2007-08-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPost::SetOutPostResetStartTime(ATUM_DATE_TIME * i_pCurrentTime)
{
	m_OutPostResetStartTime = *i_pCurrentTime;

	// 2007-10-02 by dhjin, ���� ���� �ð� �����Ѵ�.
	m_OutPostResetEndTime = m_OutPostResetStartTime;
	m_OutPostResetEndTime.Second = 0;
	m_OutPostResetEndTime.AddDateTime(0,0,0,0,OUTPOST_RESET,0);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	COutPost::CheckOutPostResetTime(ATUM_DATE_TIME * i_pCurrenTime)
/// \brief		���� �ð��� �������� ���� ���� �ð��� üũ�Ͽ� ���� �ð��� ��ȯ�Ѵ�.
/// \author		dhjin
/// \date		2007-08-20 ~ 2007-08-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	COutPost::CheckOutPostResetTime(ATUM_DATE_TIME * i_pCurrenTime)
{
	return i_pCurrenTime->GetTimeDiffTimeInMinutes(m_OutPostResetEndTime);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPost::SetOutPostPossessGuildAndInfluenceByResetSuccess()
/// \brief		�������� ���� ����, ���� ����
/// \author		dhjin
/// \date		2007-08-20 ~ 2007-08-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPost::SetOutPostPossessGuildAndInfluenceByResetSuccess()
{
	if(FALSE != m_OutPostResetPossibleGuildUID
		&& FALSE != m_OutPostResetPossibleInfluence)
	{// 2007-08-20 by dhjin, ���� ���� ����, ������ �����ϴ��� üũ
		// 2007-08-20 by dhjin, �����ϸ� ���� �õ� ���̴� �����̹Ƿ� ���� �����ϰ� �ʱ�ȭ�Ѵ�.
		m_OutPostInfluence	= m_OutPostResetPossibleInfluence;
		m_OutPostGuildUID	= m_OutPostResetPossibleGuildUID;
		util::strncpy(m_OutPostGuildName, m_OutPostResetPossibleGuildName, SIZE_MAX_GUILD_NAME);

		this->InitResetPossibleValues();
	}
	else
	{// 2007-08-20 by dhjin, ���� ���� ������ ���ٸ� ���� �����̴�.
		this->SetOutPostPossessNPCByReset();
		this->InitResetPossibleValues();
	}
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPost::SetOutPostPossessNPCByReset()
/// \brief		�������� NPC����
/// \author		dhjin
/// \date		2007-08-20 ~ 2007-08-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPost::SetOutPostPossessNPCByReset()
{
	this->InitResetPossibleValues();

	m_OutPostInfluence = 0;
	m_OutPostGuildUID = 0;
	util::zero(m_OutPostGuildName, SIZE_MAX_GUILD_NAME);	
	
	this->InitProtectorDamage();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPost::SetOutPostPossessInfluenceByReset()
/// \brief		�������� ���¼���
/// \author		dhjin
/// \date		2007-10-01 ~ 2007-10-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPost::SetOutPostPossessInfluenceByReset()
{
	this->InitResetPossibleValues();

	this->InitProtectorDamage();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPost::SetOutPostPossessByWarEnd()
/// \brief		�������� ������ ������ ���� �� ����
/// \author		dhjin
/// \date		2007-08-20 ~ 2007-08-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPost::SetOutPostPossessByWarEnd()
{
	this->InitResetPossibleValues();

	// 2007-08-20 by dhjin, �������� ���� �ð� ����
	ATUM_DATE_TIME OutPostGetTime;
	this->SetOutPostGetTime(&OutPostGetTime.GetCurrentDateTime());

	this->InitProtectorDamage();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPost::SetOutPostNextWarTime(ATUM_DATE_TIME *i_pNextWarTime)
/// \brief		�������� �� ���� �ð� ���� (��,�и�)
/// \author		dhjin
/// \date		2007-08-20 ~ 2007-08-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPost::SetOutPostNextWarTime(ATUM_DATE_TIME *i_pNextWarTime)
{
	// 2007-10-30 by dhjin, 24:00:00 ������������ ����Ǹ� ���� ��¥�� �Ѿ�� +OUTPOST_NEXTWARGAP �Ǵ� ���׷� ����
//	m_OutPostNextWarTime.Reset();
//	m_OutPostNextWarTime.SetDateTime(m_OutPostGetTime.Year, m_OutPostGetTime.Month, m_OutPostGetTime.Day, i_pNextWarTime->Hour, i_pNextWarTime->Minute, 0);
	m_OutPostNextWarTime.SetDateTime(m_OutPostNextWarTime.Year, m_OutPostNextWarTime.Month, m_OutPostNextWarTime.Day, i_pNextWarTime->Hour, i_pNextWarTime->Minute, 0);
	m_OutPostNextWarTime.AddDateTime(0,0,OUTPOST_NEXTWARGAP,0,0,0);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPost::SetOutPostNextWarTimeByChoiceUser(ATUM_DATE_TIME *i_pNextWarTime)
/// \brief		�������� �� ���� �ð� ���� (������ �ð� ���� ��, ����~!: �������� ������ ���� ���İ� �ƴϹǷ� OUTPOST_NEXTWARGAP�� ������ �ʴ´�) 
/// \author		dhjin
/// \date		2007-11-05 ~ 2007-11-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPost::SetOutPostNextWarTimeByChoiceUser(ATUM_DATE_TIME *i_pNextWarTime)
{
	m_OutPostNextWarTime.Hour	= i_pNextWarTime->Hour;
	m_OutPostNextWarTime.Minute	= i_pNextWarTime->Minute;
	m_OutPostNextWarTime.Second	= 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPost::SetOutPostPossessInfluence(BYTE i_byInfluence)
/// \brief		�������� ���� ���� ����
/// \author		dhjin
/// \date		2008-03-26 ~ 2008-03-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPost::SetOutPostPossessInfluence(BYTE i_byInfluence)
{
	m_OutPostInfluence = i_byInfluence;
}

// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý���
///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPost::SetOutPostWarStartTime()
/// \brief		���� ���۽ð��� ���� �ð����� ����
/// \author		jhseol
/// \date		2013-03-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPost::SetOutPostWarStartTime()
{
	m_OutPostNextWarTime.SetCurrentDateTime();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPost::InitOutPostInfluence()
/// \brief		�������� �ʱ�ȭ
/// \author		jhseol
/// \date		2013-03-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPost::InitOutPostInfluence()
{
	m_OutPostInfluence = 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPost::InitOutPostGuildUID()
/// \brief		������� �ʱ�ȭ
/// \author		jhseol
/// \date		2013-03-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPost::InitOutPostGuildUID()
{
	m_OutPostGuildUID = 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPost::InitOutPostGuildName()
/// \brief		������� �ʱ�ȭ
/// \author		jhseol
/// \date		2013-03-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPost::InitOutPostGuildName()
{
	util::zero(m_OutPostGuildName, SIZE_MAX_GUILD_NAME);	
}
// end 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý���
