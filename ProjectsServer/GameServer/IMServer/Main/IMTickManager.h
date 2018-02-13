#ifndef _IM_TICK_MANAGER_H_
#define _IM_TICK_MANAGER_H_

#include "TickManager.h"
#include "IMIOCPSocket.h"

#define IM_DEFAULT_TICK_INTERVAL			500		// 0.5 second

// event types
#define IM_TICK_EVENT_START_GUILD_WAR		0

class CIMTickManager : public CTickManager
{
public:
	CIMTickManager(CIMIOCP *i_pIMIOCP, DWORD i_nTickInterval = IM_DEFAULT_TICK_INTERVAL);
	virtual ~CIMTickManager();

	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent);
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime);	// �Ź� �����ؾ� �� ��
	void DoDailyWork(ATUM_DATE_TIME *pDateTime);		// �Ϸ翡 �� �� �� �ؾ� �� ��
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime);		// �� �ð����� �� �� �� �ؾ� �� ��
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);		// �� �и��� �� �� �� �ؾ� �� ��
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime);		// �� �ʸ��� �� �� �� �ؾ� �� ��
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime);	// 2005-12-27 by cmkwon, �Ѵ޿� �� �� �� �ؾ� �� ��

public:
	CIMIOCP		*m_pIMIOCP4;
};

#endif // _ATUM_IM_TICK_MANAGER_H_
